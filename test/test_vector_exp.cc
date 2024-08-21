//
// Created by igor on 8/12/24.
//
#include <doctest/doctest.h>
#include <neutrino/math/math.hh>

// #include <vector>
// #include <chrono>
// #include <utility>
// #include <iostream>
//
// typedef std::chrono::high_resolution_clock::time_point TimeVar;
//
// #define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
// #define timeNow() std::chrono::high_resolution_clock::now()
//
// template<typename F, typename... Args>
// double funcTime(F func, Args&&... args) {
//     TimeVar t1 = timeNow();
//     func(std::forward <Args>(args)...);
//     return duration(timeNow()-t1);
// }
//
// static constexpr auto VEC_SIZE = 100;
// static constexpr auto REPS = 1000;
//
// static void simple_for(const std::vector <int>& x) {
//     int out = {};
//     for (std::size_t i = 0; i < VEC_SIZE; i++) {
//         out += x[i];
//     }
// }
//
// template<auto Start, auto End, auto Inc, class F>
// constexpr void constexpr_for(F&& f) {
//     if constexpr (Start < End) {
//         f(std::integral_constant <decltype(Start), Start>());
//         constexpr_for <Start + Inc, End, Inc>(f);
//     }
// }
//
// static void constexpr_for_driver(const std::vector <int>& x) {
//     for (int i = 0; i < REPS; i++) {
//         int out = {};
//         constexpr_for <0, VEC_SIZE, 1>([&out, x](auto i) {
//             out += x[i];
//         });
//     }
// }
//
// static void simple_for_driver(const std::vector <int>& x) {
//     for (int i = 0; i < REPS; i++) {
//         simple_for(x);
//     }
// }

static auto makeExpr1(neutrino::math::vector <int, 3> const& v0, neutrino::math::vector <int, 3> const& v1,
                      neutrino::math::vector <int, 3> const& v2) {
    return v0 + v1 + v2;
}

static auto makeExpr2(neutrino::math::vector <int, 3> const& v0, neutrino::math::vector <int, 3> const& v1) {
    return v0 + v1 + neutrino::math::vector <int, 3>(1, 1, 1);
}

static auto makeExpr3(neutrino::math::vector <int, 3> const& v0, neutrino::math::vector <int, 3> const& v1) {
    return neutrino::math::vector <int, 3>(10, 100, 1000) + v0 + v1 + neutrino::math::vector <int, 3>(1, 1, 1);
}

