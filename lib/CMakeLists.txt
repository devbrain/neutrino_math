add_library(neutrino_math INTERFACE
        include/math/deprecated/detail/vector_expression.hh
        include/math/deprecated/detail/vector_functions.hh
        include/math/deprecated/detail/vector_operations.hh
        include/math/deprecated/vector.hh
        include/math/deprecated/vector_box2d.hh
        include/math/deprecated/detail/matrix_expression.hh
        include/math/deprecated/detail/matrix_operations.hh
        include/math/deprecated/detail/value_holder.hh

        include/math/detail/vector_expr.hh
        include/math/math.hh
        include/math/detail/traits.hh
        include/math/vector_fwd.hh
        include/math/vector.hh
)

add_library(neutrino::math ALIAS neutrino_math)

target_include_directories(neutrino_math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(neutrino_math INTERFACE box2d)


