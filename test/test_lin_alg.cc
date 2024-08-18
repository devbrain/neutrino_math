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
}