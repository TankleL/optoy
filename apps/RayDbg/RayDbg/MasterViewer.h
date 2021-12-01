#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "MasterViewer.g.h"

namespace winrt::raydbg::implementation
{
    struct MasterViewer : MasterViewerT<MasterViewer>
    {
        MasterViewer();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::raydbg::factory_implementation
{
    struct MasterViewer : MasterViewerT<MasterViewer, implementation::MasterViewer>
    {
    };
}
