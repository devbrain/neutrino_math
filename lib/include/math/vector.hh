//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_SDLPP_MATH_VECTOR_HH
#define NEUTRINO_SDLPP_MATH_VECTOR_HH

#include <math/detail/vector_expression.hh>
#include <math/detail/vector_operations.hh>
#include <vector>

/*
*  Vector Classes
*/
namespace neutrino::sdl::math {
    template<typename  T>
    class vector_3d : public vector_expression <vector_3d <T>, 3> {
        static_assert(neutrino::sdl::math::is_number_v<T>);
        public:
        T x, y, z;

        static constexpr std::size_t size() {
            return 3;
        }

        constexpr vector_3d() noexcept = default;
        constexpr vector_3d(const vector_3d& other) noexcept = default;
        constexpr vector_3d(vector_3d&& other) noexcept = default;

        constexpr vector_3d(const T value) noexcept
            : x(value), y(value), z(value) {
        };

        constexpr vector_3d(const T x_val, const T y_val, const T z_val) noexcept
            : x(x_val), y(y_val), z(z_val) {
        }

        constexpr vector_3d& operator=(const vector_3d& other) noexcept = default;
        constexpr vector_3d& operator=(vector_3d&& other) noexcept = default;

        constexpr void load(const T x_val, const T y_val, const T z_val) noexcept {
            x = x_val;
            y = y_val;
            z = z_val;
        }

        template<typename E>
        constexpr vector_3d(const vector_expression <E, 3>& expr) noexcept {
            x = expr[0];
            y = expr[1];
            z = expr[2];
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

        /*
        *  OPERATORS
        */
        template<typename E>
        constexpr vector_3d <T>& operator+=(const vector_expression <E, 3>& expr) noexcept {
            x += expr[0];
            y += expr[1];
            z += expr[2];
            return *this;
        }

        template<typename E>
        constexpr vector_3d <T>& operator-=(const vector_expression <E, 3>& expr) noexcept {
            x -= expr[0];
            y -= expr[1];
            z -= expr[2];
            return *this;
        }

        template<typename E>
        constexpr vector_3d <T>& operator*=(const E& a) noexcept {
            static_assert(neutrino::sdl::math::is_number_v<E>);
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }

        template<typename E>
        constexpr std::enable_if_t<is_number_v<E>, vector_3d <T>>& operator/=(const E& a) noexcept {
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }

        template<typename E>
        constexpr vector_3d <T>& operator/=(const vector_expression <E, 3>& expr) noexcept {
            x /= expr[0];
            y /= expr[1];
            z /= expr[2];
            return *this;
        }

        template<typename E>
        constexpr vector_3d <T>& operator^=(const vector_expression <E, 3>& expr) noexcept {
            T xtemp = y * expr[2] - z * expr[1];
            T ytemp = z * expr[0] - x * expr[2];
            T ztemp = x * expr[1] - y * expr[0];
            x = xtemp;
            y = ytemp;
            z = ztemp;
            return *this;
        }

        constexpr T norm2() const noexcept {
            return dot(*this, *this);
        }

        constexpr T norm() const noexcept {
            return std::sqrt(norm2());
        }

        constexpr const vector_3d <T>& unit() noexcept {
            *this /= norm();
            return *this;
        }
    };

    template<typename T>
    class vector_2d : public vector_expression <vector_2d <T>, 2> {
        static_assert(neutrino::sdl::math::is_number_v<T>);
        public:
        T x, y;

        static constexpr std::size_t size() {
            return 2;
        }

        constexpr vector_2d() noexcept = default;
        constexpr vector_2d(const vector_2d& other) noexcept = default;
        constexpr vector_2d(vector_2d&& other) noexcept = default;

        constexpr vector_2d(const T value) noexcept
            : x(value), y(value) {
        };

        constexpr vector_2d(const T x_val, const T y_val) noexcept
            : x(x_val), y(y_val) {
        }

        constexpr vector_2d& operator=(const vector_2d& other) noexcept = default;
        constexpr vector_2d& operator=(vector_2d&& other) noexcept = default;

        constexpr void load(const T x_val, const T y_val) noexcept {
            x = x_val;
            y = y_val;
        }

        template<typename E>
        constexpr vector_2d(const vector_expression <E, 2>& expr) noexcept {
            x = expr[0];
            y = expr[1];
        }

        constexpr const T& operator[](const std::size_t i) const {
            switch (i) {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range("vector2D: Index out of range");
            }
        }

        constexpr T& operator[](const std::size_t i) {
            switch (i) {
                case 0: return x;
                case 1: return y;
                default: throw std::out_of_range("vector2D: Index out of range");
            }
        }

