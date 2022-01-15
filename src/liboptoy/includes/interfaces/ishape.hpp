#ifndef OPTOY_ISHAPE_HPP
#define OPTOY_ISHAPE_HPP

#include "predef.hpp"
#include "core/interaction.hpp"
#include "core/ray.hpp"

namespace optoy {
namespace op {
    template<class TScalar>
    struct intersect_f : std::facade_operation<
        bool(surface_interaction_t<TScalar>&, TScalar&, const ray_t<TScalar>&),
        [](const auto& self, surface_interaction_t<TScalar>& arg0, TScalar& arg1, const ray_t<TScalar>& arg2) {
            return self.intersect(arg0, arg1, arg2); }> {};
} // namespace iop

template<class TScalar>
struct shape_i : std::facade<op::intersect_f<TScalar>> {};

} // namespace optoy

#endif // OPTOY_ISHAPE_HPP
