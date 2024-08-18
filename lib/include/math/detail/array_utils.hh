//
// Created by igor on 8/15/24.
//

#ifndef NEUTRINO_MATH_ARRAY_UTILS_HH
#define NEUTRINO_MATH_ARRAY_UTILS_HH

#include <array>

namespace neutrino::math::detail {
    template<size_t, class T>
    constexpr T&& identity(T&& x) { return std::forward <T>(x); }

    template<typename T, size_t ... Indices>
    std::array <T, sizeof...(Indices)> make_array(T arg, std::index_sequence <Indices...>) {
        return {identity <Indices>(arg)...,};
    }

    template<size_t idx, class T>
    constexpr auto take(const std::initializer_list <T>& x) { return *(x.begin() + idx); }

    template<typename T, size_t ... Indices>
    constexpr std::array <T, sizeof...(Indices)> fill_array(const std::initializer_list <T>& arg,
                                                  std::index_sequence <Indices...>) {
        return {take <Indices>(arg)...,};
    }

    template<typename... T>
    constexpr std::array <std::tuple_element_t <0, std::tuple <T...>>, sizeof...(T)> copy_array(T... args) {
        return {args...,};
    }
}

#endif
