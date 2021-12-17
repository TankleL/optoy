#pragma once
#include "StringUtils.g.h"

namespace winrt::raydbg::implementation {
    struct StringUtils {
        static winrt::hstring GetString(ViewEntities::Vector vec);
        static winrt::hstring GetString(ViewEntities::Point pot);
        static winrt::hstring GetString(ViewEntities::Ray ray);
    };
}

namespace winrt::raydbg::factory_implementation {
    struct StringUtils : StringUtilsT<StringUtils, implementation::StringUtils> {
    };
}
