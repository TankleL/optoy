#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"

namespace dx {
    DeviceResources::DeviceResources()
        : _screenviewport()
        , _d3dfeaturelevel(D3D_FEATURE_LEVEL_11_1) {
        _reset_dires();
        _reset_ddres();
    }

    void DeviceResources::set_dpi(float dpi) {
        if (_dpi != dpi) {
            _dpi = dpi;
            _reset_wsdres();
        }
    }
    
    void DeviceResources::set_logical_size(winrt::Windows::Foundation::Size size) {
        if (_logical_size != size) {
            _logical_size = size;
            _reset_wsdres();
        }
    }

    void DeviceResources::validate_device() {
        auto dxgi_dev = _d3ddevice.as<IDXGIDevice3>();

        winrt::com_ptr<IDXGIAdapter> adapter;
        dx::throw_if_failed(dxgi_dev->GetAdapter(adapter.put()));

        winrt::com_ptr<IDXGIFactory2> factory;
        dx::throw_if_failed(adapter->GetParent(IID_PPV_ARGS(factory.put())));

        winrt::com_ptr<IDXGIAdapter1> prev_default_adapter;
        dx::throw_if_failed(factory->EnumAdapters1(0, prev_default_adapter.put()));

        DXGI_ADAPTER_DESC1 prev_desc;
        dx::throw_if_failed(prev_default_adapter->GetDesc1(&prev_desc));

        winrt::com_ptr<IDXGIFactory4> curr_factory;
        dx::throw_if_failed(CreateDXGIFactory1(IID_PPV_ARGS(curr_factory.put())));

        winrt::com_ptr<IDXGIAdapter1> curr_default_adapter;
        dx::throw_if_failed(curr_factory->EnumAdapters1(0, curr_default_adapter.put()));

        DXGI_ADAPTER_DESC1 curr_desc;
        dx::throw_if_failed(curr_default_adapter->GetDesc1(&curr_desc));

        // if the adapter LUIDs don't match, or if the device reports that it has been removed,
        // a new D3D device must be created.
        if (prev_desc.AdapterLuid.LowPart != curr_desc.AdapterLuid.LowPart ||
            prev_desc.AdapterLuid.HighPart != curr_desc.AdapterLuid.HighPart ||
            FAILED(_d3ddevice->GetDeviceRemovedReason())) {
            // release all the references to the resources related to the outdated device.
            dxgi_dev = nullptr;
            adapter = nullptr;
            factory = nullptr;
            prev_default_adapter = nullptr;
            // create a new device and a swapchain.
            handle_device_lost();
        }
    }

    void DeviceResources::handle_device_lost() {
        _swapchain = nullptr;
        // TODO: notify device is lost
        _reset_ddres();
        _reset_wsdres();
        // TODO: notify device is restored
    }

    void DeviceResources::set_composition_scale(float scale_x, float scale_y) {
        if (!dx::equals(_composition_scale_x, scale_x) ||
            !dx::equals(_composition_scale_y, scale_y)) {
            _composition_scale_x = scale_x;
            _composition_scale_y = scale_y;
            _reset_wsdres();
        }
    }

    void DeviceResources::set_swapchainpanel(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel) {
        auto dinfo = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
        _swapchain_panel = panel;
        _logical_size = winrt::Windows::Foundation::Size{
            static_cast<float>(panel.ActualWidth()),
            static_cast<float>(panel.ActualHeight())};
        _composition_scale_x = panel.CompositionScaleX();
        _composition_scale_y = panel.CompositionScaleY();
        _dpi = dinfo.LogicalDpi();
        _reset_wsdres();
    }

    void DeviceResources::present() {
        DXGI_PRESENT_PARAMETERS params{ 0 };
        const auto hr = _swapchain->Present1(1, 0, &params);
        _d3dcontext->DiscardView1(_d3drtview.get(), nullptr, 0);
        _d3dcontext->DiscardView1(_d3ddsview.get(), nullptr, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
            handle_device_lost();
        }
        else {
            dx::throw_if_failed(hr);
        }
    }

    void DeviceResources::_reset_dires() {
    }

    void DeviceResources::_reset_ddres() {
        // This flag adds support for surfaces with a different color channel ordering
        // than the API default. It is required for compatibility with Direct2D.
        uint32_t creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
        if (sdk_layers_avaliable()) {
            creation_flags = creation_flags | D3D11_CREATE_DEVICE_DEBUG;
        }
#endif

        D3D_FEATURE_LEVEL featureLevels[] = 
        {
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };

        winrt::com_ptr<ID3D11Device> dev;
        winrt::com_ptr<ID3D11DeviceContext> ctx;

        const auto hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            creation_flags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            dev.put(),
            &_d3dfeaturelevel,
            ctx.put());

