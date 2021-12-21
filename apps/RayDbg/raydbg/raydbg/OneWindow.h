#pragma once
#include "pch.h"

namespace winrt::raydbg {
    class OneWindow {
    public:
        static inline winrt::Microsoft::UI::Xaml::Window WindowInstance{ 0 };
    };
}
