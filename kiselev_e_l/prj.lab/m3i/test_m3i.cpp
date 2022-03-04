#include "m3i.h"

#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "m3i.h"


TEST_SUITE_BEGIN("m3i");

//const M3i::Shape zero({0, 0, 0});
/*
inline bool operator==(const int[3]& l, const int[3]& r) {
    return (l[0] == r[0] && l[1] == r[1] && l[2] == r[2]);
*/
TEST_CASE("construction") {
    srand(42);
    
    SUBCASE("default constructor") {
        M3i m1;
        CHECK((m1.size(0) == 0 && m1.size(1) == 0 && m1.size(2) == 0));
    }

    // Constructors with params
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    M3i m1(size[0], size[1], size[2]);

    REQUIRE((m1.size(0) == size[0] && m1.size(1) == size[1] && m1.size(2) == size[2]));
    REQUIRE(0 == m1.at(0, 0, 0));

    SUBCASE("copy constructor") {
        M3i m2(m1);
        REQUIRE((m2.size(0) == m1.size(0) && m2.size(1) == m1.size(1) && m2.size(2) == m1.size(2)));
        REQUIRE(m2.at(0, 0, 0) == m1.at(0, 0, 0));
    }

    SUBCASE("copy assignment") {
        M3i m2 = m1;
        CHECK((m2.size(0) == m1.size(0) && m2.size(1) == m1.size(1) && m2.size(2) == m1.size(2)));
        CHECK(m2.at(0, 0, 0) == m1.at(0, 0, 0));
    }

    SUBCASE("move constructor") {
        M3i m2(m1);
        M3i m3(std::move(m2));
        CHECK((m3.size(0) == m1.size(0) && m3.size(1) == m1.size(1) && m3.size(2) == m1.size(2)));
        CHECK(m3.at(0, 0, 0) == m1.at(0, 0, 0));
    }

    SUBCASE("move assignment") {
        M3i m2(m1);
        M3i m3 = std::move(m2);
        CHECK((m3.size(0) == m1.size(0) && m3.size(1) == m1.size(1) && m3.size(2) == m1.size(2)));
        CHECK(m3.at(0, 0, 0) == m1.at(0, 0, 0));
    }
}

TEST_CASE("modification") {
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    SUBCASE("fill") {
        M3i m1(size[0], size[1], size[2]);
        int val = rand();

        m1.fill(val);

        for (int x_id = 0; x_id < size[0]; ++x_id) {
            for (int y_id = 0; y_id < size[1]; ++y_id) {
                for (int z_id = 0; z_id < size[2]; ++z_id) {
                    CHECK(m1.at(x_id, y_id, z_id) == val);
                }
            }
        }
    }

    SUBCASE("access to elements") {
        M3i m1(size[0], size[1], size[2]);

        int id[3] = {rand() % size[0], rand() % size[1], rand() % size[2]};
        int val = rand();

        m1.at(id[0], id[1], id[2]) = val;
        CHECK(m1.at(id[0], id[1], id[2]) == val);
    }

    SUBCASE("resizing") {
        M3i m1(size[0], size[1], size[2]);

        // Filling tensor with random values
        for (int x_id = 0; x_id < size[0]; ++x_id) {
            for (int y_id = 0; y_id < size[1]; ++y_id) {
                for (int z_id = 0; z_id < size[2]; ++z_id) {
                    m1.at(x_id, y_id, z_id) = rand();
                }
            }
        }

        M3i m2 = m1.clone();

        int new_size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};
        m2.resize(new_size[0], new_size[1], new_size[2]);

        CHECK((m2.size(0) == new_size[0] && m2.size(1) == new_size[1] && m2.size(2) == new_size[2]));

        for (int x_id = 0; x_id < std::min(size[0], new_size[0]); ++x_id) {
            for (int y_id = 0; y_id < std::min(size[1], new_size[1]); ++y_id) {
                for (int z_id = 0; z_id < std::min(size[2], new_size[2]); ++z_id) {
                    CHECK(m2.at(x_id, y_id, z_id) == m1.at(x_id, y_id, z_id));
                }
            }
        }
    }
}

TEST_CASE("shared data") {
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    M3i m1(size[0], size[1], size[2]);

    // Filling tensor with random values
    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                m1.at(x_id, y_id, z_id) = rand();
            }
        }
    }

    M3i m2(m1);

    SUBCASE("element access") {
        int id[3] = {rand() % size[0], rand() % size[1], rand() % size[2]};
        int val = rand();

        CHECK(m1.at(id[0], id[1], id[2]) == m2.at(id[0], id[1], id[2]));
        m1.at(id[0], id[1], id[2]) = val;
        CHECK(m2.at(id[0], id[1], id[2]) == val);
    }

    SUBCASE("resizing") {
        int new_size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

        m2.resize(new_size[0], new_size[1], new_size[2]);
        CHECK((m1.size(0) == new_size[0] && m1.size(1) == new_size[1] && m1.size(2) == new_size[2]));
    }
}

TEST_CASE("input and output") {
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    M3i m1(size[0], size[1], size[2]);

    // Filling tensor with random values
    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                m1.at(x_id, y_id, z_id) = rand();
            }
        }
    }

    std::stringstream str;
    M3i m2;

    str << m1.size(0) << " " << m1.size(1) << " " << m1.size(2) << " \n" << m1;

    str >> m2;

    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                CHECK(m1.at(x_id, y_id, z_id) == m2.at(x_id, y_id, z_id));
            }
        }
    }
}

TEST_SUITE_END();