#include <gtest/gtest.h>
#include <optoy.hpp>

TEST(irresolver_tests, basic) {
    struct mock {
        int intersect(int a, int b) { return a + b; }
    };

    optoy::irresolver irr;
    auto r = irr.resolve(std::make_proxy<optoy::irtrace_target, mock>(), 1, 2);
    ASSERT_EQ(r, 3);
}
