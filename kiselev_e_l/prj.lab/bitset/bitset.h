#ifndef LAB_BITSET_H
#define LAB_BITSET_H

#include <iostream>

typedef unsigned int uint;

class BitSet {
private:
    class BitHolder;

public:
    BitSet() = default;
    BitSet(const int len, const bool val=false);
    BitSet(const BitSet& other);
    BitSet(BitSet&& other);
    ~BitSet();

    BitSet& operator=(const BitSet& other);
    BitSet& operator=(BitSet&& other);

    void Clear();
    int Size() const;
    void Resize(const int len);
    void Fill(const bool val);

    bool operator[](const int id) const;
    BitHolder operator[](const int id);

    const BitSet operator~();
    BitSet& operator|=(const BitSet& other);
    BitSet& operator&=(const BitSet& other);
    BitSet& operator^=(const BitSet& other);
    BitSet& operator>>=(const int shift);
    BitSet& operator<<=(const int shift);

private:
    class BitHolder {
    public:
        BitHolder() = delete;
        BitHolder(BitSet& bs_, const uint id_);
        BitHolder(const BitHolder& other) = default;
        BitHolder(BitHolder&& other) = default;
        ~BitHolder() = default;
        BitHolder& operator=(const BitHolder& other) = default;
        BitHolder& operator=(BitHolder&& other) = default;
        BitHolder& operator=(const bool val);
        operator bool() const;

    private:
        BitSet& bs;
        const uint id = 0;
    };

    static const int unit_size = 8 * sizeof(uint); // Minimal indivisible memory unit
    static const uint first_bit = 1 << (unit_size - 1); // 10...0

    int size = 0; // Num of bits
    int capacity = 0; // Size of allocated memory
    uint* data = nullptr;
};


const BitSet operator|(const BitSet& l, const BitSet& r);
const BitSet operator&(const BitSet& l, const BitSet& r);
const BitSet operator^(const BitSet& l, const BitSet& r);
const BitSet operator>>(const BitSet& l, const int shift);
const BitSet operator<<(const BitSet& l, const int shift);

std::istream& operator>>(std::istream& is, BitSet& bs);
std::ostream& operator<<(std::ostream& os, const BitSet& bs);

#endif // LAB_BITSET_H