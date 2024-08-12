//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_VECTOR_EXPRESSION_HH
#define NEUTRINO_MATH_VECTOR_EXPRESSION_HH

// https://github.com/cdelv/Vector3D/blob/main/vector.h

#include <type_traits>
#include <complex>
#include <cmath>
#include <math/deprecated/detail/value_holder.hh>

namespace neutrino::math {
    template<typename T>
    struct is_complex : std::false_type {
    };

    template<typename T>
    struct is_complex <std::complex <T>> : std::is_arithmetic <T> {
    };

    template<typename T>
    static constexpr bool is_complex_v = is_complex <T>::value;

    template<typename T>
    static constexpr bool is_number_v = std::is_arithmetic_v <T> || is_complex <T>::value;

    /*
    *  Expression template to avoid unnecessary allocations in chained operations
    */
    template<typename E, std::size_t N>
    class vector_expression {
        public:
        constexpr auto operator[](const std::size_t i) const {
            return static_cast <E const&>(*this)[i];
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    // Element-wise product
    template<typename E1, typename E2, std::size_t N>
    class vec_element_wise_product : public vector_expression <vec_element_wise_product <E1, E2, N>, N> {
        const E1& _u;
        const E2& _v;

        public:
        constexpr vec_element_wise_product(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i] * _v[i];
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    template<typename E1, typename E2, std::size_t N>
    constexpr vec_element_wise_product <E1, E2, N> elem_prod(vector_expression <E1, N> const& u,
                                                             vector_expression <E2, N> const& v) noexcept {
        return vec_element_wise_product <E1, E2, N>(*static_cast <const E1*>(&u), *static_cast <const E2*>(&v));
    }

    // Cross Product
    template<typename E1, typename E2>
    class vec_cross_product : public vector_expression <vec_cross_product <E1, E2>, 3> {
        const E1& _u;
        const E2& _v;

        public:
        constexpr vec_cross_product(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            if (i == 0) return _u[1] * _v[2] - _u[2] * _v[1];
            if (i == 1) return _u[2] * _v[0] - _u[0] * _v[2];
            if (i == 2) return _u[0] * _v[1] - _u[1] * _v[0];
            return 0 * _u[0];
        }

        static constexpr std::size_t size() {
            return 3;
        }
    };

    /*
    *  OPERATORS
    */
    // Sum
    template<typename E1, typename E2, std::size_t N>
    class vec_sum : public vector_expression <vec_sum <E1, E2, N>, N> {
        const E1& _u;
        const E2& _v;

        public:
        constexpr vec_sum(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i] + _v[i];
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    // +v operator
    // Left Sum
    template<typename E1, std::size_t N>
    class left_vec_sum : public vector_expression <left_vec_sum <E1, N>, N> {
        const E1& _u;

        public:
        constexpr left_vec_sum(const E1& u) noexcept
            : _u(u) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i];
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    // Subtraction
    template<typename E1, typename E2, std::size_t N>
    class vec_subtraction : public vector_expression <vec_subtraction <E1, E2, N>, N> {
        const E1& _u;
        const E2& _v;

        public:
        constexpr vec_subtraction(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i] - _v[i];
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    // -v operator
    // Left Subtraction
    template<typename E1, std::size_t N>
    class left_vec_subtraction : public vector_expression <left_vec_subtraction <E1, N>, N> {
        const E1& _u;

        public:
        constexpr left_vec_subtraction(const E1& u) noexcept
            : _u(u) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return -_u[i];
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    // Scalar multiplication
    template<typename E1, typename E2, std::size_t N>
    class left_scalar_product : public vector_expression <left_scalar_product <E1, E2, N>, N> {
        static_assert(is_number_v <E2>);

        const E1& _u;
        const E2& _v;

        public:
        constexpr left_scalar_product(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i] * _v;
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    template<typename E1, typename E2, std::size_t N>
    class right_vec_scalar_product : public vector_expression <right_vec_scalar_product <E1, E2, N>, N> {
        static_assert(is_number_v <E2>);
        const E1& _u;
        const E2& _v;

        public:
        constexpr right_vec_scalar_product(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i] * _v;
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    // Element-wise division
    template<typename E1, typename E2, std::size_t N>
    class vec_element_wise_division : public vector_expression <vec_element_wise_division <E1, E2, N>, N> {
        const E1& _u;
        const E2& _v;

        public:
        constexpr vec_element_wise_division(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i] / _v[i];
        }

        static constexpr std::size_t size() {
            return N;
        }
    };

    // Scalar Division
    template<typename E1, typename E2, std::size_t N>
    class vec_scalar_division : public vector_expression <vec_scalar_division <E1, E2, N>, N> {
        const E1& _u;
        const E2& _v;

        public:
        constexpr vec_scalar_division(const E1& u, const E2& v) noexcept
            : _u(u), _v(v) {
        };

        constexpr auto operator[](const std::size_t i) const {
            return _u[i] / _v;
        }

        static constexpr std::size_t size() {
            return N;
        }
    };
}

#include <math/deprecated/detail/vector_functions.hh>

namespace neutrino::math {
    // Normalization
    template<typename E1, typename E2, std::size_t N>
    class vec_normalization : public vector_expression <vec_normalization <E1, E2, N>, N> {
        const E1& _u;
        const E2 Norm;

        public:
            constexpr vec_normalization(const E1& u) noexcept
                : _u(u), Norm(norm(u)) {
            };

            constexpr auto operator[](const std::size_t i) const {
                return _u[i] / Norm;
            }

            static constexpr std::size_t size() {
                return N;
            }
    };
}

#endif
