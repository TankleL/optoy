#include <gtest/gtest.h>
#include <optoy.hpp>

namespace ot = optoy;
namespace otg = optoy::geo;

TEST(intersec_test, sphere0) {
    ot::sphere_f unit_sphere{
        .radius = 1.0f,
        .z_min = 0.0f,
        .z_max = 1.0f,
        .theta_min = 0.0f,
        .theta_max = ot::pi<float>(),
        .phi_max = ot::dbl_pi<float>()
    };

    const auto n = ot::normalize_4as3(ot::vector4f(-10.0f, -10.0f, -10.0f, 0.0f));
    ot::ray_f ray {
        .o = ot::vector4f(10.0f, 10.0f, 10.0f, 1.0f),
        .d = n,
        .t_max = 99999.9f
    };
    auto res = otg::ds_f<ot::sphere_f>::intersects(unit_sphere, ray);
}

