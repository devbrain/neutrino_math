//
// Created by igor on 8/20/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MAKE_MATRIX_HH
#define NEUTRINO_MATH_DETAIL_MAKE_MATRIX_HH

#include <math/matrix.hh>

namespace neutrino::math {
    template<typename E, std::size_t R, std::size_t C>
    auto make_matrix(const E (&& data)[R][C]) {
        return matrix <E, R, C>{std::forward <decltype(data)>(data)};
    }
}

#endif
