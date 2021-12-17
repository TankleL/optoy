#pragma once
#include "MainPageViewModel.g.h"

namespace winrt::raydbg::implementation {
    struct MainPageViewModel : MainPageViewModelT<MainPageViewModel> {
        MainPageViewModel();

        winrt::Windows::Foundation::Collections::IObservableVector<ViewEntities::LineSegment> RayList() const;

    private:
        winrt::Windows::Foundation::Collections::IObservableVector<ViewEntities::LineSegment> _raylist;
    };
}

namespace winrt::raydbg::factory_implementation {
    struct MainPageViewModel : MainPageViewModelT<MainPageViewModel, implementation::MainPageViewModel> {
    };
}
