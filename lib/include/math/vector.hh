//
// Created by igor on 8/12/24.
//

#ifndef NEUTRINO_MATH_VECTOR_HH
#define NEUTRINO_MATH_VECTOR_HH

#include <stdexcept>
#include <math/vector_fwd.hh>
#include <math/detail/traits.hh>

namespace neutrino::math {
    template<typename T>
    class vector <T, 3> {
        public:
            T x{}, y{}, z{};

            static constexpr std::size_t size() {
                return 3;
            }

            constexpr vector() noexcept = default;
            constexpr vector(const vector& other) noexcept = default;
            constexpr vector(vector&& other) noexcept = default;

            constexpr explicit vector(const T value) noexcept
                : x(value), y(value), z(value) {
            };

            constexpr vector(const T x_val, const T y_val, const T z_val) noexcept
                : x(x_val), y(y_val), z(z_val) {
            }

            constexpr vector& operator=(const vector& other) noexcept = default;
            constexpr vector& operator=(vector&& other) noexcept = default;

            constexpr void load(const T x_val, const T y_val, const T z_val) noexcept {
                x = x_val;
                y = y_val;
                z = z_val;
            }

            template<class Expr, class = std::enable_if_t <is_vector_expression_v <Expr>>>
            constexpr vector(Expr const& expr) noexcept {
                x = expr[0];
                y = expr[1];
                z = expr[2];
            }

            template<class Expr, class = std::enable_if_t <is_vector_expression_v <Expr>>>
            vector& operator=(Expr const& expr) {
                x = expr[0];
                y = expr[1];
                z = expr[2];
                return *this; // this line was missing in the slides and in the talk
            }

            constexpr const T& operator[](const std::size_t i) const {
                if (i == 0) return x;
                else if (i == 1) return y;
                else if (i == 2) return z;
                else throw std::out_of_range("vector3D: Index out of range");
            }

            constexpr T& operator[](const std::size_t i) {
                if (i == 0) return x;
                else if (i == 1) return y;
                else if (i == 2) return z;
                else throw std::out_of_range("vector3D: Index out of range");
            }
    };

template<typename T>
    class vector <T, 2> {
        public:
            T x{}, y{};

            static constexpr std::size_t size() {
                return 2;
            }

            constexpr vector() noexcept = default;
            constexpr vector(const vector& other) noexcept = default;
            constexpr vector(vector&& other) noexcept = default;

            constexpr explicit vector(const T value) noexcept
                : x(value), y(value) {
            };

            constexpr vector(const T x_val, const T y_val) noexcept
                : x(x_val), y(y_val) {
            }

            constexpr vector& operator=(const vector& other) noexcept = default;
            constexpr vector& operator=(vector&& other) noexcept = default;

            constexpr void load(const T x_val, const T y_val, const T z_val) noexcept {
                x = x_val;
                y = y_val;
            }

            template<class Expr, class = std::enable_if_t <is_vector_expression_v <Expr>>>
            constexpr vector(Expr const& expr) noexcept {
                x = expr[0];
                y = expr[1];
            }

            template<class Expr, class = std::enable_if_t <is_vector_expression_v <Expr>>>
            vector& operator=(Expr const& expr) {
                x = expr[0];
                y = expr[1];
                return *this; // this line was missing in the slides and in the talk
            }

            constexpr const T& operator[](const std::size_t i) const {
                if (i == 0) return x;
                else if (i == 1) return y;
                else throw std::out_of_range("vector2D: Index out of range");
            }

            constexpr T& operator[](const std::size_t i) {
                if (i == 0) return x;
                else if (i == 1) return y;
                else throw std::out_of_range("vector2D: Index out of range");
            }
    };
}

#endif
