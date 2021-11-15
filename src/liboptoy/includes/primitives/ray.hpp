#ifndef OPTOY_RAY_H
#define OPTOY_RAY_H

#include "mathdef.hpp"

namespace optoy {
template<class TScalar>
struct ray {
    vector4d<TScalar> origin;
    vector4d<TScalar> direction;
};

using ray_f = ray<float>;
using ray_d = ray<double>;

template<class TScalar>
inline ray<TScalar> make_ray(const vector4d<TScalar>& point_from, const vector4d<TScalar>& point_to)
{}

} // namespace optoy

#endif // !OPTOY_RAY_H
