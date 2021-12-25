#include "pch.h"
#include "MasterViewer.xaml.h"
#if __has_include("MasterViewer.g.cpp")
#include "MasterViewer.g.cpp"
#endif
#include "OneWindow.h"

namespace winrt::raydbg::implementation {
    MasterViewer::MasterViewer() {
        InitializeComponent();
        swapchainpanel().CompositionScaleChanged([this](auto sender, auto) { _on_composition_scale_changed(sender); });
        swapchainpanel().SizeChanged([this](auto, auto args) { _on_swapchain_size_changed(args); });
        Loaded([this](auto, auto) { _on_loaded(); });
        Unloaded([this](auto, auto) { _on_unloaded(); });

        auto& dxres = _graphics.get_device_resources();
        dxres.device_reset([this]() { _linepool.device_reset(_graphics.get_device_resources()); });
        _graphics.drawing([this]() {
            _linepool.draw(_graphics.get_device_resources());
        });
    }

    void MasterViewer::SetLines(winrt::array_view<const ViewEntities::LineSegment> const& lines) {
    }

    //void MasterViewer::_on_dpi_changed(winrt::Windows::Graphics::Display::DisplayInformation display_info) {
    //    _graphics.dispatch([dpi = display_info.LogicalDpi()](auto& g) { g.get_device_resources().set_dpi(dpi); });
    //}

    void MasterViewer::_on_swapchain_size_changed(winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& args) {
        _graphics.dispatch([size = args.NewSize()](auto& g){ g.get_device_resources().set_logical_size(size); });
    }

    //void MasterViewer::_on_display_content_invalidated() {
    //    _graphics.dispatch([](auto& g) { g.get_device_resources().validate_device(); });
    //}

    void MasterViewer::_on_composition_scale_changed(winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel const& panel) {
        _graphics.dispatch([x = panel.CompositionScaleX(), y = panel.CompositionScaleY()](auto& g) {
            g.get_device_resources().set_composition_scale(x, y);
        });
    }
    
    void MasterViewer::_on_loaded() {
        _graphics.get_device_resources().set_swapchainpanel(swapchainpanel());
        _graphics.dispatch([this](auto& g) {
            _linepool.device_reset(g.get_device_resources()); // ensure the linepool has a correct device context.
        });
        _graphics.run_async();
    }

    void MasterViewer::_on_unloaded() {
        _graphics.request_stop([]() {});
    }
}
