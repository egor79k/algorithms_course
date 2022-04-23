#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <bitset/bitset.h>


TEST_SUITE_BEGIN("bitset");

TEST_CASE("") {
    const int sz = 139;
    BitSet bs1(sz);

    std::cout << bs1 << '\n';

    bs1[3] = true;
    bs1[6] = true;
    bs1[1] = true;
    bs1[0] = true;
    bs1[19] = true;
    
    auto x = bs1[4];

    std::cout << bs1 << '\n';

    bs1 >>= 1;

    std::cout << bs1 << '\n';

    bs1 >>= 5;

    std::cout << bs1 << '\n';

    bs1 >>= 50;

    std::cout << bs1 << '\n';

    bs1 <<= 55;

    std::cout << bs1 << '\n';

    bs1 <<= 1;

    std::cout << bs1 << '\n';
}

TEST_SUITE_END();
