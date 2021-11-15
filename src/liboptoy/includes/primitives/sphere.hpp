#ifndef OPTOY_SPHERE_H
#define OPTOY_SPHERE_H

#include "mathdef.hpp"

namespace optoy {
template<class TScalar>
struct sphere {
    vector4d<TScalar> origin;
    TScalar radius;
};

using sphere_f = sphere<float>;
using sphere_d = sphere<double>;
} // namespace optoy

#endif // !OPTOY_SPHERE_H

