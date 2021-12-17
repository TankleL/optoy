#pragma once

#include "StringUtils.h"
#include "MainPage.g.h"
#include "MainPageViewModel.h"

namespace winrt::raydbg::implementation {
    struct MainPage : MainPageT<MainPage> {
        MainPage();
        raydbg::MainPageViewModel ViewModel() const;

    private:
        raydbg::MainPageViewModel _viewmodel;
    };
}

namespace winrt::raydbg::factory_implementation {
    struct MainPage : MainPageT<MainPage, implementation::MainPage> {
    };
}
