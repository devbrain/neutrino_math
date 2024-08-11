//
// Created by igor on 8/11/24.
//
#include <doctest/doctest.h>
#include <math/vector.hh>


#define vector2D neutrino::sdl::math::vector_2d
#define vector3D neutrino::sdl::math::vector_3d

TEST_SUITE("Test vector 2d") {
    TEST_CASE("Constructors") {
        vector2D <double> a(0);
        REQUIRE_EQ(0.0, a.x);
        REQUIRE_EQ(0.0, a.y);

        vector2D <double> b(1, 2);
        REQUIRE_EQ(1, b.x);
        REQUIRE_EQ(2, b.y);

        vector2D <double> c(a + b);
        REQUIRE_EQ(1, c.x);
        REQUIRE_EQ(2, c.y);

        vector2D <double> d(2.2);
        REQUIRE_EQ(2.2, d.x);
        REQUIRE_EQ(2.2, d.y);

        vector2D B(c);
        REQUIRE_EQ(1, B.x);
        REQUIRE_EQ(2, B.y);

        B = d - a;
        REQUIRE_EQ(2.2, B.x);
        REQUIRE_EQ(2.2, B.y);
    }

    //Initialize the vector
    TEST_CASE("Initialization") {
        vector2D <double> v;
        v.load(1, 1);
        REQUIRE_EQ(1.0, v.x);
        REQUIRE_EQ(1.0, v.y);

        v.load(1, 2.7);
        REQUIRE_EQ(1.0, v.x);
        REQUIRE_EQ(2.7, v.y);

        v.load(1.33, 1e8);
        REQUIRE_EQ(1.33, v.x);
        REQUIRE_EQ(1e8, v.y);
    }

    //-------------------------
    //Vectorial operators
    //-------------------------
    //Equal
    TEST_CASE("Assigment") {
        vector2D <double> v1;
        vector2D <double> v2;
        v2.load(1, 1);
        v1 = v2;
        REQUIRE_EQ(1, v1.x);
        REQUIRE_EQ(1, v1.y);

        v2.load(1.34, -10000.02);
        v1 = v2;
        REQUIRE_EQ(1.34, v1.x);
        REQUIRE_EQ(-10000.02, v1.y);

        v2.load(1e9, 1e12);
        v1 = v2;
        REQUIRE_EQ(1e9, v1.x);
        REQUIRE_EQ(1e12, v1.y);
    }

    //Sum
    TEST_CASE("Addition") {
        vector2D <double> v1;
        vector2D <double> v2;
        vector2D <double> v3;
        v1.load(1, 1);
        v2.load(1, 1);
        v3 = v1 + v2 + v1 + v1;
        REQUIRE_EQ(4, v3.x);
        REQUIRE_EQ(4, v3.y);

        v1.load(34.5, -1.1);
        v2.load(-2.1, -3.3);
        v3 = v1 + v2;
        REQUIRE_EQ(32.4, v3.x);
        REQUIRE_EQ(-4.4, v3.y);

        v1.load(1e6, 1e-6);
        v2.load(1e6, 3e-6);
        v3 = v1 + v2;
        REQUIRE_EQ(2e6, v3.x);
        REQUIRE_EQ(4e-6, v3.y);
    }

    TEST_CASE("Add_assigment") {
        vector2D <double> v1;
        vector2D <double> v2;
        v1.load(1, 1);
        v2.load(1, 1);
        v1 += v2 + v2 + v1;
        REQUIRE_EQ(4, v1.x);
        REQUIRE_EQ(4, v1.y);

        v1.load(2, 2);
        v2.load(3.45, -1.45);
        v1 += v2;
        REQUIRE_EQ(5.45, v1.x);
        REQUIRE_EQ(0.55, v1.y);

        v2.load(-10, -10);
        v1 += v2;
        REQUIRE_EQ(-4.55, v1.x);
        REQUIRE_EQ(-9.45, v1.y);

        v2.load(22, 33);
        v1 += v2;
        REQUIRE_EQ(17.45, v1.x);
        REQUIRE_EQ(23.55, v1.y);
    }

    //Substraction
    TEST_CASE("substracting") {
        vector2D <double> v1, v2, v3;
        v2.load(1, 1);
        v3.load(-2.56, 2.56);
        v1 = v2 - v2 - v3;
        REQUIRE_EQ(2.56, v1.x);
        REQUIRE_EQ(-2.56, v1.y);

        v2.load(1.4, -1);
        v3.load(5.5, 2.56);
        v1 = v2 - v3;
        REQUIRE_EQ(-4.1, v1.x);
        REQUIRE_EQ(-3.56, v1.y);
    }

    TEST_CASE("substracting_assigment") {
        vector2D <double> v1, v2;
        v1.load(1, 1);
        v2.load(-2.56, 2.56);
        v1 -= v2 + v1;
        REQUIRE_EQ(2.56, v1.x);
        REQUIRE_EQ(-2.56, v1.y);

        v1.load(1.4, -1);
        v2.load(5.5, 2.56);
        v1 -= v2;
        REQUIRE_EQ(-4.1, v1.x);
        REQUIRE_EQ(-3.56, v1.y);
    }

    TEST_CASE("left_sum_substracting") {
        vector2D <double> v1, v2;
        v2.load(-2.3, 4.5);
        v1 = +v2;
        REQUIRE_EQ(-2.3, v1.x);
        REQUIRE_EQ(4.5, v1.y);

        v2.load(-2.3, 4.5);
        v1 = -v2;
        REQUIRE_EQ(2.3, v1.x);
        REQUIRE_EQ(-4.5, v1.y);
    }

    //Scalar multiplication
    TEST_CASE("Scalar_multiplication") {
        double a = 1.1;
        vector2D <double> v1;
        vector2D <double> v2;
        v2.load(1, 1);

        v1 = v2 * a;
        REQUIRE_EQ(1.1, v1.x);
        REQUIRE_EQ(1.1, v1.y);

        v1 = a * v2;
        REQUIRE_EQ(1.1, v1.x);
        REQUIRE_EQ(1.1, v1.y);

        v2.load(1, -2.4);
        a = -4.25;
        v1 = v2 * a;
        REQUIRE_EQ(-4.25, v1.x);
        REQUIRE_EQ(10.2, v1.y);

        v1 = v2 * a;
        REQUIRE_EQ(-4.25, v1.x);
        REQUIRE_EQ(10.2, v1.y);
    }

    TEST_CASE("Scalar_multiplication_assigment") {
        vector2D <double> v;
        double a = 1.1;
        v.load(-2.5, 4.56);
        v *= a;
        REQUIRE_EQ(-2.75, v.x);
        REQUIRE_EQ(5.016, v.y);

        a = -2.32;
        v *= a;
        REQUIRE_EQ(6.38, v.x);
        REQUIRE_EQ(-11.63712, v.y);
    }

    //Scalar division
    TEST_CASE("Scalar_division") {
        vector2D <double> v;
        double a = 0.2;
        v.load(-2.5, 28);
        v = v / a;
        REQUIRE_EQ(-12.5, v.x);
        REQUIRE_EQ(140, v.y);

        a = -2.4;
        v.load(-2.4, -7.2);
        v = v / a;
        REQUIRE_EQ(1, v.x);
        REQUIRE_EQ(3, v.y);
    }

    TEST_CASE("Scalar_division_assigment") {
        vector2D <double> v;
        double a = 0.2;
        v.load(-2.5, 28);
        v /= a;
        REQUIRE_EQ(-12.5, v.x);
        REQUIRE_EQ(140, v.y);

        a = -2.4;
        v.load(-2.4, -7.2);
        v /= a;
        REQUIRE_EQ(1, v.x);
        REQUIRE_EQ(3, v.y);
    }

    TEST_CASE("Element_Wise_product") {
        vector2D <double> v1, v2, v3;
        v2.load(1, 2);
        v3.load(3, -2);
        v1 = neutrino::sdl::math::elem_prod(v2, v3);
        REQUIRE_EQ(3, v1.x);
        REQUIRE_EQ(-4, v1.y);
    }

    TEST_CASE("Element_Wise_division") {
        vector2D <double> v1, v2, v3;
        v2.load(1, 2);
        v3.load(1, -2);
        v1 = v2 / v3;
        REQUIRE_EQ(1, v1.x);
        REQUIRE_EQ(-1, v1.y);

        v2.load(2, 1);
        v3.load(1, -2);
        v1 = v2 / v3;
        REQUIRE_EQ(2, v1.x);
        REQUIRE_EQ(-0.5, v1.y);
    }

    TEST_CASE("Element_Wise_division_assigment") {
        vector2D <double> v2, v3;
        v2.load(1, 2);
        v3.load(1, -2);
        v2 /= v3;
        REQUIRE_EQ(1, v2.x);
        REQUIRE_EQ(-1, v2.y);

        v2.load(2, 1);
        v3.load(1, -2);
        v2 /= v3;
        REQUIRE_EQ(2, v2.x);
        REQUIRE_EQ(-0.5, v2.y);
    }

    TEST_CASE("Element_Sum_reduction") {
        vector2D <double> v(1, 2);
        REQUIRE_EQ(3, sum(v));

        v.load(1.1, -2.2);
        REQUIRE_EQ(-1.1, sum(v));
    }

    //Dot product
    TEST_CASE("Dot") {
        vector2D <double> v1, v2;
        v1.load(1, 1);
        v2.load(2, 2);
        REQUIRE_EQ(4, v1 * v2);

        v1.load(1.4, 1);
        v2.load(2, -5.4);
        REQUIRE_EQ(1.4 * 2 - 5.4, dot(v1, v2));

        v1.load(1, 0);
        v2.load(0, 1);
        REQUIRE_EQ(0, v1 * v2);

        v1.load(2, 2);
        v2.load(2, 2);
        REQUIRE_EQ(8, v1 * v2);
    }

    //Cross product
    TEST_CASE("Cross") {
        vector2D <double> v2, v3;
        v2.load(1, 1);
        v3.load(1, 1);
        REQUIRE_EQ(0, cross(v2, v3));

        v2.load(1, 0);
        v3.load(0, 1);
        REQUIRE_EQ(1, cross(v2, v3));

        v2.load(10, 0.5);
        v3.load(2.2, 0.5);
        REQUIRE_EQ(3.9, v2 ^ v3);
    }

    //Norm operations
    TEST_CASE("Norm") {
        vector2D <double> v;
        v.load(1, 2);
        REQUIRE_EQ(std::sqrt(5), v.norm());

        v.load(-10, 2.5);
        REQUIRE_EQ(std::sqrt(100 + 2.5 * 2.5), norm(v));

        v.load(14, -14);
        REQUIRE_EQ(14 * std::sqrt(2), norm(v));

        v.load(0, -2);
        REQUIRE_EQ(2, norm(v));

        v.load(1, 0);
        REQUIRE_EQ(1, norm(v));
    }

    TEST_CASE("Squared_Norm") {
        vector2D <double> v;
        v.load(1, 2);
        REQUIRE_EQ(5, v.norm2());

        v.load(-10, 2.5);
        REQUIRE_EQ(100 + 2.5 * 2.5, norm2(v));

        v.load(0, -2);
        REQUIRE_EQ(4, norm2(v));

        v.load(1, 0);
        REQUIRE_EQ(1, norm2(v));
    }

    //Angle between two vectors
    TEST_CASE("Angle") {
        vector2D <double> v1, v2;
        v1.load(1, 0);
        v2.load(1, 0);
        REQUIRE_EQ(0, angle(v1, v2));

        v1.load(1, 0);
        v2.load(-1, 0);
        REQUIRE_EQ(M_PI, angle(v1, v2));

        v1.load(2, 1);
        v2.load(2, 4);
        REQUIRE_EQ(std::acos((2 * 2 + 4) / (std::sqrt(5) * std::sqrt(20))), angle(v1, v2));

        v1.load(-3, 2);
        v2.load(10, 2);
        REQUIRE_EQ(std::acos((-3 * 10 + 4) / (std::sqrt(13) * std::sqrt(104))), angle(v1, v2));
    }

    TEST_CASE("Unit_Vector_Creation") {
        vector2D <double> v(24, 444);
        REQUIRE_EQ(1.0, norm(unit(v)));

        v.load(1.2, 45);
        v = unit(v);
        REQUIRE_EQ(1, norm(v));

        v.load(1.2, 45);
        v.unit();
        REQUIRE_EQ(1, norm(v));

        v.load(1.2, 0);
        REQUIRE_EQ(1, norm(unit(v)));
    }

    TEST_CASE("Composit_Operations") {
        vector2D <double> v(1, 2);
        vector2D <double> u(1, 2);
        v += 3 * u - u * 5;
        v -= 3 * u - u * 5;
        v *= 3 * u * u * 5;
        v /= 3 * u * u * 5;

        REQUIRE_EQ(u.x, 1);
        REQUIRE_EQ(u.y, 2);
        REQUIRE_EQ(v.x, 1);
        REQUIRE_EQ(v.y, 2);
        v = -v;
        REQUIRE_EQ(v.x, -1);
    }
}

