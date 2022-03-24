#ifndef LAB_BITSET_H
#define LAB_BITSET_H

class BitSet {
public:
    BitSet();
    BitSet(const int len, const bool val=false);
    BitSet(const BitSet& other);
    BitSet(BitSet&& other);
    ~BitSet();

    BitSet& operator=(const BitSet& other);
    BitSet& operator=(BitSet&& other);

    int Size() const;
    void Resize(const int len);
    void Set(const bool val);

    //BitSet::BitHolder operator[](const int id);
    bool operator[](const int id) const;

    // Bit operations: | & ^ ~ >> <<
/*
    class BitHolder {
    public:
        // BitHolder(const BitHolder& other) = delete; ??
        BitHolder& operator=(const bool val);

    private:

    };
*/
private:
    int size = 0; // Num of bits
    int capacity = 0; // Size of allocated memory 
    unsigned int* data = nullptr;
};

#endif // LAB_BITSET_H