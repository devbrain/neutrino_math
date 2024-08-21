//
// Created by igor on 8/14/24.
//

#ifndef NEUTRINO_MATH_MATRIX_HH
#define NEUTRINO_MATH_MATRIX_HH

#include <neutrino/math/vector.hh>
#include <neutrino/math/detail/matrix_ops.hh>
#include <neutrino/math/detail/matrix_storage.hh>
#include <neutrino/math/detail/matrix_access.hh>

namespace neutrino::math {
    namespace detail {
        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class matrix_iterator_proxy;

        template<typename E, std::size_t R, std::size_t C, bool ByRow>
        class const_matrix_iterator_proxy;
    }

    namespace detail {
        template<std::size_t R, std::size_t C, typename T, typename... E>
        matrix <T, R, C> make_from_rows_proxy(const vector <E, C>&... rows);

        struct from_rows_tag {
        };
    }

    template<typename E, std::size_t R, std::size_t C>
    class matrix : public matrix_ops <E, R, C> {
        public:
            using storage_t = detail::matrix_storage <E, R, C>;
            using row_t = typename storage_t::row_t;
            typename storage_t::values_t values{};

        public:
            matrix() = default;
            matrix(const matrix&) noexcept = default;
            matrix(matrix&&) noexcept = default;
            matrix& operator=(const matrix&) noexcept = default;
            matrix& operator=(matrix&&) noexcept = default;

            constexpr matrix(const E (& data)[R][C])
                : values{storage_t::template create(data, std::make_integer_sequence <int, R>{})} {
            }

            template<typename Expr, class = std::enable_if_t <is_matrix_or_matrix_exp_v <Expr, R, C>>>
            constexpr matrix(const Expr& expr)
                : values{storage_t::template create_from_expr(expr, std::make_integer_sequence <int, R>{})} {
            }

            constexpr const auto& operator()(std::size_t r, std::size_t c) const {
                if (r >= R) {
                    throw std::out_of_range("matrix: Index out of range");
                }
                return values[r][c];
            }

            constexpr auto& operator()(std::size_t r, std::size_t c) {
                if (r >= R) {
                    throw std::out_of_range("matrix: Index out of range");
                }
                return values[r][c];
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

            template<typename Expr, class = std::enable_if_t<is_vector_or_vector_exp_v<Expr> && Expr::size() == R>>
            void set_column(const Expr& col, std::size_t col_idx) {
                for (std::size_t r = 0; r < R; r++) {
                    values[r][col_idx] = col[r];
                }
            }

            template<typename Expr, class = std::enable_if_t<is_vector_or_vector_exp_v<Expr> && Expr::size() == C>>
            void set_row(const Expr& row, std::size_t row_idx) {
                for (std::size_t c = 0; c < C; c++) {
                    values[row_idx][c] = row[c];
                }
            }

            [[nodiscard]] constexpr std::size_t get_rows_num() const {
                return R;
            }

            [[nodiscard]] constexpr std::size_t get_columns_num() const {
                return C;
            }

        private:
            template<std::size_t RR, std::size_t CC, typename TT, typename... EE>
            friend matrix <TT, RR, CC> detail::make_from_rows_proxy(const vector <EE, CC>&... rows);

            template<typename... T, class = std::enable_if_t <sizeof...(T) == R && is_all_same_v <T...>>>
            constexpr explicit matrix([[maybe_unused]] detail::from_rows_tag, const vector <T, C>&... args)
                : values{args...} {
            }
    };

    namespace detail {
        template<std::size_t R, std::size_t C, typename T, typename... E>
        matrix <T, R, C> make_from_rows_proxy(const vector <E, C>&... rows) {
            return matrix <T, sizeof...(rows), C>(from_rows_tag{}, rows...);
        }
    }
}

#endif
