//
// Created by igor on 8/18/24.
//

#ifndef NEUTRINO_MATH_DETAIL_SCALAR_HH
#define NEUTRINO_MATH_DETAIL_SCALAR_HH

#include <cmath>

namespace neutrino::math::detail {
#if !defined(NEUTRINO_MATH_EPSILON)
    template<typename T>
    static constexpr T EPSILON = T{0.00001};
#else
    template <typename T>
    static constexpr T EPSILON = T{NEUTRINO_MATH_EPSILON};
#endif

    template<typename T>
    struct scalar_traits {
        static constexpr bool is_real = false;
    };

    template<>
    struct scalar_traits <float> {
        static constexpr bool is_real = true;
    };

    template<>
    struct scalar_traits <double> {
        static constexpr bool is_real = true;
    };

    template<typename A, typename B>
    struct scalar_coersion;

    template<typename A>
    struct scalar_coersion <A, A> {
        using type = A;
    };

    template<>
    struct scalar_coersion <double, double> {
        using type = double;
    };

    template<>
    struct scalar_coersion <float, float> {
        using type = float;
    };

    template<>
    struct scalar_coersion <float, double> {
        using type = double;
    };

    template<>
    struct scalar_coersion <double, float> {
        using type = double;
    };

    template<typename A>
    struct scalar_coersion <double, A> {
        using type = double;
    };

    template<typename A>
    struct scalar_coersion <A, double> {
        using type = double;
    };

    template<typename A>
    struct scalar_coersion <float, A> {
        using type = float;
    };

    template<typename A>
    struct scalar_coersion <A, float> {
        using type = float;
    };

    template<typename A, typename B>
    using bigger_t = typename scalar_coersion <A, B>::type;

    template<bool FirstIsReal, bool SecondIsReal>
    struct scalar_ops {
        template<typename A, typename B>
        static bool equals(const A& a, const B& b) {
            using target_t = bigger_t <A, B>;
            return std::abs(static_cast <target_t>(a) - static_cast <target_t>(b)) < EPSILON <target_t>;
        }
    };

    template<>
    struct scalar_ops <false, false> {
        template<typename A, typename B>
        static bool equals(const A& a, const B& b) {
            return a == b;
        }
    };

    struct scalar {
        template<typename A, typename B>
        static constexpr bool equals(const A& a, const B& b) {
            return scalar_ops <scalar_traits <std::decay_t <A>>::is_real, scalar_traits <std::decay_t <
                                   B>>::is_real>::equals(a, b);
        }
    };
}

#endif
