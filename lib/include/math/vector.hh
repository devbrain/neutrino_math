//
// Created by igor on 8/12/24.
//

#ifndef NEUTRINO_MATH_VECTOR_HH
#define NEUTRINO_MATH_VECTOR_HH

#include <stdexcept>
#include <array>

#include <math/detail/traits.hh>
#include <math/detail/vector_ops.hh>
#include <math/detail/array_utils.hh>

#define MATH_VEC_ENABLE_IF_EXPR class = std::enable_if_t <is_vector_expression_v <Expr> && size_v<Expr> == size()>

namespace neutrino::math {
    template<typename T>
    class vector <T, 1> : public vector_ops <T, 1> {
        public:
            using value_type = T;

            T x{};

            static constexpr std::size_t size() {
                return 1;
            }

            constexpr vector() noexcept = default;
            constexpr vector(const vector& other) noexcept = default;
            constexpr vector(vector&& other) noexcept = default;

            constexpr explicit vector(const T value) noexcept
                : x(value) {
            };

            explicit constexpr vector(const std::array <T, 1>& args) noexcept
                : x(args[0]) {
            }

            explicit vector(const T (& data)[1])
                : x(data[0]) {
            }

            constexpr vector& operator=(const vector& other) noexcept = default;
            constexpr vector& operator=(vector&& other) noexcept = default;

            constexpr void load(const T x_val) noexcept {
                x = x_val;
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            constexpr vector(Expr const& expr) noexcept {
                x = expr[0];
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            vector& operator=(Expr const& expr) {
                x = expr[0];
                return *this; // this line was missing in the slides and in the talk
            }

            constexpr const T& operator[](const std::size_t i) const {
                if (i == 0) return x;
                else throw std::out_of_range("vector2D: Index out of range");
            }

            constexpr T& operator[](const std::size_t i) {
                if (i == 0) return x;
                else throw std::out_of_range("vector2D: Index out of range");
            }
    };

    template<typename T>
    class vector <T, 2> : public vector_ops <T, 2> {
        public:
            using value_type = T;
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

            explicit constexpr vector(const std::array <T, 2>& args) noexcept
                : x(args[0]), y(args[1]) {
            }

            explicit vector(const T (& data)[2])
                : x(data[0]), y(data[1]) {
            }

            constexpr vector& operator=(const vector& other) noexcept = default;
            constexpr vector& operator=(vector&& other) noexcept = default;

            constexpr void load(const T x_val, const T y_val) noexcept {
                x = x_val;
                y = y_val;
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            constexpr vector(Expr const& expr) noexcept {
                x = expr[0];
                y = expr[1];
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
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

    template<typename T>
    class vector <T, 3> : public vector_ops <T, 3> {
        public:
            using value_type = T;
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

            constexpr explicit vector(const std::array <T, 3>& args) noexcept
                : x(args[0]), y(args[1]), z(args[2]) {
            }

            explicit vector(const T (& data)[3])
                : x(data[0]), y(data[1]), z(data[2]) {
            }

            constexpr vector& operator=(const vector& other) noexcept = default;
            constexpr vector& operator=(vector&& other) noexcept = default;

            constexpr void load(const T x_val, const T y_val, const T z_val) noexcept {
                x = x_val;
                y = y_val;
                z = z_val;
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            constexpr vector(Expr const& expr) noexcept {
                x = expr[0];
                y = expr[1];
                z = expr[2];
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            vector& operator=(Expr const& expr) {
                x = expr[0];
                y = expr[1];
                z = expr[2];
                return *this;
            }

            constexpr const T& operator[](const std::size_t i) const {
                switch (i) {
                    case 0:
                        return x;
                    case 1:
                        return y;
                    case 2:
                        return z;
                    default:
                        throw std::out_of_range("vector3D: Index out of range");
                }
            }

            constexpr T& operator[](const std::size_t i) {
                if (i == 0) return x;
                else if (i == 1) return y;
                else if (i == 2) return z;
                else throw std::out_of_range("vector3D: Index out of range");
            }
    };

    template<typename T, std::size_t N>
    class vector : public vector_ops <T, N> {
        public:
            using value_type = T;
            std::array <T, N> values;

            static constexpr std::size_t size() {
                return N;
            }

            constexpr vector() noexcept = default;
            constexpr vector(const vector& other) noexcept = default;
            constexpr vector(vector&& other) noexcept = default;

            constexpr explicit vector(T&& value) noexcept
                : values(detail::make_array(value, std::make_index_sequence <N>())) {
            };

            explicit vector(const T (& data)[N])
                : values{
                    detail::array_from_values <T, N>(std::forward <decltype(data)>(data),
                                                     std::make_integer_sequence <int, N>{})
                } {
            }

            constexpr explicit vector(const std::array <T, N>& args) noexcept
                : values(args) {
            }

            template<typename... E, class = std::enable_if_t <sizeof...(E) == N>>
            constexpr explicit vector(E... args)
                : values(detail::copy_array(args...)) {
            }

            constexpr vector& operator=(const vector& other) noexcept = default;
            constexpr vector& operator=(vector&& other) noexcept = default;

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            constexpr vector(Expr const& expr) noexcept {
                for (std::size_t i = 0; i < N; i++) {
                    values[i] = expr[i];
                }
            }

            template<class Expr, MATH_VEC_ENABLE_IF_EXPR>
            vector& operator=(Expr const& expr) {
                if (this != &expr) {
                    for (std::size_t i = 0; i < N; i++) {
                        values[i] = expr[i];
                    }
                }
                return *this;
            }

            constexpr const T& operator[](const std::size_t i) const {
                if (i < N) {
                    return values[i];
                } else throw std::out_of_range("vectorN: Index out of range");
            }

            constexpr T& operator[](const std::size_t i) {
                if (i < N) {
                    return values[i];
                } else throw std::out_of_range("vectorN: Index out of range");
            }
    };

}

#endif
