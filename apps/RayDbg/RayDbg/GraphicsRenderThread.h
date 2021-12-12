#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
#include "Shaders/main_vs.hlsl.h"
#include "Shaders/main_ps.hlsl.h"

namespace winrt::raydbg {
    class GraphicsRenderThread {
    public:
        GraphicsRenderThread(dx::DeviceResources* dxres);
        void run_async();

        template<std::invocable F>
        void request_stop(F&& cb);
        Concurrency::critical_section& get_operation_mutex() { return _mtx_operation; }
        
    private:
        void _work_thread(std::stop_token st);
        void _tick();
        void _reset_device_resources();

    private:
        dx::DeviceResources* _dxres;
        std::mutex _mtx_creation;
        Concurrency::critical_section _mtx_operation;
        std::unique_ptr<std::jthread> _worker;
        winrt::com_ptr<ID3D11VertexShader> _mainvs;
        winrt::com_ptr<ID3D11PixelShader> _mainps;
    };

    inline GraphicsRenderThread::GraphicsRenderThread(dx::DeviceResources* dxres)
        : _dxres(dxres) {
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

    inline void GraphicsRenderThread::_work_thread(std::stop_token st) {
        _reset_device_resources();
        const auto et_device_reset = _dxres->device_reset(dx::DeviceResources::evhandler_t([this]() { _reset_device_resources(); }));
        while (!st.stop_requested()) {
            Concurrency::critical_section::scoped_lock lock(_mtx_operation);
            _tick();
            std::this_thread::yield();
        }
        _dxres->device_reset(et_device_reset);
    }

    inline void GraphicsRenderThread::_reset_device_resources() {
        auto* device = _dxres->get_d3ddevice();

        // create vertex shader
        auto code = dx::compile_shader(
            shaders::main_vs,
            "main_vs",
            "main",
            "vs_5_0");
        dx::throw_if_failed(device->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, _mainvs.put()));
    }

    inline void GraphicsRenderThread::_tick() {
        // reset the frame
        auto ctx = _dxres->get_d3dcontext();
        auto viewport = _dxres->get_screen_viewport();
        ctx->RSSetViewports(1, &viewport);
        ID3D11RenderTargetView* targets[] = { _dxres->get_back_render_target_view() };
        ctx->OMSetRenderTargets(1, targets, nullptr);
        ctx->ClearRenderTargetView(targets[0], DirectX::Colors::Gray);

        // draw objects

        // display
        _dxres->present();
    }
}
