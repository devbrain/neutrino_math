//
// Created by igor on 8/18/24.
//
#include <doctest/doctest.h>
#include <math/math.hh>

using namespace neutrino::math;

TEST_SUITE("test linear algebra") {
    TEST_CASE("test projection") {
        vector<float, 2> x {-6, 4};
        vector<float, 2> u {3, 2};

        vector<float, 2> p = proj(u, x);

        constexpr float a = -30.0f/13;
        constexpr float b = -20.0f/13;

        REQUIRE(p.x == doctest::Approx(a));
        REQUIRE(p.y == doctest::Approx(b));
    }

    TEST_CASE("test ortho") {
        auto m = make_matrix({
            {1.0, 0.0, 0.0},
            {1.0, 1.0, 0.0},
            {1.0, 1.0, 1.0},
            {1.0, 1.0, 1.0}
        });
        auto n = orthonormalize(m);
        const double r1 = 0.5;
        const double r2 = 1.0 / (2*sqrt(3.0));
        const double r3 = 1.0 / (sqrt(6.0));

        auto m2 = make_matrix({
            {r1, -3*r2, 0.0},
            {r1, r2, -2*r3},
            {r1, r2, r3},
            {r1, r2, r3}
        });

        REQUIRE_EQ(n, m2);
    }
}