        if (FAILED(hr)) { // fall back to the WARP device
            throw_if_failed(
                D3D11CreateDevice(
                    nullptr,
                    D3D_DRIVER_TYPE_WARP,
                    0,
                    creation_flags,
                    featureLevels,
                    ARRAYSIZE(featureLevels),
                    D3D11_SDK_VERSION,
                    dev.put(),
                    &_d3dfeaturelevel,
                    ctx.put()));
        }
        _d3ddevice = dev.as<ID3D11Device3>();
        _d3dcontext = ctx.as<ID3D11DeviceContext4>();
    }

    void DeviceResources::_reset_wsdres() {
        // clear the previous context that is specific for window size resources
        ID3D11RenderTargetView* nullviews[] = { nullptr };
        _d3dcontext->OMSetRenderTargets(ARRAYSIZE(nullviews), nullviews, nullptr);
        _d3drtview = nullptr;
        _d3ddsview = nullptr;
        _d3dcontext->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);
        _update_rtsize();
        _d3drt_size = _output_size;

        if (_swapchain != nullptr) {
            // if the swap-chain already exists, resize it.
            const auto hr = _swapchain->ResizeBuffers(
                2, // double-buffered swap-chain
                std::lround(_d3drt_size.Width),
                std::lround(_d3drt_size.Height),
                DXGI_FORMAT_B8G8R8A8_UNORM,
                0);

            if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
                handle_device_lost();
                return;
            }
            else {
                dx::throw_if_failed(hr);
            }
        }
        else {
            // otherwise, create a new one using the same adapter as the existing d3d device.
            DXGI_SWAP_CHAIN_DESC1 swapchain_desc{ 0 };
            swapchain_desc.Width = std::lround(_d3drt_size.Width);
            swapchain_desc.Height = std::lround(_d3drt_size.Height);
            swapchain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
            swapchain_desc.Stereo = false;
            swapchain_desc.SampleDesc.Count = 1;
            swapchain_desc.SampleDesc.Quality = 0;
            swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapchain_desc.BufferCount = 2;
            swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
            swapchain_desc.Flags = 0;
            swapchain_desc.Scaling = DXGI_SCALING_STRETCH;
            swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

            const auto dxgi_dev = _d3ddevice.as<IDXGIDevice3>();

            winrt::com_ptr<IDXGIAdapter> dxgi_adapter;
            dx::throw_if_failed(dxgi_dev->GetAdapter(dxgi_adapter.put()));

            winrt::com_ptr<IDXGIFactory4> dxgi_factory;
            dx::throw_if_failed(dxgi_adapter->GetParent(IID_PPV_ARGS(dxgi_factory.put())));

            // When using XAML interop, the swap chain must be created for composition.
            winrt::com_ptr<IDXGISwapChain1> swapchain;
            dx::throw_if_failed(dxgi_factory->CreateSwapChainForComposition(
                _d3ddevice.get(),
                &swapchain_desc,
                nullptr,
                swapchain.put()));

            _swapchain = swapchain.as<IDXGISwapChain3>();

			// Associate swap chain with SwapChainPanel
            _swapchain_panel.Dispatcher().RunAsync(
                winrt::Windows::UI::Core::CoreDispatcherPriority::High,
                winrt::Windows::UI::Core::DispatchedHandler([=]() {
                    winrt::com_ptr<ISwapChainPanelNative> panel_native;
                    dx::throw_if_failed(winrt::get_unknown(_swapchain_panel)->QueryInterface(IID_PPV_ARGS(&panel_native)));
                    dx::throw_if_failed(panel_native->SetSwapChain(_swapchain.get()));
                    }));
            dx::throw_if_failed(dxgi_dev->SetMaximumFrameLatency(1));
        }

        // set up inverse scale on the swap chain
        DXGI_MATRIX_3X2_F inverse_scale{ 0 };
        inverse_scale._11 = 1.0f / _effective_composition_scale_x;
        inverse_scale._22 = 1.0f / _effective_composition_scale_y;
        dx::throw_if_failed(_swapchain->SetMatrixTransform(&inverse_scale));

        // creat a render target view of the swapchain back buffer
        winrt::com_ptr<ID3D11Texture2D1> back_buffer;
        dx::throw_if_failed(_swapchain->GetBuffer(0, IID_PPV_ARGS(back_buffer.put())));
        dx::throw_if_failed(_d3ddevice->CreateRenderTargetView1(
            back_buffer.get(),
            nullptr,
            _d3drtview.put()));

        // create a depth stencil view for 3D rendering purposes
        CD3D11_TEXTURE2D_DESC1 depthstencil_desc(
            DXGI_FORMAT_D24_UNORM_S8_UINT,
            std::lround(_d3drt_size.Width),
            std::lround(_d3drt_size.Height),
            1,
            1,
            D3D11_BIND_DEPTH_STENCIL);
        winrt::com_ptr<ID3D11Texture2D1> depthstencil;
        dx::throw_if_failed(_d3ddevice->CreateTexture2D1(
            &depthstencil_desc,
            nullptr,
            depthstencil.put()));
        CD3D11_DEPTH_STENCIL_VIEW_DESC depthstencilview_desc(D3D11_DSV_DIMENSION_TEXTURE2D);
        dx::throw_if_failed(_d3ddevice->CreateDepthStencilView(
            depthstencil.get(),
            &depthstencilview_desc,
            _d3ddsview.put()));

        // viewport
        _screenviewport = CD3D11_VIEWPORT(
            0.0f,
            0.0f,
            _d3drt_size.Width,
            _d3drt_size.Height);

        _d3dcontext->RSSetViewports(1, &_screenviewport);
    }

    void DeviceResources::_update_rtsize() {
        _effective_dpi = _dpi;
        _effective_composition_scale_x = _composition_scale_x;
        _effective_composition_scale_y = _composition_scale_y;

        // todo: to save better life, associate _effective** with their ideal factors using a mapping of linear relationships
        if (_dpi > _dpi_threshold) {
            const auto w = dx::convert_dips_to_pixels(_logical_size.Width, _dpi);
            const auto h = dx::convert_dips_to_pixels(_logical_size.Height, _dpi);
            if (std::max(w, h) > _width_threshold &&
                std::min(w, h) > _height_threshold) {
                _effective_dpi /= 2.0f;
                _effective_composition_scale_x /= 2.0f;
                _effective_composition_scale_y /= 2.0f;
            }
        }

        _output_size.Width = dx::convert_dips_to_pixels(_logical_size.Width, _effective_dpi);
        _output_size.Height = dx::convert_dips_to_pixels(_logical_size.Height, _effective_dpi);

        _output_size.Width = std::max(_output_size.Width, 1.f);
        _output_size.Height = std::max(_output_size.Height, 1.f);
    }
}