TEST_SUITE("test vector exp") {
    TEST_CASE("test construct") {
        neutrino::math::vector<int, 5> v(10);
        for (int i = 0; i<5; i++) {
            REQUIRE(v[i] == 10);
        }
        neutrino::math::vector<int, 6> v1(1, 2, 3, 4, 5, 6);
        for (int i = 0; i<5; i++) {
            REQUIRE(v1[i] == i+1);
        }
    }

    TEST_CASE("test make") {
        using t1 = neutrino::math::vector<int, 6>;

        auto v1 = neutrino::math::make_vector(1, 2, 3, 4, 5, 6);
        static_assert(std::is_same_v<decltype(v1), t1>);

        for (int i = 0; i<5; i++) {
            REQUIRE(v1[i] == i+1);
        }
    }

    TEST_CASE("test trivial") {
        //std::initializer_list<int> args = {1, 2, 3};
        const neutrino::math::vector <int, 3> v1{1, 2, 3};;
        const neutrino::math::vector <int, 3> v2(10, 12, 13);
        const neutrino::math::vector <int, 3> v3 = v1 + v2;

        REQUIRE_EQ(v3.x, 11);
        REQUIRE_EQ(v3.y, 14);
        REQUIRE_EQ(v3.z, 16);

        std::array<int, 3> a = {11, 14, 16};
        int i = 0;
        for (auto x : v3) {
            REQUIRE(x == a[i++]);
        }

        REQUIRE_EQ(max(v3), 16);
        REQUIRE_EQ(min(v3), 11);
    }

    TEST_CASE("test temp") {
        const neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        const neutrino::math::vector <int, 3> v1 = {11, 21, 31};
        const neutrino::math::vector <int, 3> v2 = {12, 22, 32};

        auto exp1 = makeExpr1(v0, v1, v2);
        const neutrino::math::vector <int, 3> v1_ = exp1;
        REQUIRE_EQ(v1_.x, 33);
        REQUIRE_EQ(v1_.y, 63);
        REQUIRE_EQ(v1_.z, 93);
        auto exp = makeExpr2(v0, v1);
        const neutrino::math::vector <int, 3> v2_ = exp;
        REQUIRE_EQ(v2_.x, 22);
        REQUIRE_EQ(v2_.y, 42);
        REQUIRE_EQ(v2_.z, 62);

        auto exp2 = makeExpr3(v0, v1);
        const neutrino::math::vector <int, 3> v3_ = exp2;
        REQUIRE_EQ(v3_.x, 22+10);
        REQUIRE_EQ(v3_.y, 42+100);
        REQUIRE_EQ(v3_.z, 62+1000);
    }

    TEST_CASE("test + scalar") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        const neutrino::math::vector <int, 3> v = v0 + 1;

        REQUIRE_EQ(v.x, 11);
        REQUIRE_EQ(v.y, 21);
        REQUIRE_EQ(v.z, 31);

        int z = 2;

        const neutrino::math::vector <int, 3> v1 = z + v0 + 1;

        REQUIRE_EQ(v1.x, 11+2);
        REQUIRE_EQ(v1.y, 21+2);
        REQUIRE_EQ(v1.z, 31+2);
    }

    TEST_CASE("test negate") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        const neutrino::math::vector <int, 3> v = v0 + neutrino::math::negate(v0);

        REQUIRE_EQ(v.x, 0);
        REQUIRE_EQ(v.y, 0);
        REQUIRE_EQ(v.z, 0);
    }

    TEST_CASE("test - ") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        const neutrino::math::vector <int, 3> v = v0 - 1;

        REQUIRE_EQ(v.x, 9);
        REQUIRE_EQ(v.y, 19);
        REQUIRE_EQ(v.z, 29);

        int z = 2;

        const neutrino::math::vector <int, 3> v1 = z + v0 + 1 - v;

        REQUIRE_EQ(v1.x, 11+2-9);
        REQUIRE_EQ(v1.y, 21+2-19);
        REQUIRE_EQ(v1.z, 31+2-29);
    }

    TEST_CASE("test * ") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        const neutrino::math::vector <int, 3> v = 2 * v0;

        REQUIRE_EQ(v.x, 2*10);
        REQUIRE_EQ(v.y, 2*20);
        REQUIRE_EQ(v.z, 2*30);

        int z = 3;

        const neutrino::math::vector <int, 3> v1 = v0 * z;

        REQUIRE_EQ(v1.x, z*10);
        REQUIRE_EQ(v1.y, z*20);
        REQUIRE_EQ(v1.z, z*30);

        const neutrino::math::vector <int, 3> v2 = 7 * v0 * z;

        REQUIRE_EQ(v2.x, 7*z*10);
        REQUIRE_EQ(v2.y, 7*z*20);
        REQUIRE_EQ(v2.z, 7*z*30);

        constexpr neutrino::math::vector <int, 3> v4 = {5, 15, 25};
        const neutrino::math::vector <int, 3> v5 = v1 * v4;
        REQUIRE_EQ(v5.x, v1.x*v4.x);
        REQUIRE_EQ(v5.y, v1.y*v4.y);
        REQUIRE_EQ(v5.z, v1.z*v4.z);
    }

    TEST_CASE("test / ") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        const neutrino::math::vector <int, 3> v = v0 / 10;

        REQUIRE_EQ(v.x, 1);
        REQUIRE_EQ(v.y, 2);
        REQUIRE_EQ(v.z, 3);

        const neutrino::math::vector <int, 3> v1 = v0 / v;

        REQUIRE_EQ(v1.x, 10);
        REQUIRE_EQ(v1.y, 10);
        REQUIRE_EQ(v1.z, 10);
    }

    TEST_CASE("test sum") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        auto s0 = sum(v0);
        REQUIRE_EQ(10+20+30, s0);
    }

    TEST_CASE("test dot") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        constexpr neutrino::math::vector <int, 3> v1 = {1, 2, 3};
        auto s0 = dot(v0, v1);
        REQUIRE_EQ(10*1+20*2+30*3, s0);

        auto s1 = dot(v0, v1 * 3);
        REQUIRE_EQ(3*(10*1+20*2+30*3), s1);
    }

    TEST_CASE("test norm") {
        neutrino::math::vector<double, 3> v;
        v.load(1, 2, 0);
        REQUIRE_EQ(std::sqrt(5), norm(v));

        v.load(-10, 2.5, 0);
        REQUIRE_EQ(std::sqrt(100 + 2.5 * 2.5), norm(v));

        v.load(14, -14, 0);
        REQUIRE_EQ(14 * std::sqrt(2), norm(v));

        v.load(0, -2, 0);
        REQUIRE_EQ(2, norm(v));

        v.load(1, 0, 0);
        REQUIRE_EQ(1, norm(v));

        v.load(1, -1, 0);
        REQUIRE_EQ(14 * std::sqrt(2), norm(v*14));
    }

    TEST_CASE("Angle") {
        neutrino::math::vector <double, 3> v1, v2;
        v1.load(1, 0, 0);
        v2.load(1, 0, 0);

        REQUIRE_EQ(0, angle(v1, v2));
        REQUIRE_EQ(0, angle(v1, +v1));

        v1.load(1, 0, 0);
        REQUIRE_EQ(doctest::Approx(M_PI), angle(v1, negate(v1)));

        REQUIRE_EQ(doctest::Approx(M_PI), angle(v1, -v1));

        v1.load(2, 1, 0);
        v2.load(2, 4, 0);
        REQUIRE_EQ(doctest::Approx(std::acos((2 * 2 + 4) / (std::sqrt(5) * std::sqrt(20)))), angle(v1, v2));

        v1.load(-3, 2, 0);
        v2.load(10, 2, 0);
        REQUIRE_EQ(doctest::Approx(std::acos((-3 * 10 + 4) / (std::sqrt(13) * std::sqrt(104)))), angle(v1, v2));
    }
    TEST_CASE("normalize") {
        neutrino::math::vector <double, 2> v1{10, 0};
        neutrino::math::vector <double, 2> v2 = normalize(v1);
        REQUIRE_EQ(doctest::Approx(1), v2.x);
        REQUIRE_EQ(0, v2.y);
        REQUIRE_EQ(doctest::Approx(1), norm2(v2));
    }

    TEST_CASE("trigo") {
        neutrino::math::vector <double, 2> v1{M_PI/3, M_PI/4} ;

        neutrino::math::vector <double, 2> v2 = pow2(sin(v1)) + pow2(cos(v1));
        REQUIRE_EQ(doctest::Approx(1), v2.x);
        REQUIRE_EQ(doctest::Approx(1), v2.y);

        neutrino::math::vector <double, 2> v3 = tan(v1);
        neutrino::math::vector <double, 2> v4 = sin(v1) / cos(v1);
        neutrino::math::vector <double, 2> v5 = sin(v1) * inv(cos(v1));
        neutrino::math::vector <double, 2> v6 = sin(v1) * cosec(v1);
        REQUIRE_EQ(doctest::Approx(v3.x), v4.x);
        REQUIRE_EQ(doctest::Approx(v3.y), v4.y);
        REQUIRE_EQ(doctest::Approx(v3.x), v5.x);
        REQUIRE_EQ(doctest::Approx(v3.y), v5.y);
        REQUIRE_EQ(doctest::Approx(v3.x), v6.x);
        REQUIRE_EQ(doctest::Approx(v3.y), v6.y);

        neutrino::math::vector <bool, 2> finite = isfinite(v3);
        REQUIRE(finite.x);
        REQUIRE(finite.y);

        neutrino::math::vector <double, 2> v7{M_PI + M_PI/3, M_PI/4} ;
        neutrino::math::vector <bool, 2> s = signbit(sin(v7));
        REQUIRE(s.x);
        REQUIRE_FALSE(s.y);
        neutrino::math::vector <double, 3> v8{M_PI + M_PI/3, M_PI/4, 0} ;
        neutrino::math::vector <int, 3> s2 = sgn(sin(v8));
        REQUIRE_EQ(-1, s2.x );
        REQUIRE_EQ(1, s2.y);
        REQUIRE_EQ(0, s2.z);
    }

    TEST_CASE("test == ") {
        constexpr neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        constexpr neutrino::math::vector <int, 3> v1 = v0;
        const  neutrino::math::vector <int, 3> v2 = 10*v0;

        REQUIRE(v1 == v0);
        REQUIRE_EQ(v2, 10*v0);
    }

    TEST_CASE("test += ") {
        neutrino::math::vector <int, 3> v0 = {10, 20, 30};
        neutrino::math::vector <int, 3> v2 = {10, 10, 10};
        neutrino::math::vector <int, 3> v3 = v0;
        const neutrino::math::vector <int, 3> v1 = v0 + 10;
        v3 += 10;
        v0 += v2;

        REQUIRE_EQ(v3,  v1);
        REQUIRE_EQ(v0, v1);
    }

    // TEST_CASE("test loop") {
    //     std::vector <int> x(VEC_SIZE, 1);
    //     const auto s_time = funcTime(simple_for_driver, x);
    //     const auto c_time = funcTime(constexpr_for_driver, x);
    //
    //     std::cout << "simple for " << s_time << std::endl
    //         << "constexpr_for " << c_time << std::endl
    //     << "simple_for < constexpr_for " << (s_time < c_time) << std::endl;
    // }
}
