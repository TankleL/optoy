#include "pch.h"
#include "StringUtils.h"
#if __has_include("StringUtils.g.cpp")
#   include "StringUtils.g.cpp"
#endif

namespace winrt::raydbg::implementation {
    winrt::hstring StringUtils::GetString(ViewEntities::Vector vec) {
        return winrt::hstring{ std::format(L"v({0}, {1}, {2}, {3})", vec.x, vec.y, vec.z, vec.w).c_str() };
    }

    winrt::hstring StringUtils::GetString(ViewEntities::Point pot) {
        return winrt::hstring{ std::format(L"p({0}, {1}, {2}, {3})", pot.x, pot.y, pot.z, pot.w).c_str() };
    }

    winrt::hstring StringUtils::GetString(ViewEntities::Ray ray) {
        return winrt::hstring{ std::format(L"o{0}, d{1}", GetString(ray.o).c_str(), GetString(ray.d).c_str()).c_str()};
    }
}

