#pragma once
#include <winrt/base.h>
#include "winrt/raydbg.ViewEntities.h"
#include "StringUtils.g.h"

namespace winrt::raydbg::implementation {
    struct StringUtils {
        static winrt::hstring GetString(ViewEntities::Vector const& vec);
        static winrt::hstring GetString(ViewEntities::Point const& pot);
        static winrt::hstring GetString(ViewEntities::Ray const& ray);
        static winrt::hstring GetString(float f);
    };
}

namespace winrt::raydbg::factory_implementation {
    struct StringUtils : StringUtilsT<StringUtils, static_lifetime> {
        static winrt::hstring GetString(ViewEntities::Vector const& vec);
        static winrt::hstring GetString(ViewEntities::Point const& pot);
        static winrt::hstring GetString(ViewEntities::Ray const& ray);
        static winrt::hstring GetString(float f);
    };
}
