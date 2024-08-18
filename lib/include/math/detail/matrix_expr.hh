//
// Created by igor on 8/16/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MATRIX_EXPR_HH
#define NEUTRINO_MATH_DETAIL_MATRIX_EXPR_HH

#include <math/matrix_fwd.hh>
#include <math/detail/vector_expr.hh>


namespace neutrino::math::detail {
    template<class Callable, class LHS, class RHS, std::size_t R, std::size_t C>
    class binary_matrix_expression : public matrix_expression_base <R, C> {
        storage_type_t <LHS> lhs;
        storage_type_t <RHS> rhs;

        public:
            explicit binary_matrix_expression(LHS const& lhs_, RHS const& rhs_)
                : lhs(lhs_), rhs(rhs_) {
            }

            static constexpr auto rows() {
                return R;
            }

            static constexpr auto cols() {
                return C;
            }

            auto operator()(size_t const r, size_t const c) const {
                return Callable::template call(subscript(lhs, r, c), subscript(rhs, r, c));
            }

            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const r, std::size_t const c) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v(r, c);
                } else {
                    return v;
                }
            }
    };

    template<class Callable, class LHS, std::size_t R, std::size_t C>
    class unary_matrix_expression : public matrix_expression_base <R, C> {
        storage_type_t <LHS> lhs;

        public:
            explicit unary_matrix_expression(LHS const& lhs_)
                : lhs(lhs_) {
            }

            static constexpr auto rows() {
                return R;
            }

            static constexpr auto cols() {
                return C;
            }

            auto operator()(size_t const r, size_t const c) const {
                return Callable::template call(subscript(lhs, r, c));
            }

            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const r, std::size_t const c) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v(r, c);
                } else {
                    return v;
                }
            }
    };

    template<class LHS, std::size_t R, std::size_t C>
    class vector_as_matrix_expression : public matrix_expression_base <R, C> {
        storage_type_t <LHS> lhs;

        public:
            explicit vector_as_matrix_expression(LHS const& lhs_)
                : lhs(lhs_) {
            }

            static constexpr auto rows() {
                return R;
            }

            static constexpr auto cols() {
                return C;
            }

            auto operator()(size_t const r, size_t const c) const {
                return subscript(lhs, r, c);
            }

            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const r, std::size_t const c) const {
                if constexpr (!is_scalar_v <operand>) {
                    if constexpr (R == 1) {
                        return v(c);
                    } else {
                        return v(r);
                    }
                } else {
                    return v;
                }
            }
    };

    template<class LHS, std::size_t R, std::size_t C, bool ByRow>
    struct matrix_as_vector_expression : vector_expression_base {
        storage_type_t <LHS> lhs;
        std::size_t axis;

        public:
            explicit matrix_as_vector_expression(LHS const& lhs_, std::size_t axis_)
                : lhs(lhs_),
                  axis(axis_) {
            }

            constexpr static std::size_t size() {
                if constexpr (ByRow) {
                    return C;
                } else {
                    return R;
                }
            }

            auto operator[](size_t const idx) const {
                if constexpr (ByRow) {
                    return subscript(lhs, axis, idx);
                } else {
                    return subscript(lhs, idx, axis);
                }
            }

            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const r, std::size_t const c) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v(r, c);
                } else {
                    return v;
                }
            }
    };

    template<class LHS, std::size_t R, std::size_t C>
    class transpose_matrix_expression : public matrix_expression_base <R, C> {
        storage_type_t <LHS> lhs;

        public:
            explicit transpose_matrix_expression(LHS const& lhs_)
                : lhs(lhs_) {
            }

            static constexpr auto rows() {
                return R;
            }

            static constexpr auto cols() {
                return C;
            }

            auto operator()(size_t const r, size_t const c) const {
                return subscript(lhs, c, r);
            }

            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const r, std::size_t const c) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v(r, c);
                } else {
                    return v;
                }
            }
    };

    template<typename T, std::size_t R, std::size_t C>
    class identity_matrix_expression : public matrix_expression_base <R, C> {
        public:
            static constexpr auto rows() {
                return R;
            }

            static constexpr auto cols() {
                return C;
            }

            auto operator()(size_t const r, size_t const c) const {
                return r == c ? T{1} : T{0};
            }
    };


    template<class LHS, std::size_t AXIS, bool ByRow>
    struct matrix_as_vector_holder : vector_expression_base {
        const LHS& lhs;
        std::size_t axis;

        public:
        explicit matrix_as_vector_holder(const LHS& lhs_, std::size_t axis_)
            : lhs(lhs_),
              axis(axis_) {
        }

        static constexpr std::size_t size() {
            return AXIS;
        }

        auto operator[](size_t const idx) const {
            if constexpr (ByRow) {
                return subscript(lhs, axis, idx);
            } else {
                return subscript(lhs, idx, axis);
            }
        }

        template<class operand>
        constexpr auto subscript(operand const& v, std::size_t const r, std::size_t const c) const {
            if constexpr (!is_scalar_v <operand>) {
                return v(r, c);
            } else {
                return v;
            }
        }
    };

    template<class LHS, class RHS, std::size_t R, std::size_t C, std::size_t LHS_COLS, std::size_t RHS_ROWS>
    class matrix_mul_expression : public matrix_expression_base <R, C> {
        storage_type_t <LHS> lhs;
        storage_type_t <RHS> rhs;

        public:
        explicit matrix_mul_expression(LHS const& lhs_, RHS const& rhs_)
            : lhs(lhs_), rhs(rhs_) {
        }

        static constexpr auto rows() {
            return R;
        }

        static constexpr auto cols() {
            return C;
        }

        auto operator()(size_t const r, size_t const c) const {
            matrix_as_vector_holder<LHS, LHS_COLS, true> row(lhs_ref(), r);
            matrix_as_vector_holder<RHS, RHS_ROWS, false> col(rhs_ref(), c);
            return dot(row, col);
        }

        const LHS & lhs_ref() const {
            if constexpr (std::is_reference_v<decltype(lhs)>) {
                return (const LHS&)(lhs);
            } else {
                return *static_cast<const LHS*>(&lhs);
            }
        }

        const RHS& rhs_ref() const {
            if constexpr (std::is_reference_v<decltype(rhs)>) {
                return (const RHS&)(rhs);
            } else {
                return *static_cast<RHS*>(&rhs);
            }
        }

    };

    template<class Callable, class LHS, class RHS, size_t R, size_t C>
    auto make_binary_matrix_expr(LHS&& lhs, RHS&& rhs) {
        constexpr auto is_rhs_rvalue = std::is_rvalue_reference_v <decltype(rhs)>;
        constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;

        constexpr auto is_rhs_vec = is_vector_v <RHS>;
        constexpr auto is_lhs_vec = is_vector_v <LHS>;

        constexpr auto left_is_temp = is_lhs_rvalue && is_lhs_vec;
        constexpr auto right_is_temp = is_rhs_rvalue && is_rhs_vec;
        if constexpr (left_is_temp && right_is_temp) {
            return binary_matrix_expression <Callable, LHS, RHS, R, C>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
            };
        } else if constexpr (!left_is_temp && right_is_temp) {
            return binary_matrix_expression <Callable, LHS, RHS, R, C>{
                lhs,
                detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
            };
        } else if constexpr (left_is_temp) {
            return binary_matrix_expression <Callable, LHS, RHS, R, C>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                rhs
            };
        } else {
            return binary_matrix_expression <Callable, LHS, RHS, R, C>{
                lhs, rhs
            };
        }
    }

    template<class Callable, class LHS, size_t R, size_t C>
    auto make_unary_matrix_expr(LHS&& lhs) {
        constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;
        constexpr auto is_lhs_vec = is_vector_v <LHS>;

        if constexpr (is_lhs_vec && is_lhs_rvalue) {
            return unary_matrix_expression <Callable, LHS, R, C>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs))
            };
        } else {
            return unary_matrix_expression <Callable, LHS, R, C>{
                lhs
            };
        }
    }

    template<class LHS, size_t R, size_t C>
    auto make_view_vector_as_matrix_expr(LHS&& lhs) {
        constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;
        constexpr auto is_lhs_vec = is_vector_v <LHS>;

        if constexpr (is_lhs_vec && is_lhs_rvalue) {
            return vector_as_matrix_expression <LHS, R, C>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs))
            };
        } else {
            return vector_as_matrix_expression <LHS, R, C>{
                lhs
            };
        }
    }

    template<class LHS, std::size_t R, std::size_t C, bool ByRow>
    auto make_view_matrix_as_vector_expr(LHS&& lhs, std::size_t axis) {
        constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;
        constexpr auto is_lhs_vec = is_vector_v <LHS>;

        if constexpr (is_lhs_vec && is_lhs_rvalue) {
            return matrix_as_vector_expression <LHS, R, C, ByRow>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                axis
            };
        } else {
            return matrix_as_vector_expression <LHS, R, C, ByRow>{
                lhs, axis
            };
        }
    }

    template<class LHS, size_t R, size_t C>
    auto make_transpose_matrix_expr(LHS&& lhs) {
        constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;
        constexpr auto is_lhs_vec = is_vector_v <LHS>;

        if constexpr (is_lhs_vec && is_lhs_rvalue) {
            return transpose_matrix_expression <LHS, C, R>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs))
            };
        } else {
            return transpose_matrix_expression <LHS, C, R>{
                lhs
            };
        }
    }

    template<class LHS, class RHS, size_t R, size_t C, std::size_t LHS_COLS, std::size_t RHS_ROWS>
    auto make_mul_matrix_expr(LHS&& lhs, RHS&& rhs) {
        constexpr auto is_rhs_rvalue = std::is_rvalue_reference_v <decltype(rhs)>;
        constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;

        constexpr auto is_rhs_vec = is_vector_v <RHS>;
        constexpr auto is_lhs_vec = is_vector_v <LHS>;

        constexpr auto left_is_temp = is_lhs_rvalue && is_lhs_vec;
        constexpr auto right_is_temp = is_rhs_rvalue && is_rhs_vec;
        if constexpr (left_is_temp && right_is_temp) {
            return matrix_mul_expression <LHS, RHS, R, C, LHS_COLS, RHS_ROWS>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
            };
        } else if constexpr (!left_is_temp && right_is_temp) {
            return matrix_mul_expression <LHS, RHS, R, C, LHS_COLS, RHS_ROWS>{
                lhs,
                detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
            };
        } else if constexpr (left_is_temp) {
            return matrix_mul_expression <LHS, RHS, R, C, LHS_COLS, RHS_ROWS>{
                detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                rhs
            };
        } else {
            return matrix_mul_expression <LHS, RHS, R, C, LHS_COLS, RHS_ROWS>{
                lhs, rhs
            };
        }
    }
}

#endif
