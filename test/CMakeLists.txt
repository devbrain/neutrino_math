add_executable(unittest
        main.cc
        test_vector.cc
)

target_link_libraries(unittest neutrino::math doctest)
