#include "pch.h"
#include "StringUtils.h"
#if __has_include("StringUtils.g.cpp")
#   include "StringUtils.g.cpp"
#endif

namespace winrt::raydbg::implementation {
    winrt::hstring StringUtils::GetString(ViewEntities::Vector const& vec) {
        return winrt::hstring{ std::format(L"v({0}, {1}, {2}, {3})", vec.x, vec.y, vec.z, vec.w).c_str() };
    }

    winrt::hstring StringUtils::GetString(ViewEntities::Point const& pot) {
        return winrt::hstring{ std::format(L"p({0}, {1}, {2}, {3})", pot.x, pot.y, pot.z, pot.w).c_str() };
    }

    winrt::hstring StringUtils::GetString(ViewEntities::Ray const& ray) {
        return winrt::hstring{ std::format(L"o({0}), d({1})", GetString(ray.o).c_str(), GetString(ray.d).c_str()).c_str() };
    }

    winrt::hstring StringUtils::GetString(float f) {
        return winrt::hstring{ std::format(L"{0}", f).c_str()};
    }
}

namespace winrt::raydbg::factory_implementation {
    winrt::hstring StringUtils::GetString(ViewEntities::Vector const& vec) {
        return implementation::StringUtils::GetString(vec);
    }

    winrt::hstring StringUtils::GetString(ViewEntities::Point const& pot) {
        return implementation::StringUtils::GetString(pot);
    }

    winrt::hstring StringUtils::GetString(ViewEntities::Ray const& ray) {
        return implementation::StringUtils::GetString(ray);
    }

    winrt::hstring StringUtils::GetString(float f) {
        return implementation::StringUtils::GetString(f);
    }
}

