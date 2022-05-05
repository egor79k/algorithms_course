#include <iostream>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <bitset/bitset.h>


TEST_SUITE_BEGIN("bitset");

bool operator==(const BitSet& l, const BitSet& r) {
    if (l.Size() != r.Size()) {
        return false;
    }

    for (int i = 0; i < l.Size(); ++i) {
        if (l[i] != r[i]) {
            return false;
        }
    }

    return true;
}


TEST_CASE("constructing") {
    srand(42);

    SUBCASE("default") {
        BitSet bs;
        CHECK(bs.Size() == 0);
    }

    const int size = rand() % 1000;
    BitSet bs(size, true);

    CHECK(bs.Size() == size);

    for (int i = 0; i < size; ++i) {
        CHECK(bs[i] == true);
    }

    CHECK_THROWS(bs[-1]);
    CHECK_THROWS(bs[size]);
    CHECK_THROWS(bs[-1] = false);
    CHECK_THROWS(bs[size] = false);

    for (int i = 0; i < size / 2; ++i) {
        bs[rand() % size] = false;
    }

    SUBCASE("copy") {
        BitSet bs1(bs);
        CHECK(bs1 == bs);

        BitSet bs2 = bs;
        CHECK(bs2 == bs);

        BitSet bs3(std::move(bs1));
        CHECK(bs1.Size() == 0);
        CHECK(bs3 == bs);

        BitSet bs4 = std::move(bs2);
        CHECK(bs2.Size() == 0);
        CHECK(bs4 == bs);
    }
}


TEST_CASE("modification") {
    const int size = rand() % 1000;

    SUBCASE("clear") {
        BitSet bs(size);
        bs.Clear();
        CHECK(bs.Size() == 0);
        CHECK_THROWS(bs[0]);
    }

    SUBCASE("resize") {
        BitSet bs(size);
        const int new_size = rand() % 1000;
        bs.Resize(new_size);
        BitSet bs1(new_size);
        CHECK(bs == bs1);
        CHECK_THROWS(bs.Resize(0));
    }

    SUBCASE("fill") {
        BitSet bs(size);
        bs.Fill(true);
        BitSet bs1(size, true);
        CHECK(bs == bs1);
    }
}


TEST_CASE("operations") {
    const int size = rand() % 1000;

    BitSet bs1(size);

    for (int i = 0; i < size / 2; ++i) {
        bs1[rand() % size] = true;
    }

    BitSet bs2(size);

    for (int i = 0; i < size / 2; ++i) {
        bs2[rand() % size] = true;
    }

    BitSet bs3(size + 1);

    CHECK_THROWS(bs1 |= bs3);
    CHECK_THROWS(bs1 &= bs3);
    CHECK_THROWS(bs1 ^= bs3);
    CHECK_THROWS(bs1 | bs3);
    CHECK_THROWS(bs1 & bs3);
    CHECK_THROWS(bs1 ^ bs3);

    BitSet bs_res(size);

    for (int i = 0; i < size; ++i) {
        bs_res[i] = bs1[i] | bs2[i];
    }

    CHECK((bs1 | bs2) == bs_res);

    bs1 |= bs2;

    CHECK(bs1 == bs_res);

    for (int i = 0; i < size; ++i) {
        bs_res[i] = bs1[i] & bs2[i];
    }

    CHECK((bs1 & bs2) == bs_res);

    bs1 &= bs2;

    CHECK(bs1 == bs_res);

    for (int i = 0; i < size; ++i) {
        bs_res[i] = bs1[i] ^ bs2[i];
    }

    CHECK((bs1 ^ bs2) == bs_res);

    bs1 ^= bs2;

    CHECK(bs1 == bs_res);
}


TEST_CASE("input/output") {
    std::vector<std::string> eof_input{"10101001110111",
        " 01100101010"};
    std::vector<std::string> good_input{"111101010100101 ",
        "1100101001010  "};

    std::vector<std::string> fail_input{"yagabka ",
        "101010010210101",
        "10010101r01010101",
        ""};

    std::vector<std::string> output{"011001001101101",
        "101010010110101",
        "10010101001010101",
        "0",
        "1"};

    BitSet bs;

    for (auto str : eof_input) {
        std::stringstream istr(str);
        istr >> bs;
        CHECK(istr.eof());
    }

    for (auto str : good_input) {
        std::stringstream istr(str);
        istr >> bs;
        CHECK(istr.good());
    }

    for (auto str : fail_input) {
        std::stringstream istr(str);
        istr >> bs;
        CHECK(istr.fail());
    }

    for (auto str : output) {
        std::stringstream istr(str);
        istr >> bs;
        CHECK(istr.eof());
        istr << bs;
        CHECK(istr.str() == str);
    }
}


TEST_CASE("special cases") {
    const int size = rand() % 1000;
    BitSet bs(size);

    auto flag = bs[size / 2];
    CHECK(flag == false);
}

TEST_SUITE_END();
