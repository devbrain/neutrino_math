add_executable(unittest
        main.cc
        test_traits.cc
        test_vector_exp.cc
        test_matrix.cc
        test_lin_alg.cc
)

target_link_libraries(unittest neutrino::math doctest asan)
target_compile_options(unittest PRIVATE -Wall -Wextra -pedantic -fsanitize=address -g3)

#target_link_libraries(unittest neutrino::math doctest)
#target_compile_options(unittest PRIVATE -Wall -O3)


