//
// Created by igor on 8/20/24.
//

#ifndef NEUTRINO_MATH_DETAIL_MAKE_MATRIX_HH
#define NEUTRINO_MATH_DETAIL_MAKE_MATRIX_HH

#include <math/matrix.hh>
#include <math/detail/matrix_ops.hh>
namespace neutrino::math {
    template<typename E, std::size_t R, std::size_t C>
    auto make_matrix(const E (&& data)[R][C]) {
        return matrix <E, R, C>{std::forward <decltype(data)>(data)};
    }

    template<std::size_t C, typename ...E, class = std::enable_if_t<is_all_same_v<E...>>>
    auto make_matrix_from_rows(const vector <E, C>& ... rows) {
        constexpr auto R = sizeof...(rows);
        return detail::make_from_rows_proxy<R, C, first_type_t<E...>, E...>(rows... );
    }

    template<std::size_t C, typename ...E, class = std::enable_if_t<is_all_same_v<E...>>>
    auto make_matrix_from_cols(const vector <E, C>& ... rows) {
        constexpr auto R = sizeof...(rows);
        return transpose(detail::make_from_rows_proxy<R, C, first_type_t<E...>, E...>(rows... ));
    }

}

#endif
