#pragma once

#include "MainPage.g.h"
#include "MainPageViewModel.h"

namespace winrt::raydbg::implementation {
    struct MainPage : MainPageT<MainPage> {
        MainPage();
        raydbg::MainPageViewModel ViewModel() const { return _viewmodel; }

    private:
        raydbg::MainPageViewModel _viewmodel;
    };
}

namespace winrt::raydbg::factory_implementation {
    struct MainPage : MainPageT<MainPage, implementation::MainPage> {};
}
