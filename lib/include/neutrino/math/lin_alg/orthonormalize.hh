//
// Created by igor on 8/21/24.
//

#ifndef MATH_LIN_ALG_ORTHONORMALIZE_HH
#define MATH_LIN_ALG_ORTHONORMALIZE_HH

#include <neutrino/math/matrix.hh>
#include <neutrino/math/detail/vector_ops.hh>
#include <neutrino/math/lin_alg/projection.hh>

namespace neutrino::math {
    template <typename T, std::size_t Rows, std::size_t Cols>
    matrix<T, Rows, Cols> orthonormalize(const matrix<T, Rows, Cols>& basis) {
        matrix<T, Rows, Cols> res(basis);
        for (std::size_t i = 1; i<Cols; ++i) {
            auto bi = nth_column(basis, i);
            vector<T, Rows> projection = bi;
            for (std::size_t j = 0; j < i; ++j) {
                auto rj = nth_column(res, j);
                projection -= proj(rj, bi);
            }
            res.set_column(projection, i);
        }
        for (std::size_t i = 0; i<Cols; ++i) {
            res.set_column(normalize(nth_column(res, i)), i);
        }
        return res;
    }
}

#endif
