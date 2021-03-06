#pragma once
#include "pch.h" 

namespace dx {
    class DeviceResources final
    {
    public:
        using evhandler_t = winrt::delegate<void()>;

    public:
        explicit DeviceResources();
        void set_dpi(float dpi);
        void set_logical_size(winrt::Windows::Foundation::Size size);
        void validate_device();
        void handle_device_lost();
        void set_composition_scale(float scale_x, float scale_y);
        void set_swapchainpanel(winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel panel);
        D3D11_VIEWPORT get_screen_viewport() const { return _screenviewport; }
        ID3D11DeviceContext4* get_d3dcontext() const { return _d3dcontext.get(); }
        ID3D11Device3* get_d3ddevice() const { return _d3ddevice.get(); }
        ID3D11RenderTargetView1* get_back_render_target_view() const { return _d3drtview.get(); }
        void present();

    public:
        // events
        // probable paths
        // --> device_lost --> device_reset --> window_size_reset --> device_restored
        // --> device_reset 
        // --> window_size_reset
        winrt::event_token device_lost(evhandler_t const& handler) { return _device_lost.add(handler); }
        void device_lost(winrt::event_token const& token) { _device_lost.remove(token); }
        winrt::event_token device_reset(evhandler_t const& handler) { return _device_reset.add(handler); }
        void device_reset(winrt::event_token const& token) { _device_reset.remove(token); }
        winrt::event_token window_size_reset(evhandler_t const& handler) { return _window_size_reset.add(handler); }
        void window_size_reset(winrt::event_token const& token) { _window_size_reset.remove(token); }
        winrt::event_token device_restored(evhandler_t const& handler) { return _device_restored.add(handler); }
        void device_restored(winrt::event_token const& token) { return _device_restored.remove(token); }

    private:
        void _reset_dires(); // create device independent resources
        void _reset_ddres(); // create device dependent resources
        void _reset_wsdres(); // reset window size dependent resources
        void _update_rtsize(); // update render target size

    private:
        winrt::com_ptr<ID3D11Device3> _d3ddevice;
        winrt::com_ptr<ID3D11DeviceContext4> _d3dcontext;
        winrt::com_ptr<ID3D11RenderTargetView1> _d3drtview;
        winrt::com_ptr<ID3D11DepthStencilView> _d3ddsview;
        winrt::com_ptr<IDXGISwapChain3> _swapchain;
        D3D11_VIEWPORT _screenviewport;
        winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel _swapchain_panel;
        D3D_FEATURE_LEVEL _d3dfeaturelevel;
        winrt::Windows::Foundation::Size _d3drt_size;
        winrt::Windows::Foundation::Size _output_size;
        winrt::Windows::Foundation::Size _logical_size;
        float _dpi;
        float _composition_scale_x;
        float _composition_scale_y;
        float _effective_composition_scale_x;
        float _effective_composition_scale_y;
        float _effective_dpi;

        winrt::event<evhandler_t> _device_lost;
        winrt::event<evhandler_t> _device_reset;
        winrt::event<evhandler_t> _window_size_reset;
        winrt::event<evhandler_t> _device_restored;

    private:
        constexpr static inline float _dpi_threshold = 192.f;
        constexpr static inline float _width_threshold = 1920.f;
        constexpr static inline float _height_threshold = 1080.f;
    };
}


