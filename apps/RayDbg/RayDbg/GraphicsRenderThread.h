#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
#include "Shaders/main_vs.hlsl.h"
#include "Shaders/main_ps.hlsl.h"
#include "GraphicsEntities.h"

namespace winrt::raydbg {
    class GraphicsRenderThread final{
    public:
        GraphicsRenderThread();
        GraphicsRenderThread(GraphicsRenderThread&&) = default;
        GraphicsRenderThread(const GraphicsRenderThread&) = delete;
        void run_async();
        dx::DeviceResources& get_device_resources() { return _dxres; }
        template<std::invocable F> void request_stop(F&& cb);
        template<class F> void dispatch(F&& task);
        
    private:
        void _work_thread(std::stop_token st);
        void _run_tasks();
        void _tick();
        void _reset_device_resources();

    private:
        dx::DeviceResources _dxres;
        std::mutex _mtx_creation;
        Concurrency::critical_section _mtx_operation;
        std::unique_ptr<std::jthread> _worker;
        winrt::com_ptr<ID3D11VertexShader> _mainvs;
        winrt::com_ptr<ID3D11PixelShader> _mainps;
        winrt::com_ptr<ID3D11Buffer> _mainvb;
        winrt::com_ptr<ID3D11InputLayout> _input_layout;
        std::queue<std::function<void(GraphicsRenderThread&)>> _tasks;

    private:
        constexpr const static inline uint32_t _default_vertex_num = 128;
    };

    inline GraphicsRenderThread::GraphicsRenderThread() {
    }

    inline void GraphicsRenderThread::run_async() {
        std::scoped_lock<std::mutex> lock(_mtx_creation);
        assert(_worker == nullptr);
        _worker = std::make_unique<std::jthread>([this](auto st) {_work_thread(st); });
    }

    template<std::invocable F>
    inline void GraphicsRenderThread::request_stop(F&& cb) {
        std::thread killer([this, callback = std::forward<F&&>(cb)]() {
            std::unique_lock<std::mutex> lock(_mtx_creation);
            if (_worker) {
                _worker->request_stop();
                _worker->join();
                _worker = nullptr;
            }
            lock.unlock();
            callback();
        });
        killer.detach();
    }

    template<class F>
    inline void GraphicsRenderThread::dispatch(F&& task) {
        Concurrency::critical_section::scoped_lock lock(_mtx_operation);
        _tasks.emplace(std::forward<F>(task));
    }

    inline void GraphicsRenderThread::_work_thread(std::stop_token st) {
        _reset_device_resources();
        const auto et_device_reset = _dxres.device_reset(dx::DeviceResources::evhandler_t([this]() { _reset_device_resources(); }));
        while (!st.stop_requested()) {
            Concurrency::critical_section::scoped_lock lock(_mtx_operation);
            _run_tasks();
            _tick();
            std::this_thread::yield();
        }
        _dxres.device_reset(et_device_reset);
    }

    inline void GraphicsRenderThread::_reset_device_resources() {
        auto* device = _dxres.get_d3ddevice();

        // create a vertex shader
        auto vsc= dx::compile_shader(
            shaders::main_vs,
            "main_vs",
            "main",
            "vs_5_0");
        dx::throw_if_failed(device->CreateVertexShader(vsc->GetBufferPointer(), vsc->GetBufferSize(), nullptr, _mainvs.put()));

        // create a pixel shader
        auto psc = dx::compile_shader(
            shaders::main_ps,
            "main_ps",
            "main",
            "ps_5_0");
        dx::throw_if_failed(device->CreatePixelShader(psc->GetBufferPointer(), psc->GetBufferSize(), nullptr, _mainps.put()));

        // set up the input layout
        D3D11_INPUT_ELEMENT_DESC input_ele_desc[] = {
            { "POS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          /*
          { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
          */
        };
        dx::throw_if_failed(device->CreateInputLayout(
            input_ele_desc,
            ARRAYSIZE(input_ele_desc),
            vsc->GetBufferPointer(),
            vsc->GetBufferSize(),
            _input_layout.put()));

        // create default vertex buffer
        entities::vertex_t vertices[_default_vertex_num] = { 0 };
        D3D11_BUFFER_DESC vb_desc{ 0 };
        vb_desc.ByteWidth = sizeof(vertices);
        vb_desc.Usage = D3D11_USAGE_DEFAULT;
        vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA vb_data{ 0 };
        vb_data.pSysMem = vertices;
        dx::throw_if_failed(device->CreateBuffer(&vb_desc, &vb_data, _mainvb.put()));
    }

    inline void GraphicsRenderThread::_run_tasks() {
        while (_tasks.size() > 0) {
            _tasks.front()(*this);
            _tasks.pop();
        }
    }

    inline void GraphicsRenderThread::_tick() {
        // reset the frame
        auto ctx = _dxres.get_d3dcontext();
        auto viewport = _dxres.get_screen_viewport();
        ctx->RSSetViewports(1, &viewport);
        ID3D11RenderTargetView* targets[] = { _dxres.get_back_render_target_view() };
        ctx->OMSetRenderTargets(1, targets, nullptr);
        ctx->ClearRenderTargetView(targets[0], DirectX::Colors::Gray);

        // draw objects
        // test code:
        ctx->IASetInputLayout(_input_layout.get());
        ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        auto main_vb_ptr = _mainvb.get();
        UINT main_vb_stride = sizeof(entities::vertex_t);
        UINT main_vb_offset = 0;
        ctx->IASetVertexBuffers(0, 1, &main_vb_ptr, &main_vb_stride, &main_vb_offset);
        ctx->VSSetShader(_mainvs.get(), nullptr, 0);
        ctx->PSSetShader(_mainps.get(), nullptr, 0);

        ctx->Draw(2, 0);

        // display
        _dxres.present();
    }
}
