//
// Created by igor on 8/16/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MATRIX_OPS_HH
#define NEUTRINO_MATH_DETAIL_MATRIX_OPS_HH

#include <neutrino/math/matrix.hh>
#include <neutrino/math/vector.hh>
#include <neutrino/math/detail/matrix_expr.hh>
#include <neutrino/math/detail/matrix_traits.hh>


#if !defined(PPCAT)
#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)
#endif


namespace neutrino::math {
    template<typename Vec, class = std::enable_if_t <is_vector_or_vector_exp_v <Vec>>>
    auto as_row_vector(Vec&& vec) {
        return detail::make_view_vector_as_matrix_expr <Vec, 1, vector_size_traits <Vec>::size()>(
            std::forward <Vec>(vec));
    }

    template<typename Vec, class = std::enable_if_t <is_vector_or_vector_exp_v <Vec>>>
    auto as_column_vector(Vec&& vec) {
        return detail::make_view_vector_as_matrix_expr <Vec, vector_size_traits <Vec>::size(), 1>(
            std::forward <Vec>(vec));
    }

    template<typename Mat, class = detail::enable_if_matrix_t <Mat>>
    auto transpose(Mat&& m) {
        return detail::make_transpose_matrix_expr <Mat, detail::matrix_rows_v <Mat>, detail::matrix_columns_v <Mat>>(
            std::forward <Mat>(m));
    }

    template<typename T, std::size_t R, std::size_t C>
    auto identity() {
        return detail::identity_matrix_expression <T, R, C>();
    }

    template<typename Mat, class = detail::enable_if_matrix_t <Mat>>
    auto nth_row(Mat&& m, std::size_t axis) {
        return detail::matrix_as_vector_expression <
            Mat, detail::matrix_rows_v <Mat>, detail::matrix_columns_v <Mat>, true>(std::forward <Mat>(m), axis);
    }

    template<typename Mat, class = detail::enable_if_matrix_t <Mat>>
    auto nth_column(Mat&& m, std::size_t axis) {
        return detail::matrix_as_vector_expression <
            Mat, detail::matrix_rows_v <Mat>, detail::matrix_columns_v <Mat>, false>(std::forward <Mat>(m), axis);
    }

    template<class Mat, class = detail::enable_if_matrix_t <Mat>, bool IsMatrix = true>
    auto conj(Mat&& lhs) {
        return detail::make_unary_matrix_expr <vector_fn::unary_conjugate, Mat, detail::matrix_rows_v <Mat>,
                                               detail::matrix_columns_v <Mat>>(
            std::forward <Mat>(lhs));
    }

    template<class Mat, class = detail::enable_if_matrix_t <Mat>>
    auto adjoint(Mat&& lhs) {
        return transpose(conj(std::forward <Mat>(lhs)));
    }

    template<class Mat, class = detail::enable_if_matrix_t <Mat>, bool IsMatrix = true>
    auto sum(Mat&& lhs) {
        auto tr = lhs(0, 0);
        for (std::size_t r = 1; r < detail::matrix_rows_v <Mat>; r++) {
            tr += lhs(r, 0);
        }
        for (std::size_t c = 1; c < detail::matrix_columns_v <Mat>; c++) {
            tr += lhs(0, c);
        }
        for (std::size_t r = 1; r < detail::matrix_rows_v <Mat>; r++) {
            for (std::size_t c = 1; c < detail::matrix_columns_v <Mat>; c++) {
                tr += lhs(r, c);
            }
        }
        return tr;
    }

    template<class Mat, class = detail::enable_if_matrix_t <Mat>, bool IsMatrix = true>
    auto max(Mat&& lhs) {
        auto tr = lhs(0, 0);
        for (std::size_t r = 1; r < detail::matrix_rows_v <Mat>; r++) {
            if (lhs(r, 0) > tr) {
                tr = lhs(r, 0);
            }
        }
        for (std::size_t c = 1; c < detail::matrix_columns_v <Mat>; c++) {
            if (lhs(0, c) > tr) {
                tr = lhs(0, c);
            }
        }

        for (std::size_t r = 1; r < detail::matrix_rows_v <Mat>; r++) {
            for (std::size_t c = 1; c < detail::matrix_columns_v <Mat>; c++) {
                if (lhs(r, c) > tr) {
                    tr = lhs(r, c);
                }
            }
        }
        return tr;
    }