        /*
        *  OPERATORS
        */
        template<typename E>
        constexpr vector_2d <T>& operator+=(const vector_expression <E, 2>& expr) noexcept {
            x += expr[0];
            y += expr[1];
            return *this;
        }

        template<typename E>
        constexpr vector_2d <T>& operator-=(const vector_expression <E, 2>& expr) noexcept {
            x -= expr[0];
            y -= expr[1];
            return *this;
        }

        template<typename  E>
        constexpr vector_2d <T>& operator*=(const E& a) noexcept {
            static_assert(neutrino::sdl::math::is_number_v<E>);
            x *= a;
            y *= a;
            return *this;
        }

        template<typename E>
        constexpr std::enable_if_t<is_number_v<E>, vector_2d <T>>& operator/=(const E& a) noexcept {
            x /= a;
            y /= a;
            return *this;
        }

        template<typename E>
        constexpr vector_2d <T>& operator/=(const vector_expression <E, 2>& expr) noexcept {
            x /= expr[0];
            y /= expr[1];
            return *this;
        }

        constexpr T norm2() const noexcept {
            return dot(*this, *this);
        }

        constexpr T norm() const noexcept {
            return std::sqrt(norm2());
        }

        constexpr const vector_2d <T>& unit() noexcept {
            *this /= norm();
            return *this;
        }
    };

    template<typename T, std::size_t N>
    class vector_nd : public vector_expression <vector_nd <T, N>, N> {
        static_assert(neutrino::sdl::math::is_number_v<T>);
        private:
        std::vector <T> data;

        public:
        static constexpr std::size_t size() {
            return N;
        }

        typename std::vector <T>::iterator begin() { return data.begin(); }
        typename std::vector <T>::iterator end() { return data.end(); }

        constexpr vector_nd() noexcept = default;
        constexpr vector_nd(const vector_nd& other) noexcept = default;
        constexpr vector_nd(vector_nd&& other) noexcept = default;

        constexpr vector_nd(const T value) noexcept
            : data(N, value) {
        }

        template<typename... Args>
        constexpr vector_nd(const Args&... args) noexcept
            : data{args...} {
            static_assert(sizeof...(args) == N, "vectorND: Number of arguments does not match the size of the vector.");
        }

        constexpr vector_nd& operator=(const vector_nd& other) noexcept = default;
        constexpr vector_nd& operator=(vector_nd&& other) noexcept = default;

        template<typename... Args>
        constexpr void load(const Args&... args) noexcept {
            static_assert(sizeof...(args) == N, "vectorND: Number of arguments does not match the size of the vector.");
            data = {args...};
        }

        template<typename E>
        constexpr vector_nd(const vector_expression <E, N>& expr) noexcept {
            for (std::size_t i = 0; i < N; ++i)
                data[i] = expr[i];
        }

        constexpr const T& operator[](const std::size_t i) const {
            return data[i];
        }

        constexpr T& operator[](const std::size_t i) {
            return data[i];
        }

        /*
        *  OPERATORS
        */
        template<typename E>
        constexpr vector_nd <T, N>& operator+=(const vector_expression <E, N>& expr) noexcept {
            for (std::size_t i = 0; i < N; ++i)
                data[i] += expr[i];
            return *this;
        }

        template<typename E>
        constexpr vector_nd <T, N>& operator-=(const vector_expression <E, N>& expr) noexcept {
            for (std::size_t i = 0; i < N; ++i)
                data[i] -= expr[i];
            return *this;
        }

        template<typename E>
        constexpr vector_nd <T, N>& operator*=(const E& a) noexcept {
            static_assert(neutrino::sdl::math::is_number_v<E>);
            for (std::size_t i = 0; i < N; ++i)
                data[i] *= a;
            return *this;
        }

        template<typename E>
        constexpr vector_nd <T, N>& operator/=(const E& a) noexcept {
            static_assert(neutrino::sdl::math::is_number_v<E>);
            for (std::size_t i = 0; i < N; ++i)
                data[i] /= a;
            return *this;
        }

        template<typename E>
        constexpr vector_nd <T, N>& operator/=(const vector_expression <E, N>& expr) noexcept {
            for (std::size_t i = 0; i < N; ++i)
                data[i] /= expr[i];
            return *this;
        }

        constexpr T norm2() const noexcept {
            return dot(*this, *this);
        }

        constexpr T norm() const noexcept {
            return std::sqrt(norm2());
        }

        constexpr const vector_nd <T, N>& unit() noexcept {
            *this /= norm();
            return *this;
        }
    };
}
#endif
