//
// Created by igor on 8/20/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MATRIX_STORAGE_HH
#define NEUTRINO_MATH_DETAIL_MATRIX_STORAGE_HH

#include <neutrino/math/vector.hh>

namespace neutrino::math::detail {
    template<typename E, std::size_t R, std::size_t C>
    class matrix_storage {
        public:
            using row_t = vector <E, C>;
            using values_t = std::array <row_t, R>;

        private:
            template<int RowI, typename Expr, int... ColsI>
            constexpr static row_t expand_row_expr(const Expr& e, std::integer_sequence <int, ColsI...>) {
                return row_t{e(RowI, ColsI)...};
            }

        public:
            template<int... I>
            static constexpr values_t create(const E (& data)[R][C], std::integer_sequence <int, I...>) {
                return {row_t{data[I]}...};
            }

            template<typename Expr, int... RowI>
            static constexpr values_t create_from_expr(const Expr& e,
                                                       std::integer_sequence <int, RowI...>) {
                return {expand_row_expr <RowI>(e, std::make_integer_sequence<int, C>{})...};
            }
    };
}

#endif
