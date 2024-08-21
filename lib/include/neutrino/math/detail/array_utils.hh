//
// Created by igor on 8/15/24.
//

#ifndef NEUTRINO_MATH_ARRAY_UTILS_HH
#define NEUTRINO_MATH_ARRAY_UTILS_HH

#include <array>
#include <neutrino/math/detail/traits.hh>

namespace neutrino::math::detail {
    template<size_t, class T>
    constexpr T&& identity(T&& x) { return std::forward <T>(x); }

    template<typename T, size_t ... Indices>
    constexpr std::array <T, sizeof...(Indices)> make_array(T arg, std::index_sequence <Indices...>) {
        return {identity <Indices>(arg)...,};
    }


    template<typename... T>
    constexpr std::array <first_type_t<T...>, sizeof...(T)> copy_array(T... args) {
        return {args...,};
    }

    template <typename T, std::size_t M,int... I>
    constexpr std::array<T, M> array_from_values(const T (&data)[M], std::integer_sequence<int, I...>) {
        return {data[I]...};
    }

    template <typename T, std::size_t M, typename E, int... I>
    constexpr std::array<T, M> array_from_expression(const E& expr, std::integer_sequence<int, I...>) {
        return {expr[I]...};
    }
}

#endif
