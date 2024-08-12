//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_VECTOR_FUNCTIONS_HH
#define NEUTRINO_MATH_VECTOR_FUNCTIONS_HH

#include <math/deprecated/detail/vector_expression.hh>

/*
*  Utility functions
*/

// Sumation of all elements
template<typename E1, std::size_t N>
constexpr auto sum(const neutrino::math::vector_expression <E1, N>& expr) noexcept {
    auto Sum = expr[0];
    for (std::size_t i = 1; i < N; ++i)
        Sum += expr[i];
    return Sum;
}

// Specialization for N = 3
template<typename E1>
constexpr auto sum(const neutrino::math::vector_expression <E1, 3>& expr) noexcept {
    return expr[0] + expr[1] + expr[2];
}

// Specialization for N = 2
template<typename E1>
constexpr auto sum(const neutrino::math::vector_expression <E1, 2>& expr) noexcept {
    return expr[0] + expr[1];
}


// Dot Product
template<typename E1, typename E2, std::size_t N>
constexpr auto dot(const neutrino::math::vector_expression <E1, N>& u, const neutrino::math::vector_expression <E2, N>& v) noexcept {
    return sum(neutrino::math::elem_prod(u, v));
}

template<typename E1, typename E2>
constexpr neutrino::math::vec_cross_product <E1, E2> cross(const neutrino::math::vector_expression <E1, 3>& u,
                                           const neutrino::math::vector_expression <E2, 3>& v) noexcept {
    return neutrino::math::vec_cross_product <E1, E2>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
}

template<typename E1, typename E2>
constexpr auto cross(const neutrino::math::vector_expression <E1, 2>& u, const neutrino::math::vector_expression <E2, 2>& v) noexcept {
    return u[0] * v[1] - u[1] * v[0];
}



// Square Norm
template<typename E1, std::size_t N>
constexpr auto norm2(const neutrino::math::vector_expression <E1, N>& expr) noexcept {
    return dot(expr, expr);
}

// Norm
template<typename E1, std::size_t N>
constexpr auto norm(const neutrino::math::vector_expression <E1, N>& expr) noexcept {
    return std::sqrt(norm2(expr));
}

// Angle between 2 vectors
template<typename E1, typename E2, std::size_t N>
constexpr auto angle(const neutrino::math::vector_expression <E1, N>& u, const neutrino::math::vector_expression <E2, N>& v) noexcept {
    return std::acos(dot(u, v) / (norm(u) * norm(v)));
}

template<typename T>
constexpr T radians_to_degrees(T degrees) {
    return degrees * static_cast <T>(180.0) * static_cast <T>(M_1_PI);
}

template<typename E1, typename E2, std::size_t N>
constexpr auto angled(const neutrino::math::vector_expression <E1, N>& u, const neutrino::math::vector_expression <E2, N>& v) noexcept {
    return radians_to_degrees(std::acos(dot(u, v) / (norm(u) * norm(v))));
}

#endif
