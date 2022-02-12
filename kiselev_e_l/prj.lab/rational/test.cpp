#include <iostream>
#include <cmath>
#include <cstdlib>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "rational.h"


TEST_SUITE_BEGIN("rational");

TEST_CASE("construction")
{
    srand(42);
    
    // Default constructor
    Rational q0;
    REQUIRE(q0.getNum() == 0);
    REQUIRE(q0.getDen() == 1);

    int num = rand() - RAND_MAX / 2;
    int den = abs(2 * num + 1);

    // One-param constuctor
    Rational q1(num);
    REQUIRE(q1.getNum() == num);
    REQUIRE(q1.getDen() == 1);

    // Two-params constructor
    Rational q2(num, den);
    REQUIRE(q2.getNum() == num);
    REQUIRE(q2.getDen() == den);

    SUBCASE("copy constructor")
    {
        Rational q3(q2);
        CHECK(q3.getNum() == num);
        CHECK(q3.getDen() == den);
    }

    SUBCASE("assignment constructor")
    {
        q0 = q2;
        CHECK(q0.getNum() == num);
        CHECK(q0.getDen() == den);
    }
}

TEST_CASE("compare operations")
{
    int num = rand() - RAND_MAX / 2;
    int den = rand() + 1;
    int diff = 1;

    Rational q1(num, den);
    Rational q2(q1);
    Rational q3(num + diff, den + diff);
    Rational q4(num - diff, den - diff);

    REQUIRE(q1 == q2);
    CHECK(q1 != q3);
    CHECK(q1 <= q2);
    CHECK(q1 >= q2);
    CHECK(q1 <= q3);
    CHECK(q1 >= q4);
    CHECK(q1 < q3);
    CHECK(q1 > q4);
}

TEST_CASE("arithmetic operations")
{
    int iter = 100;

    while (iter--)
    {
        int num1 = (rand() - RAND_MAX / 2) % 10000;
        int den1 = rand() % 10000 + 1;
        int num2 = (rand() - RAND_MAX / 2) % 10000;
        int den2 = rand() % 10000 + 1;

        Rational q1(num1, den1);
        Rational q2(num2, den2);

        CHECK(q1 + q2 == Rational(num1 * den2 + num2 * den1, den1 * den2));
        CHECK(q1 - q2 == Rational(num1 * den2 - num2 * den1, den1 * den2));
        CHECK(q1 * q2 == Rational(num1 * num2, den1 * den2));
        CHECK(q1 / q2 == Rational(num1 * den2, den1 * num2));

        Rational q3(q1);
        q1 += q2;
        CHECK(q1 == Rational(num1 * den2 + num2 * den1, den1 * den2));

        q1 = q3;
        q1 -= q2;
        CHECK(q1 == Rational(num1 * den2 - num2 * den1, den1 * den2));

        q1 = q3;
        q1 *= q2;
        CHECK(q1 == Rational(num1 * num2, den1 * den2));

        q1 = q3;
        q1 /= q2;
        CHECK(q1 == Rational(num1 * den2, den1 * num2));
    }
}

TEST_SUITE_END();