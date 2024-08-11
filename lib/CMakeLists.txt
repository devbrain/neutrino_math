add_library(neutrino_math INTERFACE
        include/math/detail/vector_expression.hh
        include/math/detail/vector_functions.hh
        include/math/detail/vector_operations.hh
        include/math/vector.hh
)

add_library(neutrino::math ALIAS neutrino_math)

target_include_directories(neutrino_math INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(neutrino_math INTERFACE box2d)


