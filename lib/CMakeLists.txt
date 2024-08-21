add_library(neutrino_math INTERFACE
        include/neutrino/math/detail/traits.hh
        include/neutrino/math/detail/vector_expr.hh
        include/neutrino/math/detail/vector_functors.hh
        include/neutrino/math/math.hh
        include/neutrino/math/vector.hh
        include/neutrino/math/vector_fwd.hh
        include/neutrino/math/detail/vector_ops.hh
        include/neutrino/math/matrix_fwd.hh
        include/neutrino/math/matrix.hh
        include/neutrino/math/detail/array_utils.hh
        include/neutrino/math/detail/matrix_expr.hh
        include/neutrino/math/detail/matrix_ops.hh
        include/neutrino/math/detail/matrix_traits.hh
        include/neutrino/math/detail/scalar.hh
        include/neutrino/math/lin_alg/projection.hh
        include/neutrino/math/detail/matrix_access.hh
        include/neutrino/math/detail/matrix_storage.hh
        include/neutrino/math/detail/matrix_iterator.hh
        include/neutrino/math/detail/make_vector.hh
        include/neutrino/math/detail/make_matrix.hh
        include/neutrino/math/detail/vector_temp_value.hh
        include/neutrino/math/detail/matrix_temp_value.hh
        include/neutrino/math/detail/temp_value.hh
        include/neutrino/math/lin_alg/orthonormalize.hh
        include/neutrino/math/lin_alg/qr_decompose.hh
        include/neutrino/math/lin_alg/determinant.hh
)

add_library(neutrino::math ALIAS neutrino_math)
target_include_directories(neutrino_math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)



