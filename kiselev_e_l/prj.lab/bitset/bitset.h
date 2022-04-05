#ifndef LAB_BITSET_H
#define LAB_BITSET_H

typedef unsigned int uint;

class BitSet {
private:
    class BitHolder {
    public:
        BitHolder() = delete;
        //BitHolder(const BitHolder& other) = delete;
        //BitHolder(BitHolder&& other) = delete;

        BitHolder(uint* const unit_, const uint mask_);

        BitHolder& operator=(const bool val);

    private:
        uint* const unit = 0;
        const uint mask = 0;
    };

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

#endif // LAB_BITSET_H