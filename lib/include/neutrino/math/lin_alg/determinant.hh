//
// Created by igor on 8/21/24.
//

#ifndef NEUTRINO_MATRIX_LIN_ALG_DETERMINANT_HH
#define NEUTRINO_MATRIX_LIN_ALG_DETERMINANT_HH

#include <neutrino/math/lin_alg/orthonormalize.hh>

namespace neutrino::math {
    template <typename T, std::size_t N>
    T abs_determinant (const matrix<T, N, N>& m) {
        if constexpr (N == 1) {
            return std::abs(m(0,0));
        } else if constexpr (N == 2) {
            return std::abs(m(0,0) * m(1,1) - m(1,0) * m(0,1));
        } else {
            auto q = orthonormalize(m);
            T d = dot(nth_column(q, 0), nth_column(m, 0));
            for (std::size_t col = 1; col < N; ++col) {
                d *= dot(nth_column(q, col), nth_column(m, col));
            }
            return d;
        }
    }
}

#endif