    template<class Mat, class = detail::enable_if_matrix_t <Mat>, bool IsMatrix = true>
    auto min(Mat&& lhs) {
        auto tr = lhs(0, 0);
        for (std::size_t r = 1; r < detail::matrix_rows_v <Mat>; r++) {
            if (lhs(r, 0) < tr) {
                tr = lhs(r, 0);
            }
        }
        for (std::size_t c = 1; c < detail::matrix_columns_v <Mat>; c++) {
            if (lhs(0, c) < tr) {
                tr = lhs(0, c);
            }
        }

        for (std::size_t r = 1; r < detail::matrix_rows_v <Mat>; r++) {
            for (std::size_t c = 1; c < detail::matrix_columns_v <Mat>; c++) {
                if (lhs(r, c) < tr) {
                    tr = lhs(r, c);
                }
            }
        }
        return tr;
    }

    template<class Mat, class = detail::enable_if_square_t <Mat>>
    auto trace(Mat&& lhs) {
        auto tr = lhs(0, 0);
        for (std::size_t r = 1; r < detail::matrix_rows_v <Mat>; r++) {
            tr += lhs(r, r);
        }
        return tr;
    }

    template<class A, class B, class = detail::enable_if_can_mul_matrix_t <A, B>, bool isMatrix = true>
    auto dot(A&& lhs, B&& rhs) {
        return detail::make_mul_matrix_expr <A, B,
                                             detail::matrix_rows_v <A>, detail::matrix_columns_v <B>,
                                             detail::matrix_columns_v <A>, detail::matrix_rows_v <B>
        >(std::forward <A>(lhs), std::forward <B>(rhs));
    }

    template<class A, class B, class = detail::enable_if_same_size_matrix_t <A, B>, bool isMatrix = true>
    bool operator == (A&& a, B&& b) {
        for (std::size_t r = 0; r < detail::matrix_rows_v <A>; r++) {
            for (std::size_t c = 0; c < detail::matrix_columns_v <A>; c++) {
                if (!detail::scalar::equals(a(r, c), b(r, c))) {
                    return false;
                }
            }
        }
        return true;
    }

    template<class A, class B, class = detail::enable_if_square_t<A>, class = std::enable_if_t<is_scalar_v<B>>, bool isMatrix = true>
    bool operator == (A&& a, B&& b) {
        for (std::size_t r = 0; r < detail::matrix_rows_v <A>; r++) {
            for (std::size_t c = 0; c < detail::matrix_columns_v <A>; c++) {
                if (!detail::scalar::equals(a(r, c), b)) {
                    return false;
                }
            }
        }
        return true;
    }

    template<class A, class B, class = detail::enable_if_square_t<B>, class = std::enable_if_t<is_scalar_v<A>>, bool isMatrix = true, bool = true>
    bool operator == (A&& a, B&& b) {
        for (std::size_t r = 0; r < detail::matrix_rows_v <A>; r++) {
            for (std::size_t c = 0; c < detail::matrix_columns_v <A>; c++) {
                if (!detail::scalar::equals(b(r, c), a)) {
                    return false;
                }
            }
        }
        return true;
    }


    // =============================================================================
    // Pointwise operators
    // =============================================================================
#define d_MATH_MAT2(OP, NAME)                                                                                                                                                                   \
    template <class LHS, class RHS, class = detail::enable_if_same_size_matrix_t<LHS, RHS>, bool isMatrix = true>                                                                               \
    auto operator OP (LHS&& lhs, RHS&& rhs) {                                                                                                                                                   \
        return detail::make_binary_matrix_expr<vector_fn::NAME, LHS, RHS, detail::matrix_rows_v<LHS>, detail::matrix_columns_v<LHS>>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));           \
    }                                                                                                                                                                                           \
                                                                                                                                                                                                \
    template <class LHS, class RHS, class = detail::enable_if_matrix_t<LHS>, class = std::enable_if_t<is_scalar_v<RHS>, op_scalar>, bool = true>                                                \
    auto operator OP (LHS&& lhs, RHS&& rhs) {                                                                                                                                                   \
        return detail::make_binary_matrix_expr<vector_fn::NAME, LHS, RHS, detail::matrix_rows_v<LHS>, detail::matrix_columns_v<LHS>>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));           \
    }                                                                                                                                                                                           \
                                                                                                                                                                                                \
    template <class LHS, class RHS, class = std::enable_if_t<is_scalar_v<LHS>, op_scalar> ,class = detail::enable_if_matrix_t<RHS>>                                                             \
    auto operator OP (LHS&& lhs, RHS&& rhs) {                                                                                                                                                   \
        return detail::make_binary_matrix_expr<vector_fn::NAME, LHS, RHS, detail::matrix_rows_v<RHS>, detail::matrix_columns_v<RHS>>(std::forward<LHS>(lhs), std::forward<RHS>(rhs));           \
    }

    d_MATH_MAT2(+, plus)
    d_MATH_MAT2(-, minus)
    d_MATH_MAT2(/, div)
    d_MATH_MAT2(*, mul)

    // =============================================================================
    // element-wise functions
    // =============================================================================

