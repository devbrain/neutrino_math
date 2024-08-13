//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_VECTOR_EXPR_HH
#define NEUTRINO_MATH_VECTOR_EXPR_HH

#include <cstddef>
#include <iostream>
#include <math/detail/traits.hh>

#if defined(PPCAT_NX)
#   undef PPCAT_NX
#endif
#if defined(PPCAT)
#   undef PPCAT
#endif

#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

namespace neutrino::math {
    namespace detail {
        template<typename T, bool store_ref>
        struct storage_type;

        template<typename T>
        struct storage_type <T, true> {
            using type = const T&;
        };

        template<typename T>
        struct storage_type <T, false> {
            using type = T;
        };

        template<typename T>
        constexpr bool store_by_ref_v = traits <std::decay_t <T>>::is_leaf;

        template<typename T>
        using storage_type_t = typename storage_type <std::decay_t <T>, store_by_ref_v <T>>::type;

        template<typename T>
        class temp_value_holder {
            private:
                T value;

            public:
                temp_value_holder(temp_value_holder&& other) noexcept
                    : value(std::move(other.value)) {
                }

                temp_value_holder(const temp_value_holder& other)
                    : value(std::move(const_cast <temp_value_holder&>(other).value)) {
                }

                constexpr explicit temp_value_holder(T&& v)
                    : value(std::move(v)) {
                }

                constexpr auto& operator[](std::size_t i) {
                    return value[i];
                }

                constexpr const auto& operator[](std::size_t i) const {
                    return value[i];
                }

                static constexpr size_t size() {
                    if constexpr (is_scalar_v <T>) {
                        return 1;
                    } else {
                        return T::size();
                    }
                }
        };
    }

    template<class callable, class LHS, class RHS>
    class binary_vector_expression : public vector_expression_base {
        detail::storage_type_t <LHS> lhs;
        detail::storage_type_t <RHS> rhs;

        public:
            explicit binary_vector_expression([[maybe_unused]] callable* f, LHS const& lhs_, RHS const& rhs_)
                : lhs(lhs_), rhs(rhs_) {
            }

            static constexpr std::size_t size() {
                if constexpr (is_scalar_v <RHS> && is_scalar_v <LHS>) {
                    return 1;
                } else if constexpr (is_scalar_v <RHS>) {
                    return LHS::size();
                } else {
                    return RHS::size();
                }
            }

