#ifndef OPTOY_INTERFACE_INTERSEC_H
#define OPTOY_INTERFACE_INTERSEC_H

#include "mathdef.hpp"
#include "geo.hpp"
#include "ray.hpp"
#include "sphere.hpp"

namespace optoy {
template<class TScalar>
struct intersec{
    vector4d<TScalar> hitpoint;
    vector4d<TScalar> normal;
}; // class intersec

namespace geo{
template<class TScalar>
struct deduce_scalar<TScalar, sphere<TScalar>> {
    static inline std::optional<intersec<TScalar>> intersects(sphere<TScalar> const& shape, ray<TScalar> const& ray) {
        auto res = std::nullopt;
        return res;
    }
};
} // namespace geo 

} // namespace optoy

#endif // !OPTOY_INTERFACE_INTERSEC_H


