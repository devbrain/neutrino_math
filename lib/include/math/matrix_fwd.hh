//
// Created by igor on 8/14/24.
//

#ifndef NEUTRINO_MATH_MATRIX_FWD_HH
#define NEUTRINO_MATH_MATRIX_FWD_HH

#include <cstddef>

namespace neutrino::math {
    template<typename E, std::size_t R, std::size_t C>
    class matrix;

    template<std::size_t R, std::size_t C>
    struct matrix_expression_base {

        static constexpr std::size_t rows() {
            return R;
        }

        static constexpr std::size_t columns() {
            return C;
        }
    };


}


#endif
