#ifndef OPTOY_SPHERE_H
#define OPTOY_SPHERE_H

#include "mathdef.hpp"

namespace optoy {
template<class TScalar>
struct sphere {
    const TScalar radius;
    const TScalar z_min, z_max;
    const TScalar theta_min, theta_max, phi_max;
};

using sphere_f = sphere<float>;
using sphere_d = sphere<double>;
} // namespace optoy

#endif // !OPTOY_SPHERE_H

