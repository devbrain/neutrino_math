//
// Created by igor on 8/21/24.
//

#ifndef NEUTRINO_MATH_MATRIX_TEMP_VALUE_HH
#define NEUTRINO_MATH_MATRIX_TEMP_VALUE_HH

#include <cstddef>
#include <utility>

#include <neutrino/math/detail/traits.hh>

namespace neutrino::math::detail {
    template<typename T>
    class matrix_temp_value_holder {
        private:
            T value;

        public:
            matrix_temp_value_holder(matrix_temp_value_holder&& other) noexcept
                : value(std::move(other.value)) {
            }

            matrix_temp_value_holder(const matrix_temp_value_holder& other)
                : value(std::move(const_cast <matrix_temp_value_holder&>(other).value)) {
            }

            constexpr explicit matrix_temp_value_holder(T&& v)
                : value(std::move(v)) {
            }

            constexpr auto& operator()(std::size_t r, std::size_t c) {
                return value(r, c);
            }

            constexpr const auto& operator()(std::size_t r, std::size_t c) const {
                return value(r, c);
            }
    };
}

#endif
