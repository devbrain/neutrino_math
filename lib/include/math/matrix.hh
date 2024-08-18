//
// Created by igor on 8/14/24.
//

#ifndef NEUTRINO_MATH_MATRIX_HH
#define NEUTRINO_MATH_MATRIX_HH

#include <array>
#include <math/vector.hh>
#include "math/detail/matrix_ops.hh"

namespace neutrino::math {
    namespace detail {
        template<typename E, std::size_t R, std::size_t C>
        struct matrix_storage {
            using row_t = vector <E, C>;
            using values_t = std::array <row_t, R>;

        };
    }

    template<typename E, std::size_t R, std::size_t C>
    class matrix : public matrix_ops <E, R, C> {
        public:
            using row_t = typename detail::matrix_storage<E, R, C>::row_t;
            typename detail::matrix_storage<E, R, C>::values_t values;
        public:
            matrix() = default;

            template<typename... T, class = std::enable_if_t <sizeof...(T) == R>>
            explicit constexpr matrix(const std::initializer_list <T>&... args)
                : values{row_t(args)...} {
            }

            auto operator()(std::size_t r, std::size_t c) const {
                return values[r][c];
            }

            template<typename T, class = std::enable_if_t <is_matrix_or_matrix_exp_v <T, R, C>>>
            constexpr matrix(const T& v) {
                for (std::size_t r = 0; r < R; r++) {
                    for (std::size_t c = 0; c < C; c++) {
                        values[r][c] = v(r, c);
                    }
                }
            }

            template<typename T, class = std::enable_if_t <
                         is_matrix_or_matrix_exp_v <T, R, C>>>
            constexpr matrix& operator =(const T& v) {
                if ((void*)&v != (void*)this) {
                    for (std::size_t r = 0; r < R; r++) {
                        for (std::size_t c = 0; c < C; c++) {
                            values[r][c] = v(r, c);
                        }
                    }
                }
                return *this;
            }
    };
}

#endif
