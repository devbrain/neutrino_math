//
// Created by igor on 8/20/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MAKE_VECTOR_HH
#define NEUTRINO_MATH_DETAIL_MAKE_VECTOR_HH

#include <neutrino/math/vector.hh>

namespace neutrino::math {
    template<typename... T>
    std::enable_if_t <is_all_same_v <T...> && std::conjunction_v <std::is_scalar <T>...>, vector <
                          first_type_t <T...>, sizeof...(T)>>
    make_vector(T&&... args) {
        return vector <first_type_t <T...>, sizeof...(T)>{std::forward <T>(args)...,};
    }

    template<typename T, std::size_t N, class = std::enable_if_t <is_scalar_v <T>>>
    auto make_vector(const std::array <T, N>& data) {
        return vector <T, N>(data);
    }

    template<typename T, std::size_t N>
    auto make_vector(T (&& data)[N]) {
        return vector <T, N>{std::forward <decltype(data)>(data)};
    }
}

#endif
