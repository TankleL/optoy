#pragma once

#include "winrt/Microsoft.UI.Xaml.h"
#include "winrt/Microsoft.UI.Xaml.Markup.h"
#include "winrt/Microsoft.UI.Xaml.Controls.Primitives.h"
#include "MasterViewer.g.h"
#include "GraphicsRenderThread.h"
#include "VisualLinePool.h"

namespace winrt::raydbg::implementation {
    struct MasterViewer : MasterViewerT<MasterViewer> {
        MasterViewer();
        void SetLines(winrt::array_view<const ViewEntities::LineSegment> const& lines);

    private:
        //void _on_dpi_changed(winrt::Windows::Graphics::Display::DisplayInformation display_info);
        void _on_swapchain_size_changed(winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& args);
        //void _on_display_content_invalidated();
        void _on_composition_scale_changed(winrt::Microsoft::UI::Xaml::Controls::SwapChainPanel const& panel);
        void _on_loaded();
        void _on_unloaded();

    private:
        GraphicsRenderThread _graphics;
        VisualLinePool _linepool;
    };
}

namespace winrt::raydbg::factory_implementation {
    struct MasterViewer : MasterViewerT<MasterViewer, implementation::MasterViewer> {
    };
}
