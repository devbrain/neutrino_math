//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_MATRIX_EXPRESSION_HH
#define NEUTRINO_MATH_MATRIX_EXPRESSION_HH

#include <iostream>
#include <stdexcept>
#include <math/deprecated/vector.hh>
#include <math/deprecated/detail/value_holder.hh>

namespace neutrino::math {
    template<typename E, std::size_t Rows, std::size_t Columns>
    class matrix_expression {
        public:

            constexpr auto operator()(const std::size_t r, const std::size_t c) const {
                return static_cast <E const&>(*this)(r, c);
            }

            static constexpr std::size_t rows() {
                return Rows;
            }

            static constexpr std::size_t columns() {
                return Columns;
            }
    };

    template<typename E, std::size_t Rows>
    class vector_as_row_matrix : public matrix_expression <vector_as_row_matrix <E, Rows>, 1, Rows> {
        public:
            constexpr vector_as_row_matrix(const vector_expression <E, Rows>& vec)
                : v(vec) {
            }

            constexpr auto operator()(const std::size_t r, const std::size_t c) const {
                if (r != 0) {
                    throw std::out_of_range("vector_as_matrix: Index out of range");
                }
                return v[c];
            }

        private:
            const vector_expression <E, Rows>& v;
    };

    template<typename E, std::size_t Columns>
    class vector_as_col_matrix : public matrix_expression <vector_as_col_matrix <E, Columns>, Columns, 1> {
        public:
            constexpr vector_as_col_matrix(const vector_expression <E, Columns>& vec)
                : v(vec) {
            }

            constexpr auto operator()(const std::size_t r, const std::size_t c) const {
                if (c != 0) {
                    throw std::out_of_range("vector_as_matrix: Index out of range");
                }
                return v[r];
            }

        private:
            const vector_expression <E, Columns>& v;
    };

    template<typename E, std::size_t Rows, std::size_t Columns>
    class transpose_matrix_expression : public matrix_expression <
            transpose_matrix_expression <E, Rows, Columns>, Rows, Columns> {
        public:
            constexpr transpose_matrix_expression(const matrix_expression <E, Columns, Rows>& expr)
                : e(expr) {
            }

            constexpr auto operator()(const std::size_t r, const std::size_t c) const {
                return e(c, r);
            }

        private:
            const matrix_expression <E, Columns, Rows>& e;
    };

}

#endif