TEST_SUITE("Test vector 3d") {
    TEST_CASE("Constructors") {
        vector3D <double> a(0);
        REQUIRE_EQ(0.0, a.x);
        REQUIRE_EQ(0.0, a.y);
        REQUIRE_EQ(0.0, a.z);

        vector3D <double> b(1, 2, 3);
        REQUIRE_EQ(1, b.x);
        REQUIRE_EQ(2, b.y);
        REQUIRE_EQ(3, b.z);

        vector3D <double> c(a + b);
        REQUIRE_EQ(1, c.x);
        REQUIRE_EQ(2, c.y);
        REQUIRE_EQ(3, c.z);

        vector3D <double> d(2.2);
        REQUIRE_EQ(2.2, d.x);
        REQUIRE_EQ(2.2, d.y);
        REQUIRE_EQ(2.2, d.z);

        vector3D B(c);
        REQUIRE_EQ(1, B.x);
        REQUIRE_EQ(2, B.y);
        REQUIRE_EQ(3, B.z);

        B = d - a;
        REQUIRE_EQ(2.2, B.x);
        REQUIRE_EQ(2.2, B.y);
        REQUIRE_EQ(2.2, B.z);
    }

    //Initialize the vector
    TEST_CASE("Initialization") {
        vector3D <double> v;
        v.load(1, 1, 1);
        REQUIRE_EQ(1.0, v.x);
        REQUIRE_EQ(1.0, v.y);
        REQUIRE_EQ(1.0, v.z);

        v.load(1, 2.7, -0.2);
        REQUIRE_EQ(1.0, v.x);
        REQUIRE_EQ(2.7, v.y);
        REQUIRE_EQ(-0.2, v.z);

        v.load(1.33, 1e8, 1e-8);
        REQUIRE_EQ(1.33, v.x);
        REQUIRE_EQ(1e8, v.y);
        REQUIRE_EQ(1e-8, v.z);
    }

    //-------------------------
    //Vectorial operators
    //-------------------------
    //Equal
    TEST_CASE("Assigment") {
        vector3D <double> v1;
        vector3D <double> v2;
        v2.load(1, 1, 1);
        v1 = v2;
        REQUIRE_EQ(1, v1.x);
        REQUIRE_EQ(1, v1.y);
        REQUIRE_EQ(1, v1.z);

        v2.load(1.34, -10000.02, 0.34);
        v1 = v2;
        REQUIRE_EQ(1.34, v1.x);
        REQUIRE_EQ(-10000.02, v1.y);
        REQUIRE_EQ(0.34, v1.z);

        v2.load(1e9, 1e12, 1e-10);
        v1 = v2;
        REQUIRE_EQ(1e9, v1.x);
        REQUIRE_EQ(1e12, v1.y);
        REQUIRE_EQ(1e-10, v1.z);
    }

    //Sum
    TEST_CASE("Adding") {
        vector3D <double> v1;
        vector3D <double> v2;
        vector3D <double> v3;
        v1.load(1, 1, 1);
        v2.load(1, 1, 1);
        v3 = v1 + v2 + v1 + v1;
        REQUIRE_EQ(4, v3.x);
        REQUIRE_EQ(4, v3.y);
        REQUIRE_EQ(4, v3.z);

        v1.load(34.5, -1.1, 1.5);
        v2.load(-2.1, -3.3, -5.6);
        v3 = v1 + v2;
        REQUIRE_EQ(32.4, v3.x);
        REQUIRE_EQ(-4.4, v3.y);
        REQUIRE_EQ(-4.1, v3.z);

        v1.load(1e6, 1e-6, -1e9);
        v2.load(1e6, 3e-6, 5e9);
        v3 = v1 + v2;
        REQUIRE_EQ(2e6, v3.x);
        REQUIRE_EQ(4e-6, v3.y);
        REQUIRE_EQ(4e9, v3.z);
    }

    TEST_CASE("Add_assigment") {
        vector3D <double> v1;
        vector3D <double> v2;
        v1.load(1, 1, 1);
        v2.load(1, 1, 1);
        v1 += v2 + v2 + v1;
        REQUIRE_EQ(4, v1.x);
        REQUIRE_EQ(4, v1.y);
        REQUIRE_EQ(4, v1.z);

        v1.load(2, 2, 2);
        v2.load(3.45, -1.45, 1.25);
        v1 += v2;
        REQUIRE_EQ(5.45, v1.x);
        REQUIRE_EQ(0.55, v1.y);
        REQUIRE_EQ(3.25, v1.z);

        v2.load(-10, -10, -10);
        v1 += v2;
        REQUIRE_EQ(-4.55, v1.x);
        REQUIRE_EQ(-9.45, v1.y);
        REQUIRE_EQ(-6.75, v1.z);

        v2.load(22, 33, 44);
        v1 += v2;
        REQUIRE_EQ(17.45, v1.x);
        REQUIRE_EQ(23.55, v1.y);
        REQUIRE_EQ(37.25, v1.z);
    }

    //Substraction
    TEST_CASE("substracting") {
        vector3D <double> v1, v2, v3;
        v2.load(1, 1, 1);
        v3.load(-2.56, 2.56, -0.9);
        v1 = v2 - v2 - v3;
        REQUIRE_EQ(2.56, v1.x);
        REQUIRE_EQ(-2.56, v1.y);
        REQUIRE_EQ(0.9, v1.z);

        v2.load(1.4, -1, 20);
        v3.load(5.5, 2.56, -109);
        v1 = v2 - v3;
        REQUIRE_EQ(-4.1, v1.x);
        REQUIRE_EQ(-3.56, v1.y);
        REQUIRE_EQ(129, v1.z);
    }

    TEST_CASE("substracting_assigment") {
        vector3D <double> v1, v2;
        v1.load(1, 1, 1);
        v2.load(-2.56, 2.56, -0.9);
        v1 -= v2 + v1;
        REQUIRE_EQ(2.56, v1.x);
        REQUIRE_EQ(-2.56, v1.y);
        REQUIRE_EQ(0.9, v1.z);

        v1.load(1.4, -1, 20);
        v2.load(5.5, 2.56, -109);
        v1 -= v2;
        REQUIRE_EQ(-4.1, v1.x);
        REQUIRE_EQ(-3.56, v1.y);
        REQUIRE_EQ(129, v1.z);
    }

    TEST_CASE("left_sum_substracting") {
        vector3D <double> v1, v2;
        v2.load(-2.3, 4.5, 6.6);
        v1 = +v2;
        REQUIRE_EQ(-2.3, v1.x);
        REQUIRE_EQ(4.5, v1.y);
        REQUIRE_EQ(6.6, v1.z);

        v2.load(-2.3, 4.5, 6.6);
        v1 = -v2;
        REQUIRE_EQ(2.3, v1.x);
        REQUIRE_EQ(-4.5, v1.y);
        REQUIRE_EQ(-6.6, v1.z);
    }

    //Scalar multiplication
    TEST_CASE("Scalar_multiplication") {
        double a = 1.1;
        vector3D <double> v1;
        vector3D <double> v2;
        v2.load(1, 1, 1);

        v1 = v2 * a;
        REQUIRE_EQ(1.1, v1.x);
        REQUIRE_EQ(1.1, v1.y);
        REQUIRE_EQ(1.1, v1.z);

        v1 = a * v2;
        REQUIRE_EQ(1.1, v1.x);
        REQUIRE_EQ(1.1, v1.y);
        REQUIRE_EQ(1.1, v1.z);

        v2.load(1, -2.4, 4.56);
        a = -4.25;
        v1 = v2 * a;
        REQUIRE_EQ(-4.25, v1.x);
        REQUIRE_EQ(10.2, v1.y);
        REQUIRE_EQ(-19.38, v1.z);

        v1 = v2 * a;
        REQUIRE_EQ(-4.25, v1.x);
        REQUIRE_EQ(10.2, v1.y);
        REQUIRE_EQ(-19.38, v1.z);
    }

    TEST_CASE("Scalar_multiplication_assigment") {
        vector3D <double> v;
        double a = 1.1;
        v.load(-2.5, 4.56, 1000.8);
        v *= a;
        REQUIRE_EQ(-2.75, v.x);
        REQUIRE_EQ(5.016, v.y);
        REQUIRE_EQ(1100.88, v.z);

        a = -2.32;
        v *= a;
        REQUIRE_EQ(6.38, v.x);
        REQUIRE_EQ(-11.63712, v.y);
        REQUIRE_EQ(-2554.0416, v.z);
    }

    //Scalar division
    TEST_CASE("Scalar_division") {
        vector3D <double> v;
        double a = 0.2;
        v.load(-2.5, 28, 10008);
        v = v / a;
        REQUIRE_EQ(-12.5, v.x);
        REQUIRE_EQ(140, v.y);
        REQUIRE_EQ(50040, v.z);

        a = -2.4;
        v.load(-2.4, -7.2, 15.36);
        v = v / a;
        REQUIRE_EQ(1, v.x);
        REQUIRE_EQ(3, v.y);
        REQUIRE_EQ(-6.4, v.z);
    }

    TEST_CASE("Scalar_division_assigment") {
        vector3D <double> v;
        double a = 0.2;
        v.load(-2.5, 28, 10008);
        v /= a;
        REQUIRE_EQ(-12.5, v.x);
        REQUIRE_EQ(140, v.y);
        REQUIRE_EQ(50040, v.z);

        a = -2.4;
        v.load(-2.4, -7.2, 15.36);
        v /= a;
        REQUIRE_EQ(1, v.x);
        REQUIRE_EQ(3, v.y);
        REQUIRE_EQ(-6.4, v.z);
    }

    TEST_CASE("Element_Wise_product") {
        vector3D <double> v1, v2, v3;
        v2.load(1, 2, 3);
        v3.load(3, -2, 1);
        v1 = neutrino::sdl::math::elem_prod(v2, v3);
        REQUIRE_EQ(3, v1.x);
        REQUIRE_EQ(-4, v1.y);
        REQUIRE_EQ(3, v1.z);
    }

    TEST_CASE("Element_Wise_division") {
        vector3D <double> v1, v2, v3;
        v2.load(1, 2, 3);
        v3.load(1, -2, 3);
        v1 = v2 / v3;
        REQUIRE_EQ(1, v1.x);
        REQUIRE_EQ(-1, v1.y);
        REQUIRE_EQ(1, v1.z);

        v2.load(2, 1, 6.6);
        v3.load(1, -2, 3);
        v1 = v2 / v3;
        REQUIRE_EQ(2, v1.x);
        REQUIRE_EQ(-0.5, v1.y);
        REQUIRE_EQ(6.6 / 3, v1.z);
    }

    TEST_CASE("Element_Wise_division_assigment") {
        vector3D <double> v2, v3;
        v2.load(1, 2, 3);
        v3.load(1, -2, 3);
        v2 /= v3;
        REQUIRE_EQ(1, v2.x);
        REQUIRE_EQ(-1, v2.y);
        REQUIRE_EQ(1, v2.z);

        v2.load(2, 1, 6.6);
        v3.load(1, -2, 3);
        v2 /= v3;
        REQUIRE_EQ(2, v2.x);
        REQUIRE_EQ(-0.5, v2.y);
        REQUIRE_EQ(6.6 / 3, v2.z);
    }

    TEST_CASE("Element_Sum_reduction") {
        vector3D <double> v(1, 1, 1);
        REQUIRE_EQ(3, sum(v));

        v.load(1.1, 2.2, -4.4);
        REQUIRE_EQ(-1.1, sum(v));
    }

    //Dot product
    TEST_CASE("Dot") {
        vector3D <double> v1, v2;
        v1.load(1, 1, 1);
        v2.load(2, 2, 2);
        REQUIRE_EQ(6, v1 * v2);

        v1.load(1.4, 1, -10);
        v2.load(2, -5.4, -30);
        REQUIRE_EQ(297.4, dot(v1, v2));

        v1.load(1, 0, 1);
        v2.load(0, 1, 0);
        REQUIRE_EQ(0, v1 * v2);

        v1.load(2, 2, 2);
        v2.load(2, 2, 2);
        REQUIRE_EQ(12, v1 * v2);
    }

    //Cross product
    TEST_CASE("Cross") {
        vector3D <double> v1, v2, v3;
        v2.load(1, 1, 1);
        v3.load(1, 1, 1);
        v1 = cross(v2, v3);
        REQUIRE_EQ(0, v1.x);
        REQUIRE_EQ(0, v1.y);
        REQUIRE_EQ(0, v1.z);

        v2.load(1, 1, 1);
        v3.load(-1, -1, -1);
        v1 = v2 ^ v3;
        REQUIRE_EQ(0, v1.x);
        REQUIRE_EQ(0, v1.y);
        REQUIRE_EQ(0, v1.z);

        v2.load(1, 0, 1);
        v3.load(0, 1, 0);
        v2 ^= v3;
        v1 = v2;
        REQUIRE_EQ(-1, v1.x);
        REQUIRE_EQ(0, v1.y);
        REQUIRE_EQ(1, v1.z);

        v2.load(10, 0.5, -2);
        v3.load(2.2, 0.5, 0);
        v2 ^= v3;
        v1 = v2;
        REQUIRE_EQ(1, v1.x);
        REQUIRE_EQ(-4.4, v1.y);
        REQUIRE_EQ(3.9, v1.z);

        v2.load(10, -100, 100);
        v3.load(2.2, -100, 100);
        v1 = v2 ^ v3;
        REQUIRE_EQ(0, v1.x);
        REQUIRE_EQ(-780, v1.y);
        REQUIRE_EQ(-780, v1.z);
    }

    //Norm operations
    TEST_CASE("Norm") {
        vector3D <double> v;
        v.load(1, 2, -2);
        REQUIRE_EQ(3, v.norm());

        v.load(-10, 2.5, 2);
        REQUIRE_EQ(10.5, norm(v));

        v.load(14, -14, 0);
        REQUIRE_EQ(14 * std::sqrt(2), norm(v));

        v.load(0, -2, 33);
        REQUIRE_EQ(std::sqrt(1093), norm(v));

        v.load(1, 0, 4);
        REQUIRE_EQ(std::sqrt(17), norm(v));
    }

    TEST_CASE("Squared_Norm") {
        vector3D <double> v;
        v.load(1, 2, -2);
        REQUIRE_EQ(9, v.norm2());

        v.load(-10, 2.5, 2);
        REQUIRE_EQ(110.25, norm2(v));

        v.load(14, -14, 0);

        v.load(0, -2, 33);
        REQUIRE_EQ(1093, norm2(v));

        v.load(1, 0, 4);
        REQUIRE_EQ(17, norm2(v));
    }

    //Angle between two vectors
    TEST_CASE("Angle") {
        vector3D <double> v1, v2;
        v1.load(1, 0, 0);
        v2.load(1, 0, 0);
        REQUIRE_EQ(0, angle(v1, v2));

        v1.load(1, 0, 0);
        v2.load(-1, 0, 0);
        REQUIRE_EQ(M_PI, angle(v1, v2));

        v1.load(2, 1, 1);
        v2.load(2, 4, 5);
        REQUIRE_EQ(std::acos(13 * std::sqrt(30) / 90), angle(v1, v2));

        v1.load(-3, 2, 10);
        v2.load(10, 2, 10);
        REQUIRE_EQ(std::acos(37 / std::sqrt(5763)), angle(v1, v2));
    }

    TEST_CASE("Unit_Vector_Creation") {
        vector3D <double> v(1, 2, 3);
        v.unit();
        REQUIRE_EQ(1, norm(v));

        v.load(1.2, 45, -6.6);
        v = unit(v);
        REQUIRE_EQ(1, norm(v));

        v.load(1.2, 45, -6.6);
        REQUIRE_EQ(1, norm(unit(v)));

        v.load(1.2, 0, -6.6);
        REQUIRE_EQ(1, norm(unit(v)));
    }

    TEST_CASE("Composit_Operations") {
        vector3D <double> v(1, 2, 3);
        vector3D <double> u(1, 2, 3);
        v += 3 * u - u * 5;
        v -= 3 * u - u * 5;
        v *= 3 * u * u * 5;
        v /= 3 * u * u * 5;

        REQUIRE_EQ(u.x, 1);
        REQUIRE_EQ(u.y, 2);
        REQUIRE_EQ(u.z, 3);
        REQUIRE_EQ(v.x, 1);
        REQUIRE_EQ(v.y, 2);
        REQUIRE_EQ(v.z, 3);
        v = -v;
        REQUIRE_EQ(v.z, -3);
    }
}
