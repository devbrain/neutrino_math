//
// Created by igor on 8/11/24.
//

#ifndef NEUTRINO_MATH_VECTOR_EXPR_HH
#define NEUTRINO_MATH_VECTOR_EXPR_HH

#include <cstddef>
#include <math/detail/traits.hh>
#include <math/detail/vector_functors.hh>

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

    template<typename T>
    struct size_traits <detail::temp_value_holder <T>> {
        static constexpr size_t size() {
            return T::size();
        }
    };

    namespace detail {
        template<class LHS, class RHS>
        constexpr bool is_compatible_size_v = size_v <LHS> == size_v <RHS>;

        template<class Callable, class LHS, class RHS>
        auto make_binary_vector_expr(Callable* callable, LHS&& lhs, RHS&& rhs) {
            constexpr auto is_rhs_rvalue = std::is_rvalue_reference_v <decltype(rhs)>;
            constexpr auto is_lhs_rvalue = std::is_rvalue_reference_v <decltype(lhs)>;

            constexpr auto is_rhs_vec = is_vector_v <RHS>;
            constexpr auto is_lhs_vec = is_vector_v <LHS>;

            constexpr auto left_is_temp = is_lhs_rvalue && is_lhs_vec;
            constexpr auto right_is_temp = is_rhs_rvalue && is_rhs_vec;
            if constexpr (left_is_temp && right_is_temp) {
                return binary_vector_expression{
                    callable,
                    detail::temp_value_holder <std::decay_t <LHS>>(std::forward <LHS>(lhs)),
                    detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
                };
            } else if constexpr (!left_is_temp && right_is_temp) {
                return binary_vector_expression{
                    callable,
                    lhs,
                    detail::temp_value_holder <std::decay_t <RHS>>(std::forward <RHS>(rhs))
                };
            } else if constexpr (left_is_temp) {
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


}

#endif
