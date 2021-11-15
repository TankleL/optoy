#ifndef OPTOY_MATHDEF_H
#define OPTOY_MATHDEF_H

#include <optional>
#include <Eigen/Dense>

namespace optoy {
template<class TScalar, int TRows>
using vector = Eigen::Matrix<TScalar, TRows, 1>;

template<class TScalar, int TColumns>
using row_vector = Eigen::Matrix<TScalar, 1, TColumns>;

template<class TScalar>
using vector4d = vector<TScalar, 4>;

} // namespace optoy

#endif // !OPTOY_MATHDEF_H
