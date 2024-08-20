//
// Created by igor on 8/14/24.
//

#include <vector>
#include <doctest/doctest.h>
#include <math/math.hh>
#include <math/detail/typo.hpp>

TEST_SUITE("Matrix test suite") {
    TEST_CASE("Test constructor") {
        neutrino::math::matrix <int, 4, 4>
            m({
                {0, 1, 2, 3},
                {4, 5, 6, 7},
                {8, 9, 10, 11},
                {12, 13, 14, 15}
            });
        int k = 0;
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                REQUIRE_EQ(k++, m(r,c));
            }
        }
    }

    TEST_CASE("test views") {
        neutrino::math::vector <int, 2> v{1, 2};
        neutrino::math::matrix <int, 1, 2> m = as_row_vector(v);
        REQUIRE(m(0, 0) == 1);
        REQUIRE(m(0, 1) == 2);
    }

    TEST_CASE("Test transpose") {
        auto m = neutrino::math::make_matrix({
            {0, 1, 2},
            {4, 5, 6},
            {8, 9, 10},
            {12, 13, 14}
        });

        neutrino::math::matrix <int, 3, 4> m2(transpose(m));
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 3; c++) {
                REQUIRE_EQ(m(r,c), m2(c,r));
            }
        }
        auto t = transpose(m);
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 3; c++) {
                REQUIRE_EQ(m(r,c), t(c,r));
            }
        }
    }

    TEST_CASE("Test column/row") {
        neutrino::math::matrix <int, 4, 4>
            m({
                {0, 1, 2, 3},
                {4, 5, 6, 7},
                {8, 9, 10, 11},
                {12, 13, 14, 15}
            });

        neutrino::math::vector <int, 4> row(nth_row(m, 2));
        neutrino::math::vector <int, 4> col(nth_column(m, 2));

        for (std::size_t i = 0; i < 4; i++) {
            REQUIRE_EQ(m(2, i), row[i]);
        }

        for (std::size_t i = 0; i < 4; i++) {
            REQUIRE_EQ(m(i, 2), col[i]);
        }

        neutrino::math::vector <int, 4> col2(nth_column(transpose(m), 2));
        for (std::size_t i = 0; i < 4; i++) {
            REQUIRE_EQ(m(2, i), col2[i]);
        }
    }

    TEST_CASE("Test adjoint") {
        using zt = std::complex <float>;
        zt z_0_1{-2, -1};
        zt z_1_0{1, 1};
        zt z_1_1{0, 1};
        zt z_1_2{4, -2};
        neutrino::math::matrix <zt, 2, 3>
            m({
                {zt(1, 0), z_0_1, zt(5, 0)},
                {z_1_0, z_1_1, z_1_2}
            });
        neutrino::math::matrix <zt, 3, 2> a = adjoint(m);
        for (std::size_t r = 0; r < 3; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                REQUIRE_EQ(a(r, c), std::conj(m(c, r)));
            }
        }
    }

    TEST_CASE("Test min/max/sum") {
        auto m = neutrino::math::make_matrix({
            {1, 2},
            {4, 5},
        });
        REQUIRE_EQ(sum(m), 1+2+4+5);
        REQUIRE_EQ(min(m), 1);
        REQUIRE_EQ(max(m), 5);
        REQUIRE_EQ(trace(m), 1+5);
    }

    TEST_CASE("Test inv") {
        neutrino::math::matrix <float, 2, 2>
            m({
                {1, 2},
                {4, 5},
            });
        neutrino::math::matrix <float, 2, 2> m2(inv(m));

        for (std::size_t r = 0; r < 2; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                REQUIRE_EQ(doctest::Approx(m2(r, c)), 1.0/(m(r, c)));
            }
        }
    }

    TEST_CASE("Test plus") {
        neutrino::math::matrix <float, 2, 2>
            m({
                {1, 2},
                {4, 5},
            });
        neutrino::math::matrix <float, 2, 2> m2(m + neutrino::math::identity <float, 2, 2>());

        for (std::size_t r = 0; r < 2; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                if (r == c) {
                    REQUIRE_EQ(m2(r, c), m(r, c) + 1);
                } else {
                    REQUIRE_EQ(m2(r, c), m(r, c));
                }
            }
        }

        neutrino::math::matrix <float, 2, 2> m3 = 10 + m + 1;
        for (std::size_t r = 0; r < 2; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                REQUIRE_EQ(m3(r, c), m(r, c) + 11);
            }
        }
    }

    TEST_CASE("Test minus") {
        neutrino::math::matrix <float, 2, 2>
            m({
                {1, 2},
                {4, 5},
            });
        neutrino::math::matrix <float, 2, 2> m2(m - neutrino::math::identity <float, 2, 2>());

        for (std::size_t r = 0; r < 2; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                if (r == c) {
                    REQUIRE_EQ(m2(r, c), m(r, c) - 1);
                } else {
                    REQUIRE_EQ(m2(r, c), m(r, c));
                }
            }
        }

        neutrino::math::matrix <float, 2, 2> m3 = 10 - m - 1;
        for (std::size_t r = 0; r < 2; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                REQUIRE_EQ(m3(r, c), 10 - m(r, c) - 1);
            }
        }
    }

    TEST_CASE("Test mul") {
        neutrino::math::matrix <float, 2, 2>
            m({
                {1, 2},
                {4, 5},
            });
        neutrino::math::matrix <float, 2, 2> m2(m - 4 * neutrino::math::identity <float, 2, 2>());

        for (std::size_t r = 0; r < 2; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                if (r == c) {
                    REQUIRE_EQ(m2(r, c), m(r, c) - 4);
                } else {
                    REQUIRE_EQ(m2(r, c), m(r, c));
                }
            }
        }

        neutrino::math::matrix <float, 2, 2> m3 = 10 * m * 2;
        for (std::size_t r = 0; r < 2; r++) {
            for (std::size_t c = 0; c < 2; c++) {
                REQUIRE_EQ(m3(r, c), 20* m(r, c));
            }
        }
    }

    TEST_CASE("Test trace") {
        auto tr = trace(5 * neutrino::math::identity <float, 3, 3>());
        REQUIRE(tr == 15);
    }

    TEST_CASE("Test sum") {
        neutrino::math::matrix <int, 4, 3>
            m({
                {0, 1, 2},
                {4, 5, 6},
                {8, 9, 10},
                {12, 13, 14}
            });
        auto tr = sum(m - m);
        REQUIRE(tr == 0);
    }

    TEST_CASE("Test matrix multiplication") {
        neutrino::math::matrix <int, 2, 3>
            m1({
                {1, 2, 3},
                {4, 5, 6}
            });

        neutrino::math::matrix <int, 3, 2>
            m2({
                {7, 8},
                {9, 10},
                {11, 12}
            });
        neutrino::math::matrix <int, 2, 2> m3 = dot(m1, m2);

        REQUIRE_EQ(58, m3(0,0));
        REQUIRE_EQ(64, m3(0,1));
        REQUIRE_EQ(139, m3(1,0));
        REQUIRE_EQ(154, m3(1,1));
    }

    TEST_CASE("Test matrix crtp ops") {
        neutrino::math::matrix <int, 2, 3>
            m1({
                {1, 2, 3},
                {4, 5, 6}
            });

        neutrino::math::matrix <int, 2, 3>
            m2({
                {2 * 1, 2 * 2, 2 * 3},
                {2 * 4, 2 * 5, 2 * 6}
            });

        m1 *= 2;
        REQUIRE_EQ(m1, m2);
    }

    TEST_CASE("test access") {
        neutrino::math::matrix <int, 4, 4>
            m({
                {0, 1, 2, 3},
                {4, 5, 6, 7},
                {8, 9, 10, 11},
                {12, 13, 14, 15}
            });
        neutrino::math::matrix <int, 4, 4>
            m2({
                {0, 1, 2 - 10, 3},
                {4, 5, 6 - 10, 7},
                {8, 9, 10 - 10, 11},
                {12, 13, 14 - 10, 15}
            });
        auto cols = columns_tuple(m);
        auto c2 = std::get <2>(cols);
        double n = norm2(c2);
        REQUIRE(doctest::Approx(n) == 2*2+6*6+10*10 + 14*14);

        c2 -= 10;

        REQUIRE_EQ(m2, m);

        auto c3 = std::get <3>(cols);

        c3 = abs(c2);
        for (std::size_t i = 0; i < m.get_rows_num(); i++) {
            REQUIRE_EQ(m(i, 3), std::abs(m(i, 2)));
        }

        neutrino::math::matrix <int, 4, 4> m3{};
        m3 = c3;
        for (std::size_t i = 0; i < m.get_rows_num(); i++) {
            REQUIRE_EQ(m3(i, 3), m(i, 3));
        }

        for (const auto c : columns(m)) {
            for (std::size_t i = 0; i < m.get_rows_num(); i++) {
                REQUIRE_EQ(c[i], m(i, c.get_index()));
            }
        }
    }
}

