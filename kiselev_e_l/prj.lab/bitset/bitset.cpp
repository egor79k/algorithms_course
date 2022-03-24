#include "bitset.h"


BitSet::BitSet(const int len, const bool val) :
    size(len),
    capacity(len / sizeof(unsigned int)),
    data(new unsigned int[len / sizeof(unsigned int)]) {
    Set(val);
}


BitSet::~BitSet() {
    delete[] data;
}


void BitSet::Set(const bool val) {
    for (int i = 0; i < capacity; ++i) {
        data[i] = ~0;
    }
}