add_library(neutrino_math INTERFACE
        include/math/detail/traits.hh
        include/math/detail/vector_expr.hh
        include/math/detail/vector_functors.hh
        include/math/math.hh
        include/math/vector.hh
        include/math/vector_fwd.hh
        include/math/detail/vector_ops.hh
)

add_library(neutrino::math ALIAS neutrino_math)

target_include_directories(neutrino_math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(neutrino_math INTERFACE box2d)


