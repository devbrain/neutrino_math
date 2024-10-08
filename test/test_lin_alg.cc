//
// Created by igor on 8/18/24.
//
#include <doctest/doctest.h>
#include <neutrino/math/math.hh>

using namespace neutrino::math;

TEST_SUITE("test linear algebra") {
    TEST_CASE("test projection") {
        vector <float, 2> x{-6, 4};
        vector <float, 2> u{3, 2};

        vector <float, 2> p = proj(u, x);

        constexpr float a = -30.0f / 13;
        constexpr float b = -20.0f / 13;

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
        const double r2 = 1.0 / (2 * sqrt(3.0));
        const double r3 = 1.0 / (sqrt(6.0));

        auto m2 = make_matrix({
            {r1, -3 * r2, 0.0},
            {r1, r2, -2 * r3},
            {r1, r2, r3},
            {r1, r2, r3}
        });

        REQUIRE_EQ(n, m2);
    }

    TEST_CASE("test qr_decomposition") {
        auto m = make_matrix({
            {12.0, -51.0, 4.0},
            {6.0, 167.0, -68.0},
            {-4.0, 24.0, -41.0}
        });
        auto [q, r] = qr_decompose(m);
        REQUIRE((m == dot(q, r)));
    }

    TEST_CASE("test determinant") {
        auto m1 = make_matrix({
            {5.0, 1.0},
            {-1.0, 5.0}
        });
        REQUIRE_EQ(abs_determinant(m1), 26);

        auto m2 = make_matrix({
            {1.0, 3.0, 2.0},
            {-3.0, -1.0, -3.0},
            {2.0, 3.0, 1.0}
        });
        REQUIRE_EQ(doctest::Approx(abs_determinant(m2)), 15);
    }
}
