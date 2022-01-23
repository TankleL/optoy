#ifndef OPTOY_ICAMERA_HPP
#define OPTOY_ICAMERA_HPP

#include <stl-preview/proxy>
#include "core/ray.hpp"

namespace optoy::camera {

// x and y are within the range of [-0.5, +0.5]
template<class TScalar>
struct generate_ray_f : std::facade_operation<
    ray_t<TScalar>(TScalar, TScalar),
    [](const auto& self, auto x, auto y) { return self.generate_ray(x, y); }> {};

// x and y are within the range of [-0.5, +0.5]
template<class TScalar>
struct generate_ray_without_bias_f : std::facade_operation<
    ray_t<TScalar>(TScalar, TScalar),
    [](const auto& self, auto x, auto y) { return self.generate_ray_without_bias(x, y); }> {};

template<class TScalar>
struct interface_t : std::facade<
    generate_ray_f<TScalar>,
    generate_ray_without_bias_f<TScalar>>{};
} // namespace optoy

#endif // !OPTOY_ICAMERA_HPP
