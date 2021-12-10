#include "pch.h"
#include "MasterViewer.h"
#if __has_include("MasterViewer.g.cpp")
#include "MasterViewer.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::raydbg::implementation
{
    MasterViewer::MasterViewer()
    {
        InitializeComponent();
        auto core_wnd = Window::Current().CoreWindow();
        core_wnd.VisibilityChanged([](auto, auto) {}); // TODO
        auto display_info = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
        display_info.DpiChanged([this](auto sender, auto) { _on_dpi_changed(sender); });
        winrt::Windows::Graphics::Display::DisplayInformation::DisplayContentsInvalidated([this](auto, auto) { _on_display_content_invalidated(); });
        swapchainpanel().CompositionScaleChanged([this](auto sender, auto) { _on_composition_scale_changed(sender); });
        swapchainpanel().SizeChanged([this](auto, auto args) { _on_swapchain_size_changed(args); });
        _devres.set_swapchainpanel(swapchainpanel());
    }

    void MasterViewer::_on_dpi_changed(winrt::Windows::Graphics::Display::DisplayInformation display_info) {
        _devres.set_dpi(display_info.LogicalDpi());
    }

    void MasterViewer::_on_swapchain_size_changed(winrt::Windows::UI::Xaml::SizeChangedEventArgs args) {
        _devres.set_logical_size(args.NewSize());
    }

    void MasterViewer::_on_display_content_invalidated() {
        _devres.validate_device();
    }

    void MasterViewer::_on_composition_scale_changed(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel) {
        _devres.set_composition_scale(panel.CompositionScaleX(), panel.CompositionScaleY());
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
