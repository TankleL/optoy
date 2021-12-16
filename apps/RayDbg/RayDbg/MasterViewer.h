#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "MasterViewer.g.h"
#include "DeviceResources.h"
#include "GraphicsRenderThread.h"
#include "VisualLinePool.h"

namespace winrt::raydbg::implementation
{
    struct MasterViewer : MasterViewerT<MasterViewer>
    {
        MasterViewer();

        int32_t MyProperty();
        void MyProperty(int32_t value);

    private:
        void _on_dpi_changed(winrt::Windows::Graphics::Display::DisplayInformation display_info);
        void _on_swapchain_size_changed(winrt::Windows::UI::Xaml::SizeChangedEventArgs args);
        void _on_display_content_invalidated();
        void _on_composition_scale_changed(winrt::Windows::UI::Xaml::Controls::SwapChainPanel panel);
        void _on_loaded();
        void _on_unloaded();

    private:
        GraphicsRenderThread _graphics;
        VisualLinePool _linepool;
    };
}

namespace winrt::raydbg::factory_implementation
{
    struct MasterViewer : MasterViewerT<MasterViewer, implementation::MasterViewer>
    {
    };
}
