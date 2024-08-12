//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_MATRIX_OPERATIONS_HH
#define NEUTRINO_MATH_MATRIX_OPERATIONS_HH

#include <math/deprecated/detail/matrix_expression.hh>

namespace neutrino::math {
    template <typename T, std::size_t N>
    constexpr vector_as_row_matrix<T, N> as_row_matrix(const vector_expression<T, N>& v) {
        return vector_as_row_matrix<T, N>(v);
    }

    template <typename T, std::size_t N>
    constexpr vector_as_col_matrix<T, N> as_column_matrix(const vector_expression<T, N>& v) {
        return vector_as_col_matrix<T, N>(v);
    }

    template<typename E, std::size_t Rows, std::size_t Columns>
    constexpr transpose_matrix_expression<E, Columns, Rows> transpose(const matrix_expression<E, Rows, Columns>& e) {
        return transpose_matrix_expression<E, Columns, Rows> (*static_cast <const E*>(&e));
    }



}

#endif
