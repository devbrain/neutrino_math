//
// Created by igor on 8/21/24.
//

#ifndef NEUTRINO_MATH_DETAIL_VECTOR_TEMP_VALUE_HH
#define NEUTRINO_MATH_DETAIL_VECTOR_TEMP_VALUE_HH

#include <cstddef>
#include <utility>

#include <neutrino/math/detail/traits.hh>

namespace neutrino::math::detail {
    template<typename T>
    class vector_temp_value_holder {
        private:
            T value;

        public:
            vector_temp_value_holder(vector_temp_value_holder&& other) noexcept
                : value(std::move(other.value)) {
            }

            vector_temp_value_holder(const vector_temp_value_holder& other)
                : value(std::move(const_cast <vector_temp_value_holder&>(other).value)) {
            }

            constexpr explicit vector_temp_value_holder(T&& v)
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



#endif
