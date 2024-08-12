//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_VECTOR_EXPR_HH
#define NEUTRINO_MATH_VECTOR_EXPR_HH

#include <cstddef>
#include <iostream>
#include <math/detail/traits.hh>

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
        //callable f_;

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

    template<class Callable, class LHS>
    class functor_vector_expression : public vector_functor_expression_base {
        detail::storage_type_t <LHS> lhs;

        public:
            using T = element_type_t <LHS>;

            explicit functor_vector_expression([[maybe_unused]] Callable* f, LHS const& lhs_)
                : lhs(lhs_) {
            }

            static constexpr std::size_t size() {
                return LHS::size();
            }

            operator T() const {
                return Callable::template call(lhs);
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

        template<class Callable, class LHS>
        auto make_functor_vector_expr(Callable* callable, LHS&& lhs) {
            constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;
            constexpr auto is_lhs_vec = is_vector_v <LHS>;

            if constexpr (is_lhs_vec && is_lhs_rvalue) {
                return functor_vector_expression{
                    callable,
                    detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs))
                };
            } else {
                return functor_vector_expression{
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

        struct functor_sum {
            template<typename A>
            static auto call(const A& x) {
                using lhs_t = element_type_t <A>;
                lhs_t out = {};
                constexpr std::size_t size = detail::size_traits <std::decay_t <A>>::size();
                for (std::size_t i = 0; i < size; i++) {
                    out += x[i];
                }
                return out;
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

    template<class LHS, class = std::enable_if_t <is_vector_or_vector_exp_v <LHS>>>
    auto sum2(LHS&& lhs) {
        return detail::make_functor_vector_expr(static_cast <vector_ops::functor_sum*>(nullptr), std::forward <LHS>(lhs));;
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
}

#endif
