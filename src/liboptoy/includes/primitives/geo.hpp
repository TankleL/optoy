#ifndef OPTOY_GEO_H
#define OPTOY_GEO_H

#include <optional>

namespace optoy {
// forward decls list:
template<class TScalar> struct intersec;
template<class TScalar> struct ray;
} // namespace optoy

namespace optoy::geo {
template<class TScalar, class TShape>
struct deduce_scalar {
    static inline std::optional<intersec<TScalar>> intersects(TShape const& shape, ray<TScalar> const& ray);
};

template<class TScalar, class TShape>
using ds = deduce_scalar<TScalar, TShape>;

template<class TShape>
using ds_f = deduce_scalar<float, TShape>;
template<class TShape>
using ds_d = deduce_scalar<double, TShape>;

} // namespace optoy::geo

#endif // !OPTOY_GEO_H
