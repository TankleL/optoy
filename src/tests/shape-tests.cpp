#include <gtest/gtest.h>
#include <optoy.hpp>

namespace ot = optoy;

TEST(ShapeTests, Sphere) {
    ot::sphere_f unit_sphere{
        .radius = 1.0f,
        .z_min = 0.0f,
        .z_max = 1.0f,
        .theta_min = 0.0f,
        .theta_max = ot::pi<float>(),
        .phi_max = ot::dbl_pi<float>()
    };

    ot::ray_f ray {
        .o = ot::vector4f(10.0f, 10.0f, 10.0f, 1.0f),
        .d = ot::normalize_xyz(ot::vector4f(-10.0f, -10.0f, -10.0f, 0.0f)),
        .t_max = 99999.9f
    };

    auto p = std::make_proxy<ot::shape_i<float>>(unit_sphere);
    ot::surface_interaction_t<float> si;
    float t;
    auto hit = p.invoke<ot::op::intersect_f<float>>(si, t, ray);
    ASSERT_TRUE(hit);
}
