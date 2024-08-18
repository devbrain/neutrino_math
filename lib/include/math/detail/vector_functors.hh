//
// Created by igor on 8/13/24.
//

#ifndef NEUTRINO_MATH_VECTOR_FUNCTORS_HH
#define NEUTRINO_MATH_VECTOR_FUNCTORS_HH

#include <cmath>

namespace neutrino::math::vector_fn {
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
                return A{1} / std::sin(x);
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
                return A{1} / std::cos(x);
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
                return A{1} / x;
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
                return A{1} / std::sqrt(x);
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
                return A{1} / std::cbrt(x);
            }
        };

        struct unary_pow2 {
            template<typename A>
            static auto call(const A& x) {
                return x * x;
            }
        };

        struct unary_invpow2 {
            template<typename A>
            static auto call(const A& x) {
                return A{1} / (x * x);
            }
        };

        struct unary_conjugate {
            template<typename A>
            static auto call(const A& x) {
                if constexpr (detail::is_complex_v<A>) {
                    return std::conj(x);
                } else {
                    return x;
                }
            }
        };
    }
#endif
