#include "m3i.h"

#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "m3i.h"


TEST_SUITE_BEGIN("m3i");

const M3i::Shape zero({0, 0, 0});

TEST_CASE("construction") {
    srand(42);
    
    SUBCASE("default constructor") {
        M3i m1;
        CHECK(m1.get_size() == zero);
    }

    // Constructors with params
    M3i::Shape size = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};
    M3i m0(size.x, size.y, size.z);

    REQUIRE(m0.get_size() == size);
    REQUIRE(0 == m0.at(0, 0, 0));

    M3i m1(size);

    REQUIRE(m1.get_size() == size);
    REQUIRE(0 == m1.at(0, 0, 0));

    int val = rand();
    M3i m2(size, val);

    REQUIRE(m2.get_size() == size);
    REQUIRE(m2.at(0, 0, 0) == val);

    SUBCASE("copy constructor") {
        M3i m3(m2);
        REQUIRE(m3.get_size() == m2.get_size());
        REQUIRE(m3.at(0, 0, 0) == m2.at(0, 0, 0));
    }

    SUBCASE("move constructor") {
        M3i m3(m2);
        M3i m4(std::move(m3));
        CHECK(m4.get_size() == m2.get_size());
        CHECK(m4.at(0, 0, 0) == m2.at(0, 0, 0));
        CHECK(m3.get_size() == zero);
    }

    SUBCASE("copy assignment") {
        M3i m3 = m2;
        CHECK(m3.get_size() == m2.get_size());
        CHECK(m3.at(0, 0, 0) == m2.at(0, 0, 0));
    }

    SUBCASE("move assignment") {
        M3i m3(m2);
        M3i m4 = std::move(m3);
        CHECK(m4.get_size() == m2.get_size());
        CHECK(m4.at(0, 0, 0) == m2.at(0, 0, 0));
        CHECK(m3.get_size() == zero);
    }
}

TEST_CASE("modification") {
    M3i::Shape size = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};
    
    SUBCASE("clear") {
        M3i m1(size);

        m1.clear();
        CHECK(m1.get_size() == zero);
    }

    SUBCASE("fill") {
        M3i m1(size);
        int val = rand();

        m1.fill(val);

        for (int x_id = 0; x_id < size.x; ++x_id) {
            for (int y_id = 0; y_id < size.y; ++y_id) {
                for (int z_id = 0; z_id < size.z; ++z_id) {
                    CHECK(m1.at(x_id, y_id, z_id) == val);
                }
            }
        }
    }

    SUBCASE("access to elements") {
        M3i m1(size);

        M3i::Shape id = {rand() % size.x, rand() % size.y, rand() % size.z};
        int val = rand();

        m1.at(id.x, id.y, id.z) = val;
        CHECK(m1.at(id.x, id.y, id.z) == val);
    }

    SUBCASE("resizing") {
        M3i m1(size);

        for (int x_id = 0; x_id < size.x; ++x_id) {
            for (int y_id = 0; y_id < size.y; ++y_id) {
                for (int z_id = 0; z_id < size.z; ++z_id) {
                    m1.at(x_id, y_id, z_id) = rand();
                }
            }
        }

        M3i m2(m1);

        M3i::Shape new_size = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};
        m2.resize(new_size.x, new_size.y, new_size.z);

        CHECK(m2.get_size() == new_size);

        for (int x_id = 0; x_id < std::min(size.x, new_size.x); ++x_id) {
            for (int y_id = 0; y_id < std::min(size.y, new_size.y); ++y_id) {
                for (int z_id = 0; z_id < std::min(size.z, new_size.z); ++z_id) {
                    CHECK(m2.at(x_id, y_id, z_id) == m1.at(x_id, y_id, z_id));
                }
            }
        }
    }
}

TEST_SUITE_END();