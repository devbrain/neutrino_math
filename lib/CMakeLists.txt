add_library(neutrino_math INTERFACE
        include/math/detail/traits.hh
        include/math/detail/vector_expr.hh
        include/math/detail/vector_functors.hh
        include/math/math.hh
        include/math/vector.hh
        include/math/vector_fwd.hh
        include/math/detail/vector_ops.hh
        include/math/matrix_fwd.hh
        include/math/matrix.hh
        include/math/detail/array_utils.hh
        include/math/detail/matrix_expr.hh
        include/math/detail/matrix_ops.hh
        include/math/detail/matrix_traits.hh
        include/math/detail/scalar.hh
        include/math/lin_alg/projection.hh
        include/math/detail/matrix_access.hh
        include/math/detail/typo.hpp
        include/math/detail/matrix_storage.hh
        include/math/detail/matrix_iterator.hh
        include/math/detail/make_vector.hh
        include/math/detail/make_matrix.hh
        include/math/detail/vector_temp_value.hh
        include/math/detail/matrix_temp_value.hh
        include/math/detail/temp_value.hh
)

add_library(neutrino::math ALIAS neutrino_math)

target_include_directories(neutrino_math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(neutrino_math INTERFACE box2d)