#define d_MATH_MAT1(NAME)                                                           \
    template<class LHS, class = detail::enable_if_matrix_t <LHS>>                   \
    auto NAME(LHS&& lhs) {                                                          \
        return detail::make_unary_matrix_expr<vector_fn::PPCAT(unary_, NAME),       \
                LHS, detail::matrix_rows_v <LHS>,                                   \
                detail::matrix_columns_v <LHS>>                                     \
                (std::forward <LHS>(lhs));                                          \
    }

    d_MATH_MAT1(inv) // x -> 1/x
    d_MATH_MAT1(exp)
    d_MATH_MAT1(exp2)
    d_MATH_MAT1(log)
    d_MATH_MAT1(log2)
    d_MATH_MAT1(log10)

    d_MATH_MAT1(sin)
    d_MATH_MAT1(asin)
    d_MATH_MAT1(sec)
    d_MATH_MAT1(cos)
    d_MATH_MAT1(cosec)
    d_MATH_MAT1(acos)
    d_MATH_MAT1(tan)
    d_MATH_MAT1(atan)
    d_MATH_MAT1(cotan)
    d_MATH_MAT1(acotan)

    d_MATH_MAT1(sinh)
    d_MATH_MAT1(asinh)
    d_MATH_MAT1(cosh)
    d_MATH_MAT1(acosh)
    d_MATH_MAT1(tanh)
    d_MATH_MAT1(atanh)

    d_MATH_MAT1(abs)
    d_MATH_MAT1(ceil)
    d_MATH_MAT1(floor)
    d_MATH_MAT1(round)

    d_MATH_MAT1(sgn)
    d_MATH_MAT1(signbit)
    d_MATH_MAT1(isnan)
    d_MATH_MAT1(isfinite)
    d_MATH_MAT1(isinf)

    d_MATH_MAT1(sqrt)
    d_MATH_MAT1(invsqrt) // x -> 1/sqrt(x)
    d_MATH_MAT1(cbrt)
    d_MATH_MAT1(invcbrt)
    d_MATH_MAT1(pow2)
    d_MATH_MAT1(invpow2)

    template<class T, std::size_t R, std::size_t C>
    struct matrix_ops {
        using Derived = matrix <T, R, C>;

        auto rows() const noexcept {
            return R;
        }

        auto columns() const noexcept {
            return C;
        }

        template <typename E>
        Derived& operator += (E&& x) {
            *self() = *self() + std::forward<E>(x);
            return *self();
        }

        template <typename E>
        Derived& operator -= (E&& x) {
            *self() = *self() - std::forward<E>(x);
            return *self();
        }

        template <typename E>
        Derived& operator *= (E&& x) {
            *self() = *self() * std::forward<E>(x);
            return *self();
        }

        template <typename E>
        Derived& operator /= (E&& x) {
            *self() = *self() / std::forward<E>(x);
            return *self();
        }


        [[nodiscard]] std::string to_string() const {
            std::ostringstream os;
            os << "[";
            for (std::size_t r=0; r<R; r++) {
                if (r > 0) {
                    os << " ";
                }
                os << self()->values[r].to_string();
                if (r < R-1) {
                    os << "\n";
                }
            }
            os << "]";
            return os.str();
        }

        friend std::ostream& operator << (std::ostream& os, const matrix_ops<T, R, C>& x) {
            os << x.to_string();
            return os;
        }

        protected:
            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const r, std::size_t c) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v(r, c);
                } else {
                    return v;
                }
            }

            const auto& get(std::size_t r, std::size_t c) const {
                return self()->operator()(r, c);
            }

            auto& get(std::size_t r, std::size_t c) {
                return self()->operator()(r, c);
            }

            Derived* self() {
                return static_cast <Derived*>(this);
            }

            const Derived* self() const {
                return static_cast <const Derived*>(this);
            }
    };
}

#endif
