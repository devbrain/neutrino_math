//
// Created by igor on 8/11/24.
//
#include <doctest/doctest.h>
#include <math/math.hh>
//
// TEST_SUITE("Matrix expressions") {
//     TEST_CASE("from vector to row matrix") {
//         neutrino::math::vector_2d<int> v;
//         v.load(1, 2);
//         auto e = neutrino::math::as_row_matrix(v);
//         REQUIRE_EQ(e.rows(), 1);
//         REQUIRE_EQ(e.columns(), 2);
//         REQUIRE_EQ(e(0, 0), 1);
//         REQUIRE_EQ(e(0, 1), 2);
//     }
//     TEST_CASE("from vector to column matrix") {
//         neutrino::math::vector_2d<int> v;
//         v.load(1, 2);
//         auto e = neutrino::math::as_column_matrix(v);
//         REQUIRE_EQ(e.rows(), 2);
//         REQUIRE_EQ(e.columns(), 1);
//         REQUIRE_EQ(e(0, 0), 1);
//         REQUIRE_EQ(e(1, 0), 2);
//     }
//     TEST_CASE("test transpose") {
//         neutrino::math::vector_2d<int> v;
//         v.load(1, 2);
//         auto e = neutrino::math::transpose(neutrino::math::as_column_matrix(v));
//         REQUIRE_EQ(e.rows(), 1);
//         REQUIRE_EQ(e.columns(), 2);
//         REQUIRE_EQ(e(0, 0), 1);
//         REQUIRE_EQ(e(0, 1), 2);
//     }
// }
