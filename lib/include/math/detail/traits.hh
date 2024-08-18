//
// Created by igor on 8/12/24.
//

#ifndef NEUTRINO_MATH_TRAITS_HH
#define NEUTRINO_MATH_TRAITS_HH

#include <type_traits>
#include <complex>

#include <math/vector_fwd.hh>
#include <math/matrix_fwd.hh>



namespace neutrino::math {

    namespace detail {
        enum class math_object_t {
            VECTOR,
            MATRIX,
            UNKOWN
        };

        template <typename T>
        struct traits {
            static constexpr auto type = math_object_t::UNKOWN;
            static constexpr bool is_leaf = false;
        };

        template <typename E, std::size_t N>
        struct traits<vector<E, N>> {
            static constexpr auto type = math_object_t::VECTOR;
            static constexpr bool is_leaf = true;
            static constexpr auto size = N;
        };

        template <typename E, std::size_t R, std::size_t C>
        struct traits<matrix<E, R, C>> {
            static constexpr auto type = math_object_t::MATRIX;
            static constexpr bool is_leaf = true;
            static constexpr auto rows = R;
            static constexpr auto columns = C;
        };

        template<typename T>
        struct is_complex : std::false_type {
        };

        template<typename T>
        struct is_complex <std::complex <T>> : std::is_arithmetic <T> {
        };

        template<typename T>
        static constexpr bool is_complex_v = is_complex <T>::value;

        template <std::size_t R, std::size_t C, typename T>
        struct is_matrix_helper {
            static constexpr bool value = false;
        };

        template <std::size_t R, std::size_t C, typename T>
        struct is_matrix_helper <R, C, matrix<T, R, C>> {
            static constexpr bool value = true;
        };

    }

    struct op_matrix {};
    struct op_scalar {};
    struct op_vector {};

    template <typename T>
    constexpr bool is_vector_v = detail::traits<std::decay_t<T>>::type == detail::math_object_t::VECTOR;

    template <typename T, std::size_t R, std::size_t C>
    constexpr bool is_matrix_v = detail::is_matrix_helper<R, C, std::decay_t<T>>::value;

    template<typename T>
    static constexpr bool is_scalar_v = std::is_arithmetic_v <T> || detail::is_complex <T>::value;

    template<typename T>
    static constexpr bool is_vector_expression_v = std::is_base_of_v<vector_expression_base, std::decay_t<T>>;

    template<typename T, std::size_t R, std::size_t C>
    static constexpr bool is_matrix_expression_v = std::is_base_of_v<matrix_expression_base<R, C>, std::decay_t<T>>;

    template<typename T>
    static constexpr bool is_vector_or_vector_exp_v = is_vector_v<T> || is_vector_expression_v<T>;

    template<typename T, std::size_t R, std::size_t C>
    static constexpr bool is_matrix_or_matrix_exp_v = is_matrix_v<T, R, C> || is_matrix_expression_v<T, R, C>;

    template<typename T, class = std::enable_if_t<is_vector_or_vector_exp_v<T>>>
    using vector_element_type_t = std::decay_t<decltype(std::declval<std::decay_t<T>&>()[std::size_t{}])>;


    template<typename T>
        struct vector_size_traits {
        static constexpr size_t size() {
            if constexpr (is_vector_or_vector_exp_v <T>) {
                return std::decay_t<T>::size();
            } else {
                return 1;
            }
        }
    };

    template<typename T>
    static constexpr auto size_v = vector_size_traits<std::decay_t<T>>::size();

}

#endif
