//
// Created by igor on 8/14/24.
//

#ifndef NEUTRINO_MATH_MATRIX_HH
#define NEUTRINO_MATH_MATRIX_HH

#include <math/vector.hh>
#include <math/detail/matrix_ops.hh>
#include <math/detail/matrix_storage.hh>
#include <math/detail/matrix_access.hh>

namespace neutrino::math {
    namespace detail {
        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class matrix_iterator_proxy;

        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class const_matrix_iterator_proxy;
    }

    template<typename E, std::size_t R, std::size_t C>
    class matrix : public matrix_ops <E, R, C> {
        public:
            using storage_t = detail::matrix_storage <E, R, C>;
            using row_t = typename storage_t::row_t;
            typename storage_t::values_t values{};

        public:
            matrix() = default;

            constexpr matrix(const E (& data)[R][C])
                : values{storage_t::template create(data, std::make_integer_sequence <int, R>{})} {
            }

            auto operator()(std::size_t r, std::size_t c) const {
                if (r >= R) {
                    throw std::out_of_range("matrix: Index out of range");
                }
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

            template<bool ByRow>
            matrix& operator =(const detail::matrix_axis_access <E, R, C, ByRow>& view) {
                for (std::size_t i = 0; i < view.size(); i++) {
                    if constexpr (ByRow) {
                        values[view.get_index()][i] = view[i];
                    } else {
                        values[i][view.get_index()] = view[i];
                    }
                }
                return *this;
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

            [[nodiscard]] constexpr std::size_t get_rows_num() const {
                return R;
            }

            [[nodiscard]] constexpr std::size_t get_columns_num() const {
                return C;
            }
    };


}

#endif
