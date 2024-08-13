//
// Created by igor on 8/13/24.
//

#ifndef NEUTRINO_MATH_VECTOR_OPS_HH
#define NEUTRINO_MATH_VECTOR_OPS_HH

#include <math/detail/vector_functors.hh>
#include <math/detail/vector_expr.hh>
#if defined(PPCAT_NX)
#   undef PPCAT_NX
#endif
#if defined(PPCAT)
#   undef PPCAT
#endif

#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

namespace neutrino::math {
// ================================================================================================================
    // Operators
    // ================================================================================================================


    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto negate(LHS&& lhs) {
        return detail::make_unary_vector_expr(static_cast <vector_fn::unary_minus*>(nullptr), std::forward <LHS>(lhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator +
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator +(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::plus*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator +(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::plus*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <RHS> && is_scalar_v <LHS>>>
    auto operator +(const LHS& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::plus*>(nullptr), lhs, std::forward <RHS>(rhs));
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto operator +(LHS&& lhs) {
        return detail::make_unary_vector_expr(static_cast <vector_fn::unary_plus*>(nullptr), std::forward <LHS>(lhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator -
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator -(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::minus*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator -(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::minus*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto operator -(LHS&& lhs) {
        return detail::make_unary_vector_expr(static_cast <vector_fn::unary_minus*>(nullptr), std::forward <LHS>(lhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator * element-wise multiplication
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator *(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::mul*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator *(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::mul*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <RHS> && is_scalar_v <LHS>>>
    auto operator *(const LHS& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::mul*>(nullptr), lhs, std::forward <RHS>(rhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator / : element-wise division
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator /(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::div*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator /(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_fn::div*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    // ================================================================================================================
    // Functors
    // ================================================================================================================
    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto sum(LHS&& lhs) {
        using lhs_t = element_type_t <LHS>;
        lhs_t out = {};
        constexpr std::size_t size = size_v <LHS>;
        for (std::size_t i = 0; i < size; i++) {
            out += lhs[i];
        }
        return out;
    }

    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto dot(LHS&& lhs, RHS&& rhs) {
        return sum(std::forward <LHS>(lhs) * std::forward <RHS>(rhs));
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto norm2(LHS&& lhs) {
        return dot(std::forward <LHS>(lhs), std::forward <LHS>(lhs));
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto norm(LHS&& lhs) {
        return std::sqrt(norm2(std::forward <LHS>(lhs)));
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto normalize(LHS&& lhs) {
        return std::forward <LHS>(lhs) / norm(std::forward <LHS>(lhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto angle(LHS&& lhs, RHS&& rhs) {
        return std::acos(dot(std::forward <LHS>(lhs),
                             std::forward <RHS>(rhs) / (norm(std::forward <LHS>(lhs)) * norm(
                                                            std::forward <RHS>(rhs)))));
    }

    // ================================================================================================
    // Common math functions
    // ================================================================================================
#define d_MATH_VEC1(NAME)                                                                                                        \
    template<class LHS>                                                                                                          \
    auto NAME(LHS&& lhs, std::enable_if_t <is_vector_or_vector_exp_v <LHS>>* = nullptr) {                                        \
        return detail::make_unary_vector_expr(static_cast <vector_fn::PPCAT(unary_, NAME)*>(nullptr), std::forward <LHS>(lhs)); \
    }                                                                                                                            \
                                                                                                                                 \
    template<class LHS>                                                                                                          \
    auto NAME(LHS&& lhs, std::enable_if_t <is_scalar_v <LHS>>* = nullptr) {                                                      \
        return detail::make_unary_vector_expr(static_cast <vector_fn::PPCAT(unary_, NAME)*>(nullptr), std::forward <LHS>(lhs)); \
    }

    d_MATH_VEC1(inv) // x -> 1/x
    d_MATH_VEC1(exp)
    d_MATH_VEC1(exp2)
    d_MATH_VEC1(log)
    d_MATH_VEC1(log2)
    d_MATH_VEC1(log10)

    d_MATH_VEC1(sin)
    d_MATH_VEC1(asin)
    d_MATH_VEC1(sec)
    d_MATH_VEC1(cos)
    d_MATH_VEC1(cosec)
    d_MATH_VEC1(acos)
    d_MATH_VEC1(tan)
    d_MATH_VEC1(atan)
    d_MATH_VEC1(cotan)
    d_MATH_VEC1(acotan)

    d_MATH_VEC1(sinh)
    d_MATH_VEC1(asinh)
    d_MATH_VEC1(cosh)
    d_MATH_VEC1(acosh)
    d_MATH_VEC1(tanh)
    d_MATH_VEC1(atanh)

    d_MATH_VEC1(abs)
    d_MATH_VEC1(ceil)
    d_MATH_VEC1(floor)
    d_MATH_VEC1(round)

    d_MATH_VEC1(sgn)
    d_MATH_VEC1(signbit)
    d_MATH_VEC1(isnan)
    d_MATH_VEC1(isfinite)
    d_MATH_VEC1(isinf)

    d_MATH_VEC1(sqrt)
    d_MATH_VEC1(invsqrt) // x -> 1/sqrt(x)
    d_MATH_VEC1(cbrt)
    d_MATH_VEC1(invcbrt)
    d_MATH_VEC1(pow2)
    d_MATH_VEC1(invpow2)

    // ===========================================================================

}

#if defined(PPCAT_NX)
#   undef PPCAT_NX
#endif
#if defined(PPCAT)
#   undef PPCAT
#endif


#endif
