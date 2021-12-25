#pragma once
#include "MainPageViewModel.g.h"

namespace winrt::raydbg::implementation {
    struct MainPageViewModel : MainPageViewModelT<MainPageViewModel> {
        using raylist_updated_handler_t = winrt::Windows::Foundation::EventHandler<IInspectable>;

        MainPageViewModel();
        winrt::Windows::Foundation::Collections::IObservableVector<ViewEntities::LineSegment> RayList() const;
        winrt::event_token RayListUpdated(raylist_updated_handler_t const& handler) { return _raylist_updated.add(handler); }
        void RayListUpdated(winrt::event_token const& token) { _raylist_updated.remove(token); }

    private:
        winrt::Windows::Foundation::Collections::IObservableVector<ViewEntities::LineSegment> _raylist;
        winrt::event<raylist_updated_handler_t> _raylist_updated;
    };
}

namespace winrt::raydbg::factory_implementation {
    struct MainPageViewModel : MainPageViewModelT<MainPageViewModel, implementation::MainPageViewModel> {
    };
}