            auto operator[](size_t const i) const {
                return callable::template call(subscript(lhs, i), subscript(rhs, i));
            }

            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const i) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v[i];
                } else {
                    return v;
                }
            }
    };

    template<class Callable, class LHS>
    class unary_vector_expression : public vector_expression_base {
        detail::storage_type_t <LHS> lhs;

        public:
            explicit unary_vector_expression([[maybe_unused]] Callable* f, LHS const& lhs_)
                : lhs(lhs_) {
            }

            static constexpr std::size_t size() {
                if constexpr (is_scalar_v <LHS>) {
                    return 1;
                } else {
                    return LHS::size();
                }
            }

            auto operator[](size_t const i) const {
                return Callable::template call(subscript(lhs, i));
            }

            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const i) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v[i];
                } else {
                    return v;
                }
            }
    };

    namespace detail {
        template<typename T>
        struct size_traits {
            static constexpr size_t size() {
                if constexpr (is_vector_or_vector_exp_v <T>) {
                    return T::size();
                } else {
                    return 1;
                }
            }
        };

        template<typename T>
        struct size_traits <temp_value_holder <T>> {
            static constexpr size_t size() {
                return T::size();
            }
        };

        template<class LHS, class RHS>
        constexpr bool is_compatible_size_v = size_traits <std::decay_t <LHS>>::size() == size_traits <std::decay_t <
                                                  RHS>>::size();

        template<class Callable, class LHS, class RHS>
        auto make_binary_vector_expr(Callable* callable, LHS&& lhs, RHS&& rhs) {
            constexpr auto is_rhs_rvalue = std::is_rvalue_reference_v <decltype(rhs)>;
            constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;

            constexpr auto is_rhs_vec = is_vector_v <RHS>;
            constexpr auto is_lhs_vec = is_vector_v <LHS>;

            if constexpr (is_rhs_vec && is_rhs_rvalue) {
                if constexpr (is_lhs_rvalue && is_lhs_vec) {
                    return binary_vector_expression{
                        callable,
                        detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                        detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
                    };
                } else {
                    return binary_vector_expression{
                        callable,
                        lhs,
                        detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
                    };
                }
            } else if constexpr (is_lhs_rvalue && is_lhs_vec) {
                return binary_vector_expression{
                    callable,
                    detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                    rhs
                };
            } else {
                return binary_vector_expression{
                    callable,
                    lhs, rhs
                };
            }
        }

        template<class Callable, class LHS>
        auto make_unary_vector_expr(Callable* callable, LHS&& lhs) {
            constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;
            constexpr auto is_lhs_vec = is_vector_v <LHS>;

            if constexpr (is_lhs_vec && is_lhs_rvalue) {
                return unary_vector_expression{
                    callable,
                    detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs))
                };
            } else {
                return unary_vector_expression{
                    callable,
                    lhs
                };
            }
        }
    }

    // ================================================================================================================
    // Operators
    // ================================================================================================================
    namespace vector_ops {
        struct plus {
            template<typename A, typename B>
            static auto call(const A& x, const B& y) {
                return x + y;
            }
        };

        struct minus {
            template<typename A, typename B>
            static auto call(const A& x, const B& y) {
                return x - y;
            }
        };

        struct mul {
            template<typename A, typename B>
            static auto call(const A& x, const B& y) {
                return x * y;
            }
        };

        struct div {
            template<typename A, typename B>
            static auto call(const A& x, const B& y) {
                return x / y;
            }
        };

        struct unary_minus {
            template<typename A>
            static auto call(const A& x) {
                return -x;
            }
        };

        struct unary_plus {
            template<typename A>
            static auto call(const A& x) {
                return x;
            }
        };

        struct unary_sin {
            template<typename A>
            static auto call(const A& x) {
                return std::sin(x);
            }
        };



        struct unary_sec {
            template<typename A>
            static auto call(const A& x) {
                return A{1}/std::sin(x);
            }
        };

        struct unary_asin {
            template<typename A>
            static auto call(const A& x) {
                return std::asin(x);
            }
        };

        struct unary_cos {
            template<typename A>
            static auto call(const A& x) {
                return std::cos(x);
            }
        };

        struct unary_cosec {
            template<typename A>
            static auto call(const A& x) {
                return A{1}/std::cos(x);
            }
        };

        struct unary_acos {
            template<typename A>
            static auto call(const A& x) {
                return std::acos(x);
            }
        };

        struct unary_tan {
            template<typename A>
            static auto call(const A& x) {
                return std::tan(x);
            }
        };

        struct unary_atan {
            template<typename A>
            static auto call(const A& x) {
                return std::atan(x);
            }
        };

        struct unary_cotan {
            template<typename A>
            static auto call(const A& x) {
                return std::sin(x) / std::cos(x);
            }
        };

        struct unary_acotan {
            template<typename A>
            static auto call(const A& x) {
                return M_PI_2 - std::atan(x);
            }
        };

        struct unary_inv {
            template<typename A>
            static auto call(const A& x) {
                return A{1}/x;
            }
        };

        struct unary_exp {
            template<typename A>
            static auto call(const A& x) {
                return std::exp(x);
            }
        };

        struct unary_exp2 {
            template<typename A>
            static auto call(const A& x) {
                return std::exp2(x);
            }
        };

        struct unary_log {
            template<typename A>
            static auto call(const A& x) {
                return std::log(x);
            }
        };

        struct unary_log2 {
            template<typename A>
            static auto call(const A& x) {
                return std::log2(x);
            }
        };

        struct unary_log10 {
            template<typename A>
            static auto call(const A& x) {
                return std::log10(x);
            }
        };

        struct unary_sinh {
            template<typename A>
            static auto call(const A& x) {
                return std::sinh(x);
            }
        };

        struct unary_asinh {
            template<typename A>
            static auto call(const A& x) {
                return std::asinh(x);
            }
        };

        struct unary_cosh {
            template<typename A>
            static auto call(const A& x) {
                return std::cosh(x);
            }
        };

        struct unary_acosh {
            template<typename A>
            static auto call(const A& x) {
                return std::acosh(x);
            }
        };

        struct unary_tanh {
            template<typename A>
            static auto call(const A& x) {
                return std::tanh(x);
            }
        };

        struct unary_atanh {
            template<typename A>
            static auto call(const A& x) {
                return std::atanh(x);
            }
        };

        struct unary_abs {
            template<typename A>
            static auto call(const A& x) {
                return std::abs(x);
            }
        };

        struct unary_ceil {
            template<typename A>
            static auto call(const A& x) {
                return std::ceil(x);
            }
        };

        struct unary_floor {
            template<typename A>
            static auto call(const A& x) {
                return std::floor(x);
            }
        };

        struct unary_round {
            template<typename A>
            static auto call(const A& x) {
                return std::round(x);
            }
        };

        struct unary_sgn {
            template<typename A>
            static auto call(const A& x) {
                return x == A{0} ? 0 : std::signbit(x) ? -1 : 1;
            }
        };

        struct unary_signbit {
            template<typename A>
            static auto call(const A& x) {
                return std::signbit(x);
            }
        };

        struct unary_isfinite {
            template<typename A>
            static auto call(const A& x) {
                return std::isfinite(x);
            }
        };

        struct unary_isnan {
            template<typename A>
            static auto call(const A& x) {
                return std::isnan(x);
            }
        };

        struct unary_isinf {
            template<typename A>
            static auto call(const A& x) {
                return std::isnan(x);
            }
        };

        struct unary_sqrt {
            template<typename A>
            static auto call(const A& x) {
                return std::sqrt(x);
            }
        };

        struct unary_invsqrt {
            template<typename A>
            static auto call(const A& x) {
                return A{1}/std::sqrt(x);
            }
        };

        struct unary_cbrt {
            template<typename A>
            static auto call(const A& x) {
                return std::cbrt(x);
            }
        };

        struct unary_invcbrt {
            template<typename A>
            static auto call(const A& x) {
                return A{1}/std::cbrt(x);
            }
        };

        struct unary_pow2 {
            template<typename A>
            static auto call(const A& x) {
                return x*x;
            }
        };

        struct unary_invpow2 {
            template<typename A>
            static auto call(const A& x) {
                return A{1}/x*x;
            }
        };
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto negate(LHS&& lhs) {
        return detail::make_unary_vector_expr(static_cast <vector_ops::unary_minus*>(nullptr), std::forward <LHS>(lhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator +
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator +(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::plus*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator +(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::plus*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <RHS> && is_scalar_v <LHS>>>
    auto operator +(const LHS& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::plus*>(nullptr), lhs, std::forward <RHS>(rhs));
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto operator +(LHS&& lhs) {
        return detail::make_unary_vector_expr(static_cast <vector_ops::unary_plus*>(nullptr), std::forward <LHS>(lhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator -
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator -(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::minus*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator -(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::minus*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto operator -(LHS&& lhs) {
        return detail::make_unary_vector_expr(static_cast <vector_ops::unary_minus*>(nullptr), std::forward <LHS>(lhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator * element-wise multiplication
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator *(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::mul*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator *(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::mul*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <RHS> && is_scalar_v <LHS>>>
    auto operator *(const LHS& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::mul*>(nullptr), lhs, std::forward <RHS>(rhs));
    }

    // ----------------------------------------------------------------------------------------------------------------
    // Operator / : element-wise division
    // ----------------------------------------------------------------------------------------------------------------
    template<class LHS, class RHS, class = std::enable_if_t <
                 is_vector_or_vector_exp_v <LHS> && is_vector_or_vector_exp_v <RHS> && detail::is_compatible_size_v <
                     LHS, RHS>>>
    auto operator /(LHS&& lhs, RHS&& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::div*>(nullptr), std::forward <LHS>(lhs),
                                               std::forward <RHS>(rhs));
    }

    template<class LHS, class RHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS> && is_scalar_v <RHS>>>
    auto operator /(LHS&& lhs, const RHS& rhs) {
        return detail::make_binary_vector_expr(static_cast <vector_ops::div*>(nullptr), std::forward <LHS>(lhs), rhs);
    }

    // ================================================================================================================
    // Functors
    // ================================================================================================================
    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto sum(LHS&& lhs) {
        using lhs_t = element_type_t <LHS>;
        lhs_t out = {};
        constexpr std::size_t size = detail::size_traits <std::decay_t <LHS>>::size();
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
        return detail::make_unary_vector_expr(static_cast <vector_ops::PPCAT(unary_, NAME)*>(nullptr), std::forward <LHS>(lhs)); \
    }                                                                                                                            \
                                                                                                                                 \
    template<class LHS>                                                                                                          \
    auto NAME(LHS&& lhs, std::enable_if_t <is_scalar_v <LHS>>* = nullptr) {                                                      \
        return detail::make_unary_vector_expr(static_cast <vector_ops::PPCAT(unary_, NAME)*>(nullptr), std::forward <LHS>(lhs)); \
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

}

#if defined(PPCAT_NX)
#   undef PPCAT_NX
#endif
#if defined(PPCAT)
#   undef PPCAT
#endif

#endif
