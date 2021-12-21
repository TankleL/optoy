#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"
//#include "GraphicsEntities.h"

namespace winrt::raydbg {
    class GraphicsRenderThread final{
    public:
        using evhandler_t = winrt::delegate<void()>;

    public:
        GraphicsRenderThread();
        GraphicsRenderThread(GraphicsRenderThread&&) = default;
        GraphicsRenderThread(const GraphicsRenderThread&) = delete;
        void run_async();
        dx::DeviceResources& get_device_resources() { return _dxres; }
        template<std::invocable F> void request_stop(F&& cb);
        template<class F> void dispatch(F&& task);

    public:
        winrt::event_token drawing(evhandler_t const& handler) { return _drawing.add(handler); }
        void drawing(winrt::event_token const& token) { _drawing.remove(token); }
        
    private:
        void _work_thread(std::stop_token st);
        void _run_tasks();
        void _tick();

    private:
        dx::DeviceResources _dxres;
        std::mutex _mtx_creation;
        Concurrency::critical_section _mtx_operation;
        std::unique_ptr<std::jthread> _worker;
        std::queue<std::function<void(GraphicsRenderThread&)>> _tasks;

    private:
        winrt::event<evhandler_t> _drawing;
    };

    inline GraphicsRenderThread::GraphicsRenderThread() {
    }

    inline void GraphicsRenderThread::run_async() {
        std::scoped_lock<std::mutex> lock(_mtx_creation);
        assert(_worker == nullptr);
        _worker = std::make_unique<std::jthread>([this](auto st) {_work_thread(st); });
        auto handle = static_cast<HANDLE>(_worker->native_handle());
        ::SetThreadDescription(handle, L"[RayDBG]Graphics-Thread");
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
        while (!st.stop_requested()) {
            Concurrency::critical_section::scoped_lock lock(_mtx_operation);
            _run_tasks();
            _tick();
            std::this_thread::yield();
        }
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
        _drawing();

        // display
        _dxres.present();
    }
}
