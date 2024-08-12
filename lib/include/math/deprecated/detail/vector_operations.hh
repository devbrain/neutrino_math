//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_VECTOR_OPERATIONS_HH
#define NEUTRINO_MATH_VECTOR_OPERATIONS_HH

#include <math/deprecated/detail/vector_expression.hh>

template<typename E1, typename E2, std::size_t N>
constexpr neutrino::math::vec_sum <E1, E2, N> operator+(const neutrino::math::vector_expression <E1, N>& u,
                                        const neutrino::math::vector_expression <E2, N>& v) noexcept {
    return neutrino::math::vec_sum <E1, E2, N>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
}


template<typename E1, std::size_t N>
constexpr neutrino::math::left_vec_sum <E1, N> operator+(const neutrino::math::vector_expression <E1, N>& u) noexcept {
    return neutrino::math::left_vec_sum <E1, N>(*static_cast <const E1*>(&u));
}

template<typename E1, typename E2, std::size_t N>
constexpr neutrino::math::vec_subtraction <E1, E2, N> operator-(neutrino::math::vector_expression <E1, N> const& u,
                                                neutrino::math::vector_expression <E2, N> const& v) noexcept {
    return neutrino::math::vec_subtraction <E1, E2, N>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
}

template<typename E1, std::size_t N>
constexpr neutrino::math::left_vec_subtraction <E1, N> operator-(const neutrino::math::vector_expression <E1, N>& u) noexcept {
    return neutrino::math::left_vec_subtraction <E1, N>(*static_cast <const E1*>(&u));
}

// Dot product
template<typename E1, typename E2, std::size_t N>
constexpr auto operator*(const neutrino::math::vector_expression <E1, N>& u, const neutrino::math::vector_expression <E2, N>& v) noexcept {
    return dot(u, v);
}

// Cross product
template<typename E1, typename E2, std::size_t N>
constexpr auto operator^(const neutrino::math::vector_expression <E1, N>& u, const neutrino::math::vector_expression <E2, N>& v) noexcept {
    return cross(u, v);
}

template<typename E1, typename  E2, std::size_t N>
constexpr std::enable_if_t<neutrino::math::is_number_v<E2>, neutrino::math::left_scalar_product <E1, E2, N>> operator *(const E2& v, const neutrino::math::vector_expression <E1, N>& u) noexcept {
    static_assert(neutrino::math::is_number_v<E2>);
    return neutrino::math::left_scalar_product <E1, E2, N>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
}

template<typename E1, typename E2, std::size_t N>
constexpr std::enable_if_t<neutrino::math::is_number_v<E2>, neutrino::math::right_vec_scalar_product <E1, E2, N>> operator *(const neutrino::math::vector_expression <E1, N>& u, const E2& v) noexcept {
    return neutrino::math::right_vec_scalar_product <E1, E2, N>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
}

template<typename E1, typename E2, std::size_t N>
constexpr neutrino::math::vec_element_wise_division <E1, E2, N> operator/(neutrino::math::vector_expression <E1, N> const& u,
                                                                neutrino::math::vector_expression <E2, N> const& v) noexcept {
    return neutrino::math::vec_element_wise_division <E1, E2, N>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
}

template<typename E1, typename E2, std::size_t N>
constexpr std::enable_if_t<neutrino::math::is_number_v<E2>, neutrino::math::vec_scalar_division <E1, E2, N>> operator/(const neutrino::math::vector_expression <E1, N>& u, const E2& v) noexcept {
    return neutrino::math::vec_scalar_division <E1, E2, N>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
}

template<typename E1, std::size_t N>
constexpr auto unit(const neutrino::math::vector_expression <E1, N>& u) noexcept {
    return neutrino::math::vec_normalization <E1, decltype(norm(u)), N>(*static_cast <const E1*>(&u));
}


#endif
