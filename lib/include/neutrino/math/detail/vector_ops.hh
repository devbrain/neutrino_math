//
// Created by igor on 8/13/24.
//

#ifndef NEUTRINO_MATH_VECTOR_OPS_HH
#define NEUTRINO_MATH_VECTOR_OPS_HH

#include <iostream>
#include <sstream>
#include <neutrino/math/detail/vector_functors.hh>
#include <neutrino/math/detail/vector_expr.hh>
#include <neutrino/math/detail/scalar.hh>

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

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>, op_vector>>
    auto conj(LHS&& lhs) {
        return detail::make_unary_vector_expr(static_cast <vector_fn::unary_conjugate*>(nullptr),
                                              std::forward <LHS>(lhs));
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
        auto out = lhs[0];
        constexpr std::size_t size = size_v <LHS>;
        for (std::size_t i = 1; i < size; i++) {
            out += lhs[i];
        }
        return out;
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto prod(LHS&& lhs) {
        auto out = lhs[0];
        constexpr std::size_t size = size_v <LHS>;
        for (std::size_t i = 1; i < size; i++) {
            out *= lhs[i];
        }
        return out;
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto max(LHS&& lhs) {
        auto out = lhs[0];
        constexpr std::size_t size = size_v <LHS>;
        for (std::size_t i = 1; i < size; i++) {
            if (lhs[i] > out) {
                out = lhs[i];
            }
        }
        return out;
    }

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto min(LHS&& lhs) {
        auto out = lhs[0];
        constexpr std::size_t size = size_v <LHS>;
        for (std::size_t i = 1; i < size; i++) {
            if (lhs[i] < out) {
                out = lhs[i];
            }
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
        return detail::make_unary_vector_expr(static_cast <vector_fn::PPCAT(unary_, NAME)*>(nullptr), std::forward <LHS>(lhs));  \
    }                                                                                                                            \
                                                                                                                                 \
    template<class LHS>                                                                                                          \
    auto NAME(LHS&& lhs, std::enable_if_t <is_scalar_v <LHS>>* = nullptr) {                                                      \
        return detail::make_unary_vector_expr(static_cast <vector_fn::PPCAT(unary_, NAME)*>(nullptr), std::forward <LHS>(lhs));  \
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
    template<typename Derived, std::size_t N>
    struct generic_vector_ops;

    template<typename Vec, std::size_t N>
    class vector_iterator {
        friend struct generic_vector_ops <Vec, N>;

        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = typename Vec::value_type;
            using pointer = value_type*;
            using reference = value_type&;

            reference operator*() const { return m_vec[m_ptr]; }

            // Prefix increment
            vector_iterator& operator++() {
                m_ptr++;
                return *this;
            }

            // Postfix increment
            vector_iterator operator++(int) {
                auto tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const vector_iterator& a, const vector_iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!=(const vector_iterator& a, const vector_iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            vector_iterator(std::size_t idx, Vec& vec)
                : m_vec(vec), m_ptr(idx) {
            }

            Vec& m_vec;
            std::size_t m_ptr;
    };

    template<typename Vec, std::size_t N>
    class const_vector_iterator {
        friend struct generic_vector_ops <Vec, N>;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = typename Vec::value_type;
            using reference = const value_type&;

            reference operator*() const { return m_vec[m_ptr]; }

            // Prefix increment
            const_vector_iterator& operator++() {
                m_ptr++;
                return *this;
            }

            // Postfix increment
            const_vector_iterator operator++(int) {
                auto tmp = *this;
                ++(*this);
                return tmp;
            }

            friend bool operator==(const const_vector_iterator& a, const const_vector_iterator& b) {
                return a.m_ptr == b.m_ptr;
            };

            friend bool operator!=(const const_vector_iterator& a, const const_vector_iterator& b) {
                return a.m_ptr != b.m_ptr;
            };

        private:
            const_vector_iterator(std::size_t idx, const Vec& vec)
                : m_vec(vec), m_ptr(idx) {
            }

            const Vec& m_vec;
            std::size_t m_ptr;
    };

    template<typename Derived, std::size_t N>
    struct generic_vector_ops {
        auto operator()(std::size_t idx) const {
            return get(idx);
        }

        template <typename ...E>
        void load(const E&... args) {
            int i = 0;
            ([&]
            {
                self()->operator[](i++) = args;
            } (), ...);
        }

        const_vector_iterator <Derived, N> begin() const {
            return const_vector_iterator <Derived, N>(0, *self());
        }

        const_vector_iterator <Derived, N> end() const {
            return const_vector_iterator <Derived, N>(N, *self());
        }

        const_vector_iterator <Derived, N> cbegin() const {
            return const_vector_iterator <Derived, N>(0, *self());
        }

        const_vector_iterator <Derived, N> cend() const {
            return const_vector_iterator <Derived, N>(N, *self());
        }

        vector_iterator <Derived, N> begin() {
            return vector_iterator <Derived, N>(0, *self());
        }

        vector_iterator <Derived, N> end() {
            return vector_iterator <Derived, N>(N, *self());
        }

        // ---------------------------------------------------------------------------------
        // Equals
        // ---------------------------------------------------------------------------------
        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        [[nodiscard]] bool equals(const T& other) const {
            for (std::size_t i = 0; i < Derived::size(); i++) {
                if (!detail::scalar::equals(get(i), subscript(other, i))) {
                    return false;
                }
            }
            return true;
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        [[nodiscard]] int compare(const T& other) const {
            for (std::size_t i = 0; i < Derived::size(); i++) {
                const auto oth = subscript(other, i);
                if (get(i) < oth) {
                    return -1;
                } else if (get(i) > oth) {
                    return 1;
                }
            }
            return 0;
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        friend bool operator ==(const Derived& a, const T& b) {
            return a.equals(b);
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        friend bool operator ==(const T& a, const Derived& b) {
            return a.equals(b);
        }

        friend bool operator ==(const Derived& a, const Derived& b) {
            return a.equals(b);
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        friend bool operator !=(const Derived& a, const T& b) {
            return !a.equals(b);
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        friend bool operator !=(const T& a, const Derived& b) {
            return !a.equals(b);
        }

        friend bool operator !=(const Derived& a, const Derived& b) {
            return !a.equals(b);
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        Derived& operator +=(T&& a) {
            for (std::size_t i = 0; i < Derived::size(); i++) {
                get(i) += subscript(a, i);
            }
            return *self();
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        Derived& operator -=(T&& a) {
            for (std::size_t i = 0; i < Derived::size(); i++) {
                get(i) -= subscript(a, i);
            }
            return *self();
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        Derived& operator *=(T&& a) {
            for (std::size_t i = 0; i < Derived::size(); i++) {
                get(i) *= subscript(a, i);
            }
            return *self();
        }

        template<typename T, class = std::enable_if_t <
                     (is_vector_or_vector_exp_v <T> && size_v <T> == N) || is_scalar_v <T>>>
        Derived& operator /=(T&& a) {
            for (std::size_t i = 0; i < Derived::size(); i++) {
                get(i) /= subscript(a, i);
            }
            return *self();
        }

        [[nodiscard]] std::string to_string() const {
            std::ostringstream os;
            os << "[";
            for (std::size_t i = 0; i < Derived::size(); i++) {
                if (i > 0) {
                    os << ", ";
                }
                os << get(i);
            }
            os << "]";
            return os.str();
        }

        friend std::ostream& operator <<(std::ostream& os, const Derived& a) {
            os << a.to_string();
            return os;
        }

        protected:
            template<class operand>
            constexpr auto subscript(operand const& v, std::size_t const i) const {
                if constexpr (!is_scalar_v <operand>) {
                    return v[i];
                } else {
                    return v;
                }
            }

            const auto& get(std::size_t idx) const {
                return self()->operator[](idx);
            }

            auto& get(std::size_t idx) {
                return self()->operator[](idx);
            }

            Derived* self() {
                return static_cast <Derived*>(this);
            }

            const Derived* self() const {
                return static_cast <const Derived*>(this);
            }


    };

    template<typename Elem, std::size_t N>
    struct vector_ops : generic_vector_ops <vector <Elem, N>, N> {
    };
}

#if defined(PPCAT_NX)
#   undef PPCAT_NX
#endif
#if defined(PPCAT)
#   undef PPCAT
#endif

#endif
