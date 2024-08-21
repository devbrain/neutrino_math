//
// Created by igor on 8/21/24.
//

#ifndef NEUTRINO_MATH_LIN_ALG_QR_DECOMPOSE_HH
#define NEUTRINO_MATH_LIN_ALG_QR_DECOMPOSE_HH

#include <neutrino/math/lin_alg/orthonormalize.hh>

namespace neutrino::math {
    template<typename T, std::size_t N>
    std::tuple <matrix <T, N, N>, matrix <T, N, N>> qr_decompose(const matrix <T, N, N>& m) {
        auto q = orthonormalize(m);
        matrix <T, N, N> r{};
        for (std::size_t col = 0; col < N; ++col) {
            for (std::size_t row = 0; row <= col; ++row) {
                r(row, col) = dot(nth_column(q, row), nth_column(m, col));
            }
        }
        return std::make_tuple(std::move(q), std::move(r));
    }
}

#endif
