//
// Created by igor on 8/18/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MATRIX_ACCESS_HH
#define NEUTRINO_MATH_DETAIL_MATRIX_ACCESS_HH

#include <tuple>
#include <math/detail/matrix_storage.hh>

namespace neutrino::math::detail {
    template<typename E, std::size_t R, std::size_t C, bool ByRow>
    class matrix_axis_access : public vector_expression_base,
                               public generic_vector_ops <matrix_axis_access <E, R, C, ByRow>, ByRow ? C : R> {
        typename matrix_storage <E, R, C>::values_t& values;
        std::size_t axis;

        public:
            matrix_axis_access(matrix <E, R, C>& m, std::size_t axis_)
                : values(m.values),
                  axis(axis_) {
            }

            static constexpr std::size_t size() {
                if constexpr (ByRow) {
                    return C;
                } else {
                    return R;
                }
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            matrix_axis_access& operator=(Expr const& expr) {
                if ((void*)this != (void*)&expr) {
                    for (std::size_t i = 0; i < size(); i++) {
                        this->operator[](i) = expr[i];
                    }
                }
                return *this;
            }

            auto& operator[](std::size_t idx) {
                if constexpr (ByRow) {
                    return values[axis][idx];
                } else {
                    return values[idx][axis];
                }
            }

            const auto& operator[](std::size_t idx) const {
                if constexpr (ByRow) {
                    return values[axis][idx];
                } else {
                    return values[idx][axis];
                }
            }

        [[nodiscard]] std::size_t get_index() const noexcept {
                return axis;
            }
    };

    template<typename E, std::size_t R, std::size_t C, bool ByRow>
    struct const_matrix_axis_access : vector_expression_base {
        using vals_t = typename matrix_storage <E, R, C>::values_t;
        const vals_t& values;
        std::size_t axis;

        const_matrix_axis_access(const matrix <E, R, C>& m, std::size_t axis_)
            : values(m.values),
              axis(axis_) {
        }

        static constexpr std::size_t size() {
            if constexpr (ByRow) {
                return C;
            } else {
                return R;
            }
        }

        const auto& operator[](std::size_t idx) const {
            if constexpr (ByRow) {
                return values[axis][idx];
            } else {
                return values[idx][axis];
            }
        }
    };

    template<typename E, std::size_t R, std::size_t C, size_t ... Indices>
    constexpr auto make_access_by_row_helper(matrix <E, R, C>& mat, std::index_sequence <Indices...>) {
        return std::make_tuple(matrix_axis_access <E, R, C, true>(mat, Indices)...);
    }

    template<typename E, std::size_t R, std::size_t C, size_t ... Indices>
    constexpr auto make_access_by_col_helper(matrix <E, R, C>& mat, std::index_sequence <Indices...>) {
        return std::make_tuple(matrix_axis_access <E, R, C, false>(mat, Indices)...);
    }

    template<typename E, std::size_t R, std::size_t C, size_t ... Indices>
    constexpr auto make_access_by_row_helper(const matrix <E, R, C>& mat, std::index_sequence <Indices...>) {
        return std::make_tuple(const_matrix_axis_access <E, R, C, true>(mat, Indices)...);
    }

    template<typename E, std::size_t R, std::size_t C, size_t ... Indices>
    constexpr auto make_access_by_col_helper(const matrix <E, R, C>& mat, std::index_sequence <Indices...>) {
        return std::make_tuple(const_matrix_axis_access <E, R, C, false>(mat, Indices)...);
    }
}

namespace neutrino::math {
    template<typename E, std::size_t R, std::size_t C>
    constexpr auto columns(matrix <E, R, C>& m) {
        return detail::make_access_by_col_helper(m, std::make_index_sequence <C>());
    }

    template<typename E, std::size_t R, std::size_t C>
    constexpr auto columns(const matrix <E, R, C>& m) {
        return detail::make_access_by_col_helper(m, std::make_index_sequence <C>());
    }

    template<typename E, std::size_t R, std::size_t C>
    constexpr auto rows(matrix <E, R, C>& m) {
        return detail::make_access_by_row_helper(m, std::make_index_sequence <R>());
    }

    template<typename E, std::size_t R, std::size_t C>
    constexpr auto rows(const matrix <E, R, C>& m) {
        return detail::make_access_by_col_helper(m, std::make_index_sequence <R>());
    }
}

#endif
