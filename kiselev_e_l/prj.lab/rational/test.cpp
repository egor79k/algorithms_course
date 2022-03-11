#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "rational.h"


TEST_SUITE_BEGIN("rational");

TEST_CASE("construction") {
    srand(42);
    
    // Default constructor
    Rational q0;
    REQUIRE(q0.num() == 0);
    REQUIRE(q0.denum() == 1);

    int num = rand() - RAND_MAX / 2;
    int den = abs(2 * num + 1);

    // One-param constuctor
    Rational q1(num);
    REQUIRE(q1.num() == num);
    REQUIRE(q1.denum() == 1);

    // Two-params constructor
    Rational q2(num, den);
    REQUIRE(q2.num() == num);
    REQUIRE(q2.denum() == den);

    // Zero denum exception
    CHECK_THROWS(Rational(rand(), 0));

    SUBCASE("copy constructor") {
        Rational q3(q2);
        CHECK(q3.num() == num);
        CHECK(q3.denum() == den);
    }

    SUBCASE("assignment constructor") {
        q0 = q2;
        CHECK(q0.num() == num);
        CHECK(q0.denum() == den);
    }
}

TEST_CASE("compare operations") {
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

TEST_CASE("arithmetic operations") {
    int iter = 100;

    while (iter--) {
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

    Rational q1(rand(), rand() + 1);
    
    CHECK_THROWS(q1 / Rational(0, rand()));
    CHECK_THROWS(q1 /= Rational(0, rand()));
}

TEST_CASE("special cases") {
    SUBCASE("zero equality") {
        Rational q1(0, rand() - RAND_MAX / 2);
        Rational q2(0, rand() - RAND_MAX / 2);
    
        CHECK(1 == q1.denum());
        CHECK(q1 == q2);
    }

    struct Fraction {
        std::string str;
        int nmr;
        int dmr;
    };

    const std::vector<Fraction> fractions{{"3/4", 3, 4},
                                          {"-1/125", 1, -125},
                                          {"0/1", 0, 1},
                                          {"428/1517", -428, -1517},
                                          {"1/1", 1024, 1024},
                                          {"-8/9", -8, 9},
                                          {"17/380", 17, 380},
                                          {"-12345/1", 12345, -1}};

    SUBCASE("operator <<") {
        for (auto frac : fractions) {
            std::stringstream output;
            Rational q1(frac.nmr, frac.dmr);

            output << q1;

            CHECK(output.str() == frac.str);
        }
    }

    SUBCASE("operator >>") {
        for (auto frac : fractions) {
            std::stringstream input(frac.str);
            Rational q1;

            input >> q1;

            CHECK(!input.fail());
            CHECK(Rational(frac.nmr, frac.dmr) == q1);
        }

        std::vector<const char*> bad_input{"16/0", "9/-81", "3 15", "4\\80", "1 / 25", "7/ 8", "5 /17"};

        for (auto str : bad_input) {
            std::stringstream input(str);
            Rational q1;

            input >> q1;
            //CHECK_THROWS(input >> q1);
            CHECK(input.fail());
        }
    }
}

TEST_SUITE_END();