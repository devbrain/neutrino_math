//
// Created by igor on 8/18/24.
//

#ifndef NEUTRINO_MATH_LIN_ALG_PROJECTION_HH
#define NEUTRINO_MATH_LIN_ALG_PROJECTION_HH

#include <math/vector.hh>

namespace neutrino::math {
    /**
     * Project the vector vec onto the line spanned by the vector e
     **/
    template<typename A, typename B>
    auto proj(A&& e, B&& vec) {
        auto d1 = dot(std::forward <A>(e), std::forward <B>(vec));
        auto d2 = dot(std::forward <A>(e), std::forward <A>(e));
        return (d1/d2) * std::forward <A>(e);
    }
}

#endif
