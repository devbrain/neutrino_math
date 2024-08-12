add_executable(unittest
        main.cc
        #test_vector.cc
        #test_matrix.cc
        test_traits.cc
        test_vector_exp.cc
)

target_link_libraries(unittest neutrino::math doctest asan)
target_compile_options(unittest PRIVATE -Wall -Wextra -pedantic -fsanitize=address -g3)

#target_link_libraries(unittest neutrino::math doctest)
#target_compile_options(unittest PRIVATE -Wall -O3)


