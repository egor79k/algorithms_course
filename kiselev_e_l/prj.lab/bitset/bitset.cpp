#include <algorithm>
#include <stdexcept>

#include <bitset/bitset.h>

BitSet::BitSet(const int len, const bool val) :
    size(len),
    capacity((len + unit_size - 1) / unit_size),
    data(new uint[(len + unit_size - 1) / unit_size]) {
    Fill(val);
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
    other.size = 0;
    other.capacity = 0;
    other.data = nullptr;
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

    other.size = 0;
    other.capacity = 0;
    other.data = nullptr;

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
        throw std::invalid_argument("Negative size");
    }

    const int old_capacity = capacity;
    uint* old_data = data;
    
    size = len;
    capacity = (len + unit_size - 1) / unit_size;
    data = new uint[capacity];

    Fill(false);

    for (int i = 0; i < std::min(capacity, old_capacity); ++i) {
        data[i] = old_data[i];
    }
}


void BitSet::Fill(const bool val) {
    uint bits_val = val ? ~0 : 0;

    for (int i = 0; i < capacity; ++i) {
        data[i] = bits_val;
    }

    // Reset tail to zeros
    data[capacity - 1] &= ((~0) << (capacity * unit_size - size));
}


bool BitSet::operator[](const int id) const {
    if (id < 0 || id >= size) {
        throw std::invalid_argument("Index out of bounds");
    }
    
    return data[id / unit_size] & (first_bit >> (id % unit_size));
}


BitSet::BitHolder BitSet::operator[](const int id) {
    if (id < 0 || id >= size) {
        throw std::invalid_argument("Index out of bounds");
    }

    return BitHolder(*this, id);
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
        throw std::invalid_argument("Different sizes");
    }

    for (int i = 0; i < capacity; ++i) {
        data[i] |= other.data[i];
    }

    return *this;
}


BitSet& BitSet::operator&=(const BitSet& other) {
    if (size != other.size) {
        throw std::invalid_argument("Different sizes");
    }

    for (int i = 0; i < capacity; ++i) {
        data[i] &= other.data[i];
    }

    return *this;
}


BitSet& BitSet::operator^=(const BitSet& other) {
    if (size != other.size) {
        throw std::invalid_argument("Different sizes");
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


BitSet::BitHolder::BitHolder(BitSet& bs_, const uint id_) :
    bs(bs_),
    id(id_) {}


BitSet::BitHolder& BitSet::BitHolder::operator=(const bool val) {
    uint* unit = bs.data + id / bs.unit_size;
    const uint mask = bs.first_bit >> (id % bs.unit_size);

    if (val) {
        *unit |= mask;
    }
    else {
        *unit &= ~mask;
    }

    return *this;
}


BitSet::BitHolder::operator bool() const {
    return bs.data[id / bs.unit_size] & (bs.first_bit >> (id % bs.unit_size));
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


std::istream& operator>>(std::istream& is, BitSet& bs) {
    std::string input;
    is >> input;

    bs.Clear();
    bs.Resize(input.size());

    for (int i = 0; i < input.size(); ++i) {
        switch (input[i]) {
            case '0':
            bs[i] = false;
            break;

            case '1':
            bs[i] = true;
            break;

            default:
            is.setstate(std::ios_base::failbit);
            return is;
            break;
        }
    }

    return is;
}


std::ostream& operator<<(std::ostream& os, const BitSet& bs) {
    for(int i = 0; i < bs.Size(); ++i) {
        os << bs[i];
    }

    return os;
}
