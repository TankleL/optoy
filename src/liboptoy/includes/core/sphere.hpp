#ifndef OPTOY_SPHERE_H
#define OPTOY_SPHERE_H

#include "mathutils.hpp"
#include "interfaces/ishape.hpp"
#include "core/ray.hpp"

namespace optoy {
template<class TScalar>
class sphere_t {
public:
    const TScalar radius;
    const TScalar z_min, z_max;
    const TScalar theta_min, theta_max, phi_max;

public:
     bool intersect(surface_interaction_t<TScalar>& interaction, TScalar& hit_travel, const ray_t<TScalar>& ray) const;
};

template<class TScalar>
inline bool sphere_t<TScalar>::intersect(
    surface_interaction_t<TScalar>& interaction,
    TScalar& hit_travel,
    const ray_t<TScalar>& ray) const {
    // resolve a quadratic equation for variable *t* : (Ox + t * Dx)^2 + (Oy + t * Dy)^2 + (Oz + t * Dz)^2 - R^2  = 0
    TScalar Ox(ray.o(0)), Oy(ray.o(1)), Oz(ray.o(2));
    TScalar Dx(ray.d(0)), Dy(ray.d(1)), Dz(ray.d(2));

    const TScalar a = square(Dx) + square(Dy) + square(Dz);
    const TScalar b = 2.0 * (Dx * Ox + Dy * Oy + Dz * Oz);
    const TScalar c = square(Ox) + square(Oy) + square(Oz) - square(radius);

    TScalar t0, t1;
    if (!solve_quadratic(a, b, c, t0, t1)) return false; // no intersection.
    if (t0 > ray.t_max || t1 <= 0.0) return false; // scope exceeded or wrong direction.

    // find the closest t
    hit_travel = t0;
    if (hit_travel <= 0.0) {
        if (t1 > ray.t_max) {
            return false; // scope exceeded.
        }
        hit_travel = t1;
    }

    return true;
}

using sphere_f = sphere_t<float>;
using sphere_d = sphere_t<double>;
} // namespace optoy

#endif // !OPTOY_SPHERE_H

