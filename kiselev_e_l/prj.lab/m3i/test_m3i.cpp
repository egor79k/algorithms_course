#include <iostream>
#include <sstream>
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <m3i/m3i.h>


TEST_SUITE_BEGIN("m3i");

TEST_CASE("construction") {
    srand(42);
    
    SUBCASE("default constructor") {
        M3i m1;
        CHECK((m1.Size(0) == 0 && m1.Size(1) == 0 && m1.Size(2) == 0));
    }

    // Constructors with params
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    M3i m1(size[0], size[1], size[2]);

    REQUIRE((m1.Size(0) == size[0] && m1.Size(1) == size[1] && m1.Size(2) == size[2]));
    REQUIRE(0 == m1.At(0, 0, 0));

    CHECK_THROWS({M3i m(-size[0], size[1], size[2]);});
    CHECK_THROWS({M3i m(size[0], -size[1], size[2]);});
    CHECK_THROWS({M3i m(size[0], size[1], -size[2]);});

    CHECK_THROWS(m1.Size(3));
    CHECK_THROWS(m1.Size(-1));

    SUBCASE("init list constructor") {
        M3i m2{{{1, 2, 3},
                {4, 5, 6}},
                {{7, 8, 9},
                {0, 1, 2}}};

        CHECK((m2.Size(0) == 2 && m2.Size(1) == 2 && m2.Size(2) == 3));
        CHECK((m2.At(0, 1, 2) == 6 && m2.At(1, 0, 1) == 8));
    }

    SUBCASE("copy constructor") {
        M3i m2(m1);
        REQUIRE((m2.Size(0) == m1.Size(0) && m2.Size(1) == m1.Size(1) && m2.Size(2) == m1.Size(2)));
        REQUIRE(m2.At(0, 0, 0) == m1.At(0, 0, 0));
    }

    SUBCASE("copy assignment") {
        M3i m2 = m1;
        CHECK((m2.Size(0) == m1.Size(0) && m2.Size(1) == m1.Size(1) && m2.Size(2) == m1.Size(2)));
        CHECK(m2.At(0, 0, 0) == m1.At(0, 0, 0));
    }

    SUBCASE("move constructor") {
        M3i m2(m1);
        M3i m3(std::move(m2));
        CHECK((m3.Size(0) == m1.Size(0) && m3.Size(1) == m1.Size(1) && m3.Size(2) == m1.Size(2)));
        CHECK(m3.At(0, 0, 0) == m1.At(0, 0, 0));
    }

    SUBCASE("move assignment") {
        M3i m2(m1);
        M3i m3 = std::move(m2);
        CHECK((m3.Size(0) == m1.Size(0) && m3.Size(1) == m1.Size(1) && m3.Size(2) == m1.Size(2)));
        CHECK(m3.At(0, 0, 0) == m1.At(0, 0, 0));
    }
}

TEST_CASE("modification") {
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    SUBCASE("fill") {
        M3i m1(size[0], size[1], size[2]);
        int val = rand();

        m1.Fill(val);

        for (int x_id = 0; x_id < size[0]; ++x_id) {
            for (int y_id = 0; y_id < size[1]; ++y_id) {
                for (int z_id = 0; z_id < size[2]; ++z_id) {
                    CHECK(m1.At(x_id, y_id, z_id) == val);
                }
            }
        }
    }

    SUBCASE("access to elements") {
        M3i m1(size[0], size[1], size[2]);

        int id[3] = {rand() % size[0], rand() % size[1], rand() % size[2]};
        int val = rand();

        m1.At(id[0], id[1], id[2]) = val;
        CHECK(m1.At(id[0], id[1], id[2]) == val);

        CHECK_THROWS(m1.At(size[0], size[1], size[2]));
        CHECK_THROWS(m1.At(id[0], -id[1], id[2]) = val);
        CHECK_THROWS(m1.At(id[0], -id[1], id[2]));
    }

    SUBCASE("resizing") {
        M3i m1(size[0], size[1], size[2]);

        // Filling tensor with random values
        for (int x_id = 0; x_id < size[0]; ++x_id) {
            for (int y_id = 0; y_id < size[1]; ++y_id) {
                for (int z_id = 0; z_id < size[2]; ++z_id) {
                    m1.At(x_id, y_id, z_id) = rand();
                }
            }
        }

        M3i m2 = m1.Clone();

        int new_size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};
        m2.Resize(new_size[0], new_size[1], new_size[2]);

        CHECK((m2.Size(0) == new_size[0] && m2.Size(1) == new_size[1] && m2.Size(2) == new_size[2]));

        for (int x_id = 0; x_id < std::min(size[0], new_size[0]); ++x_id) {
            for (int y_id = 0; y_id < std::min(size[1], new_size[1]); ++y_id) {
                for (int z_id = 0; z_id < std::min(size[2], new_size[2]); ++z_id) {
                    CHECK(m2.At(x_id, y_id, z_id) == m1.At(x_id, y_id, z_id));
                }
            }
        }

        CHECK_THROWS(m2.Resize(-new_size[0], new_size[1], new_size[2]));
    }
}

TEST_CASE("shared data") {
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    M3i m1(size[0], size[1], size[2]);

    // Filling tensor with random values
    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                m1.At(x_id, y_id, z_id) = rand();
            }
        }
    }

    M3i m2(m1);

    SUBCASE("element access") {
        int id[3] = {rand() % size[0], rand() % size[1], rand() % size[2]};
        int val = rand();

        CHECK(m1.At(id[0], id[1], id[2]) == m2.At(id[0], id[1], id[2]));
        m1.At(id[0], id[1], id[2]) = val;
        CHECK(m2.At(id[0], id[1], id[2]) == val);
    }

    SUBCASE("resizing") {
        int new_size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

        m2.Resize(new_size[0], new_size[1], new_size[2]);
        CHECK((m1.Size(0) == new_size[0] && m1.Size(1) == new_size[1] && m1.Size(2) == new_size[2]));
    }

    SUBCASE("clone") {
        M3i m3 = m1.Clone();

        CHECK((m1.Size(0) == m3.Size(0) && m1.Size(1) == m3.Size(1) && m1.Size(2) == m3.Size(2)));

        int id[3] = {rand() % size[0], rand() % size[1], rand() % size[2]};
        int val = rand();
        int old_val = m3.At(id[0], id[1], id[2]);

        CHECK(m1.At(id[0], id[1], id[2]) == old_val);
        m1.At(id[0], id[1], id[2]) = val;
        CHECK(m3.At(id[0], id[1], id[2]) == old_val);
    }
}

TEST_CASE("input and output") {
    int size[3] = {rand() % 100 + 1, rand() % 100 + 1, rand() % 100 + 1};

    M3i m1(size[0], size[1], size[2]);

    // Filling tensor with random values
    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                m1.At(x_id, y_id, z_id) = rand();
            }
        }
    }

    std::stringstream str;
    M3i m2;

    str << m1;

    std::string size_str;

    str >> size_str;

    CHECK(size_str == "size:");

    CHECK_NOTHROW(str >> m2);

    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                CHECK(m1.At(x_id, y_id, z_id) == m2.At(x_id, y_id, z_id));
            }
        }
    }
}

TEST_SUITE_END();