#include "pch.h"
#include "DeviceResources.h"
#include "DirectXHelper.h"

namespace dx {
    DeviceResources::DeviceResources()
        : _screenViewport()
        , _d3dfeaturelevel(D3D_FEATURE_LEVEL_11_1) {
        _create_dires();
        _create_ddres();
    }

    void DeviceResources::set_swapchainpanel(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel) {
        auto dinfo = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
        _swapchain_panel = panel;
        _logical_size = winrt::Windows::Foundation::Size{
            static_cast<float>(panel.ActualWidth()),
            static_cast<float>(panel.ActualHeight())};
        _dpi = dinfo.LogicalDpi();
        
    }

    void DeviceResources::_create_dires() {
    }

    void DeviceResources::_create_ddres() {
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

    void DeviceResources::_ensure_wsdres() {
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
                // _handle_device_lost();
                return;
            }
            else {
                winrt::throw_hresult(hr);
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
            swapchain_desc.Scaling = DXGI_SCALING_NONE;
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

			// Associate swap chain with SwapChainPanel
            _swapchain_panel.Dispatcher().RunAsync(
                winrt::Windows::UI::Core::CoreDispatcherPriority::High,
                winrt::Windows::UI::Core::DispatchedHandler([=]() {
                    winrt::com_ptr<ISwapChainPanelNative> panel_native;
                    dx::throw_if_failed(winrt::get_unknown(_swapchain_panel)->QueryInterface(IID_PPV_ARGS(&panel_native)));
                    dx::throw_if_failed(panel_native->SetSwapChain(swapchain.get()));
                    }));
        }
    }

    void DeviceResources::_update_rtsize() {

    }
}
