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
using vector3 = vector<TScalar, 3>;
using vector3f = vector3<float>;
using vector3d = vector3<double>;

template<class TScalar>
using vector4 = vector<TScalar, 4>;
using vector4f = vector4<float>;
using vector4d = vector4<double>;

template<class TScalar>
inline constexpr TScalar pi() {
    return static_cast<TScalar>(3.1415926535897932384626433832795);
}

template<class TScalar>
inline constexpr TScalar dbl_pi() {
    return static_cast<TScalar>(6.283185307179586476925286766559);
}

template<class TScalar>
inline bool eq(TScalar lhs, TScalar rhs); // leave it undefined

template<>
inline bool eq<float>(float lhs, float rhs) {
    return std::abs(lhs - rhs) <= std::numeric_limits<float>::epsilon();
}

template<>
inline bool eq<double>(double lhs, double rhs) {
    return std::abs(lhs - rhs) <= std::numeric_limits<double>::epsilon();
}

template<class T, class U, class V>
inline T clamp(T val, U low, V high) {
    if (val < low) return low;
    else if (val > high) return high;
    else return val;
}

// solve_quadratic() finds solutions of the given quadratic equation ax^2 + bx + c = 0
template<class TScalar>
inline bool solve_quadratic(TScalar a, TScalar b, TScalar c, TScalar& x0, TScalar& x1) {
    double discrim = (double)b * (double)b - 4 * (double)a * (double)c;
    if (discrim < 0.0) return false;
    auto root_discrim = std::sqrt(discrim);
    double q;
    if (b < 0.0) q = -0.5 * (b - root_discrim);
    else         q = -0.5 * (b + root_discrim);
    x0 = q / a;
    x1 = c / q;
    if (x0 > x1) std::swap(x0, x1);
    return true;
}

template<class TVec4>
inline TVec4 normalize_4as3(TVec4&& src) {
    const auto len = std::sqrt(src(0) * src(0) + src(1) * src(1) + src(2) * src(2));
    return TVec4(src(0) / len, src(1) / len, src(2) / len, src(3));
}

} // namespace optoy

#endif // !OPTOY_MATHDEF_H
