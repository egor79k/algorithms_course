#include "bitset.h"
#include <iostream>
#include <bitset/bitset.h>

BitSet::BitSet(const int len, const bool val) :
    size(len),
    capacity((len + unit_size - 1) / unit_size),
    data(new uint[(len + unit_size - 1) / unit_size]) {
    Set(val);
}


BitSet::BitSet(const BitSet& other) :
    size(other.size),
    capacity(other.capacity),
    data(new uint[other.capacity]) {
    for (int i = 0; i < capacity; ++i) {
        data[i] = other.data[i];
    }
}


BitSet::BitSet(BitSet&& other) :
    size(other.size),
    capacity(other.capacity),
    data(other.data) {
    other.Clear();
}


BitSet::~BitSet() {
    Clear();
}


BitSet& BitSet::operator=(const BitSet& other) {
    Clear();
    
    size = other.size;
    capacity = other.capacity;
    data = new uint[other.capacity];

    for (int i = 0; i < capacity; ++i) {
        data[i] = other.data[i];
    }

    return *this;
}


BitSet& BitSet::operator=(BitSet&& other) {
    Clear();

    size = other.size;
    capacity = other.capacity;
    data = other.data;

    other.Clear();

    return *this;
}


void BitSet::Clear() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }

    size = 0;
    capacity = 0;
}


int BitSet::Size() const {
    //printf("s=%d, c=%d\n", size, capacity);
    return size;
}


void BitSet::Resize(const int len) {
    if (len < 0) {
        return;
    }

    const int old_capacity = capacity;
    uint* old_data = data;
    
    capacity = (len + unit_size - 1) / unit_size;
    data = new uint[capacity];

    Set(false);

    for (int i = 0; i < std::min(capacity, old_capacity); ++i) {
        data[i] = old_data[i];
    }
}


void BitSet::Set(const bool val) {
    uint bits_val = val ? ~0 : 0;

    for (int i = 0; i < capacity; ++i) {
        data[i] = bits_val;
    }

    // Reset tail to zeros
    data[capacity - 1] &= ((~0) << (capacity * unit_size - size));
}


bool BitSet::operator[](const int id) const {
    return data[id / unit_size] & (first_bit >> (id % unit_size));
}


BitSet::BitHolder BitSet::operator[](const int id) {
    return BitHolder(data + id / unit_size, first_bit >> (id % unit_size));
}


const BitSet BitSet::operator~() {
    BitSet result(*this);

    for (int i = 0; i < capacity; ++i) {
        result.data[i] = ~data[i];
    }

    // Reset tail to zeros
    result.data[capacity - 1] &= ((~0) << (capacity * unit_size - size));

    return result;
}


BitSet& BitSet::operator|=(const BitSet& other) {
    if (size != other.size) {
        return *this;
    }

    for (int i = 0; i < capacity; ++i) {
        data[i] |= other.data[i];
    }

    return *this;
}


BitSet& BitSet::operator&=(const BitSet& other) {
    if (size != other.size) {
        return *this;
    }

    for (int i = 0; i < capacity; ++i) {
        data[i] &= other.data[i];
    }

    return *this;
}


BitSet& BitSet::operator^=(const BitSet& other) {
    if (size != other.size) {
        return *this;
    }

    for (int i = 0; i < capacity; ++i) {
        data[i] ^= other.data[i];
    }

    return *this;
}


BitSet& BitSet::operator>>=(const int shift) {
    const int unit_shift = shift / unit_size;
    const int bit_shift = shift % unit_size;

    for (int i = capacity - 1; i >= 0; --i) {
        if (i >= unit_shift) {
            data[i] = data[i - unit_shift];
            data[i] >>= bit_shift;
            if (i > unit_shift) {
                data[i] |= (data[i - unit_shift - 1] << (unit_size - bit_shift));
            }
        }
        else {
            data[i] = 0;
        }
    }

    return *this;
}


BitSet& BitSet::operator<<=(const int shift) {
    const int unit_shift = shift / unit_size;
    const int bit_shift = shift % unit_size;

    for (int i = 0; i < capacity; ++i) {
        if (i + unit_shift < capacity) {
            data[i] = data[i + unit_shift];
            data[i] <<= bit_shift;
            if (i + unit_shift + 1 < capacity) {
                data[i] |= (data[i + unit_shift + 1] >> (unit_size - bit_shift));
            }
        }
        else {
            data[i] = 0;
        }
    }

    return *this;
}


BitSet::BitHolder::BitHolder(uint* const unit_, const uint mask_) :
    unit(unit_),
    mask(mask_) {}


BitSet::BitHolder& BitSet::BitHolder::operator=(const bool val) {
    if (val) {
        *unit |= mask;
    }
    else {
        *unit &= ~mask;
    }

    return *this;
}


const BitSet operator|(const BitSet& l, const BitSet& r) {
    BitSet result(l);
    return result |= r;
}


const BitSet operator&(const BitSet& l, const BitSet& r) {
    BitSet result(l);
    return result &= r;
}


const BitSet operator^(const BitSet& l, const BitSet& r) {
    BitSet result(l);
    return result ^= r;
}


const BitSet operator>>(const BitSet& l, const int shift) {
    BitSet result(l);
    return result >> shift;
}


const BitSet operator<<(const BitSet& l, const int shift) {
    BitSet result(l);
    return result << shift;
}
