//
// Created by igor on 8/21/24.
//

#ifndef NEUTRINO_MATH_DETAIL_TEMP_VALUE_HH
#define NEUTRINO_MATH_DETAIL_TEMP_VALUE_HH

#include <math/detail/vector_temp_value.hh>
#include <math/detail/matrix_temp_value.hh>

namespace neutrino::math::detail {
    template<typename T>
    constexpr bool is_basic_math_object = traits <std::decay_t <T>>::type == math_object_t::MATRIX
                                          || traits <std::decay_t <T>>::type == math_object_t::VECTOR;

    template<typename T, math_object_t Type>
    struct temp_value_traits;

    template<typename T>
    struct temp_value_traits<T, math_object_t::MATRIX> {
        using type = matrix_temp_value_holder<T>;
    };

    template<typename T>
    struct temp_value_traits<T, math_object_t::VECTOR> {
        using type = vector_temp_value_holder<T>;
    };

    template <typename T>
    using temp_value_t = typename temp_value_traits<std::decay_t<T>, traits<std::decay_t<T>>::type>::type;
}

#endif
