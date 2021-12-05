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
        _devres.set_swapchainpanel(swapchainpanel());
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
