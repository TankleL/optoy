#ifndef OPTOY_RAY_H
#define OPTOY_RAY_H

#include "mathutils.hpp"

namespace optoy {
template<class TScalar>
struct ray_t {
    const vector4<TScalar> o; // origin
    const vector4<TScalar> d; // direction
    const TScalar          t_max;
};

using ray_f = ray_t<float>;
using ray_d = ray_t<double>;

template<class TScalar>
inline ray_t<TScalar> make_ray(const vector4<TScalar>& point_from, const vector4<TScalar>& point_to)
{}

template<class TScalar>
inline constexpr vector4<TScalar> on(const ray<TScalar>& r, TScalar t) {
    return r.o + r.d * t;
}

} // namespace optoy

#endif // !OPTOY_RAY_H
