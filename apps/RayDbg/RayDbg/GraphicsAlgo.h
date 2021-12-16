#pragma once
#include <cmath>
#include <tuple>
#include "GraphicsEntities.h"

namespace winrt::raydbg::galgo {
    template<class V>
    V normalize(V v) {
        const auto& m = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
        return V{v.x/m, v.y/m, v.z/m};
    }

    template<class T>
    std::tuple<entities::point_t<T>, entities::point_t<T>>
    end_points(entities::line_segment_t<T> const& ls) {
        return std::make_tuple(
            entities::point_t{
                ls.o.x + ls.tmin * ls.d.x,
                ls.o.y + ls.tmin * ls.d.y,
                ls.o.x + ls.tmin * ls.d.z,
            },
            entities::point_t{
                ls.o.x + ls.tmax * ls.d.x,
                ls.o.y + ls.tmax * ls.d.y,
                ls.o.z + ls.tmax * ls.d.z,
            }
        );
    }
}
