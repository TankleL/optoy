#ifndef OPTOY_ISHAPE_HPP
#define OPTOY_ISHAPE_HPP

#include "predef.hpp"
#include "core/interaction.hpp"
#include "core/ray.hpp"

namespace optoy {
namespace shape {
    template<class TScalar>
    struct intersect_f : std::facade_expr<
        bool(surface_interaction_t<TScalar>&, TScalar&, const ray_t<TScalar>&),
        [](const auto& self, surface_interaction_t<TScalar>& arg0, TScalar& arg1, const ray_t<TScalar>& arg2) {
            return self.intersect(arg0, arg1, arg2); }> {};

    template<class TScalar>
    struct interface_t : std::facade<intersect_f<TScalar>> {};
} // namespace shape 
} // namespace optoy

#endif // OPTOY_ISHAPE_HPP
