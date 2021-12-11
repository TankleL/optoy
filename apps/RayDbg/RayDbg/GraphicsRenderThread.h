#pragma once
#include "pch.h"
#include "DeviceResources.h"

namespace winrt::raydbg {
    class GraphicsRenderThread {
    public:
        GraphicsRenderThread(dx::DeviceResources* dxres);
        void run_async();

        template<std::invocable F>
        void request_stop(F&& cb);
        
    private:
        void _work_thread(std::stop_token st);

    private:
        dx::DeviceResources* _dxres;
        std::mutex _mtx_creation;
        std::mutex _mtx_operation;
        std::unique_ptr<std::jthread> _worker;
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
        while (!st.stop_requested()) {
            std::unique_lock<std::mutex> lock(_mtx_operation);
            auto ctx = _dxres->get_d3dcontext();
            auto viewport = _dxres->get_screen_viewport();
            ctx->RSSetViewports(1, &viewport);
            ID3D11RenderTargetView* targets[] = { _dxres->get_back_render_target_view() };
            ctx->OMSetRenderTargets(1, targets, nullptr);
            ctx->ClearRenderTargetView(targets[0], DirectX::Colors::Gray);
            _dxres->present();
        }
    }
}
