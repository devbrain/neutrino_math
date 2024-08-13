//
// Created by igor on 8/12/24.
//

#ifndef NEUTRINO_MATH_TRAITS_HH
#define NEUTRINO_MATH_TRAITS_HH

#include <type_traits>
#include <math/vector_fwd.hh>
#include <complex>

namespace neutrino::math {

    namespace detail {
        template <typename T>
        struct traits {
            static constexpr bool is_vector = false;
            static constexpr bool is_leaf = false;
        };

        template <typename E, std::size_t N>
        struct traits<vector<E, N>> {
            static constexpr bool is_vector = true;
            static constexpr bool is_leaf = true;
        };

        template<typename T>
        struct is_complex : std::false_type {
        };

        template<typename T>
        struct is_complex <std::complex <T>> : std::is_arithmetic <T> {
        };

        template<typename T>
        static constexpr bool is_complex_v = is_complex <T>::value;


    }

    template <typename T>
    constexpr bool is_vector_v = detail::traits<std::decay_t<T>>::is_vector;

    template<typename T>
    static constexpr bool is_scalar_v = std::is_arithmetic_v <T> || detail::is_complex <T>::value;

    template<typename T>
    static constexpr bool is_vector_expression_v = std::is_base_of_v<vector_expression_base, std::decay_t<T>>;

    template<typename T>
    static constexpr bool is_vector_or_vector_exp_v = is_vector_v<T> || is_vector_expression_v<T>;

    template<typename T, class = std::enable_if_t<is_vector_or_vector_exp_v<T>>>
    using element_type_t = std::decay_t<decltype(std::declval<std::decay_t<T>&>()[std::size_t{}])>;

}

#endif
