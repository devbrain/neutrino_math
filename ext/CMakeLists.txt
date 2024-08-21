include(FetchContent)

set(FETCHCONTENT_QUIET off)

if (NEUTRINO_MATH_BUILD_TEST)
    add_subdirectory(doctest)
endif()






