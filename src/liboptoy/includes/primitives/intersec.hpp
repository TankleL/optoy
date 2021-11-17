#ifndef OPTOY_INTERFACE_INTERSEC_H
#define OPTOY_INTERFACE_INTERSEC_H

#include "mathdef.hpp"
#include "geo.hpp"
#include "ray.hpp"
#include "sphere.hpp"

namespace optoy {
template<class TScalar>
struct intersec{
    vector4<TScalar> hitpoint;
    vector4<TScalar> normal;
}; // class intersec

namespace geo{
template<class TScalar>
struct deduce_scalar<TScalar, sphere<TScalar>> {
    static inline std::optional<intersec<TScalar>> intersects(sphere<TScalar> const& shape, ray<TScalar> const& ray_in) {
        TScalar phi;
        vector4<TScalar> p_hit;
        // <<Transform Ray to object space>> 
        //  target equation: x^2 + y^2 + z^2 - r^2 = 0
        //  use the ray: (ox + t * dx)^2 + (oy + t * dy)^2 + (oz + t * dz)^2 - r^2  = 0
        ray<TScalar> r = ray_in;

        // <<Compute quadratic sphere coefficients>> 
        TScalar ox(r.o(0)), oy(r.o(1)), oz(r.o(2));
        TScalar dx(r.d(0)), dy(r.d(1)), dz(r.d(2));
        const auto rd = shape.radius;

        auto a = dx * dx + dy * dy + dz * dz;
        auto b = 2 * (dx * ox + dy * oy + dz * oz);
        auto c = ox * ox + oy * oy + oz * oz - rd * rd;

        // <<Solve quadratic equation for t values>> 
        TScalar t0, t1;
        if (!solve_quadratic(a, b, c, t0, t1)) return std::nullopt;
        if (t0 > r.t_max || t1 <= 0) return std::nullopt;
        TScalar t_hit = t0;
        if (t_hit <= 0) {
            t_hit = t1;
            if (t_hit > r.t_max) return std::nullopt;
        }

        // <<Compute sphere hit position and phi>> 
        p_hit = on(r, t_hit); // TODO: refine the p_hit
        phi = std::atan2(p_hit(1), p_hit(0));
        if (phi < 0.0) phi += 2 * pi<TScalar>();

        // <<Test sphere intersection against clipping parameters>> 
        if ((shape.z_min > -rd && p_hit(2) < shape.z_min) ||
            (shape.z_max <  rd && p_hit(2) > shape.z_max) || phi > shape.phi_max) {
            if (eq(t_hit, t1)) return std::nullopt;
            if (t1 > r.t_max) return std::nullopt;
            t_hit = t1;

            p_hit = on(r, t_hit); // TODO: refine the p_hit
            phi = std::atan2(p_hit(1), p_hit(0));
            if (phi < 0.0) phi += 2 * pi<TScalar>();

            if ((shape.z_min > -rd && p_hit(2) < shape.z_min) ||
                (shape.z_max <  rd && p_hit(2) > shape.z_max) || phi > shape.phi_max) {
                return std::nullopt;
            }
        }

        // <<Find parametric representation of sphere hit>> 
        auto u = phi / shape.phi_max;
        auto theta = std::acos(clamp(p_hit(2) / rd, (TScalar)-1, (TScalar)-1));
        auto v = (theta - shape.theta_min) / (shape.theta_max - shape.theta_min);

            // <<Compute sphere dp/du and dp/dv>>
        auto z_rd = std::sqrt(p_hit(0) * p_hit(0) + p_hit(1) * p_hit(1));
        auto inv_z_rd = 1 / z_rd;
        auto cos_phi = p_hit(0) * inv_z_rd;
        auto sin_phi = p_hit(1) * inv_z_rd;
        vector3<TScalar> dpdu(-shape.phi_max * p_hit(1), shape.phi_max * p_hit(0), 0);
        vector3<TScalar> dpdv = (shape.theta_max - shape.theta_min) *
            vector3<TScalar>(p_hit(2) * cos_phi, p_hit(2) * sin_phi, -rd * std::sin(theta));

            // <<Compute dn/du and dn/dv>>
        vector3<TScalar> d2Pduu = -shape.phi_max * shape.phi_max * vector3<TScalar>(p_hit(0), p_hit(1), 0);
        vector3<TScalar> d2Pduv = (shape.theta_max - shape.theta_min) * p_hit(2) * shape.phi_max *
            vector3<TScalar>(-sin_phi, cos_phi, 0);
        vector3<TScalar> d2Pdvv = -(shape.theta_max - shape.theta_min) * (shape.theta_max - shape.theta_min) *
            vector3<TScalar>(p_hit(0), p_hit(1), p_hit(2));

                // <<Compute coefficients for fundamental forms>>
        auto E = dpdu.dot(dpdu);
        auto F = dpdu.dot(dpdv);
        auto G = dpdv.dot(dpdv);
        auto N = dpdu.cross(dpdv);  N.normalize();
        auto e = N.dot(d2Pduu);
        auto f = N.dot(d2Pduv);
        auto g = N.dot(d2Pdvv);

                // <<Compute dn/du and dn/dv from fundamental form coefficients>>
        auto invEGF2 = (TScalar)1 / (E * G - E * F);
        vector3<TScalar> dndu((f * F - e * G) * invEGF2 * dpdu +
                              (e * F - f * E) * invEGF2 * dpdv);
        vector3<TScalar> dndv((g * F - f * G) * invEGF2 * dpdu +
                              (f * F - g * E) * invEGF2 * dpdv);

        // <<Compute error bounds for sphere intersection>> 
        // TODO:

        // <<Initialize SurfaceInteraction from parametric information>> 
        auto n = dpdu.cross(dpdv); n.normalize();
        auto res = std::make_optional(intersec<TScalar>{
                .hitpoint = p_hit,
                .normal = vector4<TScalar>(n(0), n(1), n(2), 0)});

        // <<Update tHit for quadric intersection>> 
        // TODO:

        return res;
    }
};
} // namespace geo 

} // namespace optoy

#endif // !OPTOY_INTERFACE_INTERSEC_H


