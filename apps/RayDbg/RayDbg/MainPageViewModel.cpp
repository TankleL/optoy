#include "pch.h"
#include "MainPageViewModel.h"
#if __has_include("MainPageViewModel.g.cpp")
#   include "MainPageViewModel.g.cpp"
#endif

namespace winrt::raydbg::implementation
{
    MainPageViewModel::MainPageViewModel()
        : _raylist(winrt::single_threaded_observable_vector<ViewEntities::LineSegment>()) {
        _raylist.Append(ViewEntities::LineSegment{
            .ray = ViewEntities::Ray{
                .o {0.f, 0.f, 0.f, 1.f},
                .d {0.57735f, 0.57735f, 0.57735f, 0.f}},
            .tmin {0.f},
            .tmax {1.f}});
    }

    winrt::Windows::Foundation::Collections::IObservableVector<ViewEntities::LineSegment> MainPageViewModel::RayList() const {
        return _raylist;
    }
}
