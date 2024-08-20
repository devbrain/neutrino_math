//
// Created by igor on 8/20/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MATRIX_STORAGE_HH
#define NEUTRINO_MATH_DETAIL_MATRIX_STORAGE_HH

#include <math/vector.hh>

namespace neutrino::math::detail {
    template<typename E, std::size_t R, std::size_t C>
    struct matrix_storage {
        using row_t = vector <E, C>;
        using values_t = std::array <row_t, R>;

        template<int... I>
        static constexpr values_t create(const E (& data)[R][C], std::integer_sequence <int, I...>) {
            return {row_t{data[I]}...};
        }
    };
}

#endif
