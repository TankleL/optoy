#include <gtest/gtest.h>
#include <optoy.hpp>

namespace ot = optoy;
namespace otg = optoy::geo;

TEST(intersec_test, sphere0) {
    ot::sphere_f shape;
    ot::ray_f ray;

    auto res = otg::ds_f<decltype(shape)>::intersects(shape, ray);
}
