#include "pch.h"
#include "MainPage.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

namespace winrt::raydbg::implementation {
    MainPage::MainPage()
        : _viewmodel(winrt::make<MainPageViewModel>()) {
        InitializeComponent();
    }
}
