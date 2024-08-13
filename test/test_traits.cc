//
// Created by igor on 8/12/24.
//
#include <doctest/doctest.h>
#include <math/detail/traits.hh>
#include <math/vector.hh>
#include <math/detail/vector_expr.hh>
#include <vector>


template<class LHS, class RHS>
auto test_size([[maybe_unused]] LHS&& lhs, [[maybe_unused]] RHS&& rhs) {
    return neutrino::math::detail::is_compatible_size_v<LHS, RHS>;
}

TEST_SUITE("Test traits") {
    TEST_CASE("test vector traits") {
        using namespace neutrino::math;
        REQUIRE_FALSE(detail::traits<std::vector<int>>::is_vector);
        REQUIRE_FALSE(detail::traits<std::vector<int>>::is_leaf);

        REQUIRE(detail::traits<vector<int, 3>>::is_vector);
        REQUIRE(detail::traits<vector<int, 3>>::is_leaf);

        constexpr vector<int, 3> v0 = {10, 20, 30};
        constexpr vector<int, 2> v1 = {10, 20};
        constexpr vector<double, 3> v2 = {10, 20, 30};

        const auto v = neutrino::math::negate(v0);
        using uexpr_t = decltype(v);
        using vt = decltype(v0);
        REQUIRE(is_vector_expression_v<uexpr_t>);
        REQUIRE_EQ(3, size_v<uexpr_t>);
        REQUIRE(detail::is_compatible_size_v<vt, uexpr_t>);
        REQUIRE_FALSE(test_size(v1, 1));
        REQUIRE_FALSE(test_size(v1, v0));
        REQUIRE(test_size(v0, v2));

        static_assert(is_vector_or_vector_exp_v <vt>);
        static_assert(is_vector_or_vector_exp_v <uexpr_t>);
        static_assert(detail::is_compatible_size_v<vt, uexpr_t>);
        static_assert(!detail::is_compatible_size_v<vt, decltype(v1)>);
        static_assert(!detail::is_compatible_size_v<vt, int>);
    }

}