#include "pch.h"
#include "MasterViewer.h"
#if __has_include("MasterViewer.g.cpp")
#include "MasterViewer.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::raydbg::implementation
{
    MasterViewer::MasterViewer() {
        InitializeComponent();
        auto core_wnd = Window::Current().CoreWindow();
        core_wnd.VisibilityChanged([](auto, auto) {}); // TODO
        auto display_info = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
        display_info.DpiChanged([this](auto sender, auto) { _on_dpi_changed(sender); });
        winrt::Windows::Graphics::Display::DisplayInformation::DisplayContentsInvalidated([this](auto, auto) { _on_display_content_invalidated(); });
        swapchainpanel().CompositionScaleChanged([this](auto sender, auto) { _on_composition_scale_changed(sender); });
        swapchainpanel().SizeChanged([this](auto, auto args) { _on_swapchain_size_changed(args); });
        Loaded([this](auto, auto) { _on_loaded(); });
        Unloaded([this](auto, auto) { _on_unloaded(); });
        auto& dxres = _graphics.get_device_resources();
        dxres.set_swapchainpanel(swapchainpanel());
        dxres.device_reset([this]() { _linepool.device_reset(_graphics.get_device_resources()); });
        _graphics.drawing([this]() {
            _linepool.draw(_graphics.get_device_resources());
        });
    }

    void MasterViewer::_on_dpi_changed(winrt::Windows::Graphics::Display::DisplayInformation display_info) {
        _graphics.dispatch([dpi = display_info.LogicalDpi()](auto& g) { g.get_device_resources().set_dpi(dpi); });
    }

    void MasterViewer::_on_swapchain_size_changed(winrt::Windows::UI::Xaml::SizeChangedEventArgs args) {
        _graphics.dispatch([size = args.NewSize()](auto& g){ g.get_device_resources().set_logical_size(size); });
    }

    void MasterViewer::_on_display_content_invalidated() {
        _graphics.dispatch([](auto& g) { g.get_device_resources().validate_device(); });
    }

    void MasterViewer::_on_composition_scale_changed(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel) {
        _graphics.dispatch([x = panel.CompositionScaleX(), y = panel.CompositionScaleY()](auto& g) {
            g.get_device_resources().set_composition_scale(x, y);
        });
    }
    
    void MasterViewer::_on_loaded() {
        _graphics.dispatch([this](auto& g) {
            _linepool.device_reset(g.get_device_resources()); // ensure the linepool has a correct device context.
        });
        _graphics.run_async();
    }

    void MasterViewer::_on_unloaded() {
        _graphics.request_stop([]() {});
    }

    int32_t MasterViewer::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MasterViewer::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
