#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "bitset.h"


TEST_SUITE_BEGIN("bitset");

void print_bs(const BitSet& bs) {
    for (int i = 0; i < bs.Size(); ++i)
        printf("%d", bs[i]);
    putchar('\n');
}

TEST_CASE("") {
    const int sz = 139;
    BitSet bs1(sz);

    print_bs(bs1);

    bs1[3] = true;
    bs1[6] = true;
    bs1[1] = true;
    bs1[0] = true;
    bs1[19] = true;
    
    print_bs(bs1);

    bs1 >>= 1;

    print_bs(bs1);

    bs1 >>= 5;

    print_bs(bs1);

    bs1 >>= 50;

    print_bs(bs1);

    bs1 <<= 55;

    print_bs(bs1);

    bs1 <<= 1;

    print_bs(bs1);

    BitSet bs2(35, true);

    for (int i = 0; i < 64; ++i)
        printf("%d", static_cast<const BitSet&>(bs2)[i]);
    putchar('\n');

    bs2[2] = false;
    bs2[13] = false;
    bs2[14] = false;
    bs2[15] = false;

    for (int i = 0; i < 64; ++i)
        printf("%d", static_cast<const BitSet&>(bs2)[i]);
    putchar('\n');

    bs2 = ~bs2;

    for (int i = 0; i < 64; ++i)
        printf("%d", static_cast<const BitSet&>(bs2)[i]);
    putchar('\n');
}

TEST_SUITE_END();
