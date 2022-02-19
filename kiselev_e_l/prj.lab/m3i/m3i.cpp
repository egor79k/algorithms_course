#include "m3i.h"

#include <algorithm>


M3i::M3i(const M3i& other) :
    size(other.size) {
    if (other.data != nullptr) {
        data = new int[size.volume()];

        for (int i = 0; i < size.volume(); ++i) {
            data[i] = other.data[i];
        }
    }
}


M3i::M3i(M3i&& other) :
    size(other.size),
    data(other.data) {
    other.clear();
}


M3i::M3i(const int x, const int y, const int z, const int val) :
    data(new int[x * y * z]),
    size({x, y, z}) {
    fill(val);
}


M3i& M3i::operator=(const M3i& other) {
    clear();

    size = other.size;

    if (other.data != nullptr) {
        data = new int[size.volume()];

        for (int i = 0; i < size.volume(); ++i) {
            data[i] = other.data[i];
        }
    }

    return *this;
}


M3i& M3i::operator=(M3i&& other) {
    clear();
    size = other.size;
    data = other.data;
    other.clear();
    return *this;
}


M3i::~M3i() {
    clear();
}


void M3i::clear() {
    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }

    size = {0, 0, 0};
}


void M3i::fill(const int val) {
    for (int i = 0; i < size.volume(); ++i) {
        data[i] = val;
    }
}


int& M3i::at(const int x, const int y, const int z) {
    return data[x * size.y * size.z + y * size.z + z];
}


const int& M3i::at(const int x, const int y, const int z) const {
    return data[x * size.y * size.z + y * size.z + z];
}


void M3i::resize(const int x, const int y, const int z, const int val) {
    int *old_data = data;
    Shape old_size = size;

    if (x > 0 && y > 0 && z > 0) {
        size = {x, y, z};
        data = new int[size.volume()];

        fill(val);

        for (int x_id = 0; x_id < std::min(size.x, old_size.x); ++x_id) {
            for (int y_id = 0; y_id < std::min(size.y, old_size.y); ++y_id) {
                for (int z_id = 0; z_id < std::min(size.z, old_size.z); ++z_id) {
                    at(x_id, y_id, z_id) = old_data[x_id * old_size.y * old_size.z + y_id * old_size.z + z_id];
                }
            }
        }
    }
    else {
        size = {0, 0, 0};
        data = nullptr;
    }

    delete[] old_data;
}


std::istream& M3i::read_from(std::istream& is) {
    return is;
}


std::ostream& M3i::write_to(std::ostream& os) const noexcept {
    for (int x_id = 0; x_id < size.x; ++x_id) {
        for (int y_id = 0; y_id < size.y; ++y_id) {
            for (int z_id = 0; z_id < size.z; ++z_id) {
                os << at(x_id, y_id, z_id) << " ";
            }

            os << "\n";
        }

        os << "\n";
    }

    return os;
}


inline int M3i::Shape::volume() {
    return x * y * z;
}
