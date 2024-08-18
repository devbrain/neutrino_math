//
// Created by igor on 8/18/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MATRIX_TRAITS_HH
#define NEUTRINO_MATH_DETAIL_MATRIX_TRAITS_HH

#include <math/detail/matrix_expr.hh>

namespace neutrino::math::detail {
    template <typename T>
    struct matrix_traits {
        static constexpr bool value = false;
        static constexpr std::size_t rows = is_scalar_v<T> ? 1 : 0;
        static constexpr std::size_t columns = is_scalar_v<T> ? 1 : 0;
    };

    template<class T,  std::size_t R, std::size_t C>
    struct matrix_traits<matrix<T, R, C>> {
        static constexpr bool value = true;
        static constexpr std::size_t rows = R;
        static constexpr std::size_t columns = C;
    };


    template<class callable, class LHS, class RHS, std::size_t R, std::size_t C>
    struct matrix_traits<binary_matrix_expression<callable, LHS, RHS, R, C>> {
        static constexpr bool value = true;
        static constexpr std::size_t rows = R;
        static constexpr std::size_t columns = C;
    };


    template<class callable, class LHS, std::size_t R, std::size_t C>
    struct matrix_traits<unary_matrix_expression<callable, LHS, R, C>> {
        static constexpr bool value = true;
        static constexpr std::size_t rows = R;
        static constexpr std::size_t columns = C;
    };



    template<class LHS, std::size_t R, std::size_t C>
    struct matrix_traits<vector_as_matrix_expression<LHS, R, C>> {
        static constexpr bool value = true;
        static constexpr std::size_t rows = R;
        static constexpr std::size_t columns = C;
    };


    template<class LHS, std::size_t R, std::size_t C>
    struct matrix_traits<transpose_matrix_expression<LHS, R, C>> {
        static constexpr bool value = true;
        static constexpr std::size_t rows = R;
        static constexpr std::size_t columns = C;
    };


    template<class T, std::size_t R, std::size_t C>
    struct matrix_traits<identity_matrix_expression<T, R, C>> {
        static constexpr bool value = true;
        static constexpr std::size_t rows = R;
        static constexpr std::size_t columns = C;
    };



    template<class A, class B, std::size_t R, std::size_t C, std::size_t LHS_COLS, std::size_t RHS_ROWS>
    struct matrix_traits<matrix_mul_expression<A, B, R, C, LHS_COLS, RHS_ROWS>> {
        static constexpr bool value = true;
        static constexpr std::size_t rows = R;
        static constexpr std::size_t columns = C;
    };


    template <typename T>
    using enable_if_matrix_t = std::enable_if_t<matrix_traits<std::decay_t<T>>::value, op_matrix>;

    template <typename T>
    constexpr auto matrix_rows_v = matrix_traits<std::decay_t<T>>::rows;

    template <typename T>
    constexpr auto matrix_columns_v = matrix_traits<std::decay_t<T>>::columns;

    template <typename A, typename B>
    using enable_if_same_size_matrix_t = std::enable_if_t<(matrix_traits<std::decay_t<A>>::value && matrix_traits<std::decay_t<B>>::value)
                                                           && (matrix_rows_v<A> == matrix_rows_v<B>)
                                                           && (matrix_columns_v<A> == matrix_columns_v<B>),
                                                           op_matrix>;
    template <typename T>
    using enable_if_square_t = std::enable_if_t<matrix_traits<std::decay_t<T>>::value && (matrix_rows_v<T> == matrix_columns_v<T>) , op_matrix>;


    template <typename A, typename B>
    using enable_if_can_mul_matrix_t = std::enable_if_t<(matrix_traits<std::decay_t<A>>::value && matrix_traits<std::decay_t<B>>::value)
                                                           && (matrix_columns_v<A> == matrix_rows_v<B>),
                                                           op_matrix>;


}

#endif
