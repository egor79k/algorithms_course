#include "m3i.h"

#include <algorithm>


M3i::M3i() :
    ptr(new Shared_data{nullptr, {0, 0, 0}, 1})
{}


M3i::M3i(const int x, const int y, const int z) :
    ptr(new Shared_data{new int[x * y * z], {x, y, z}, 1}) {
    fill(0);
}


M3i::M3i(const M3i& other) :
    ptr(other.ptr) {
    ptr->ref_count++;
}


M3i& M3i::operator=(const M3i& other) {
    clear();

    ptr = other.ptr;
    ptr->ref_count++;

    return *this;
}


M3i::M3i(M3i&& other) :
    ptr(other.ptr) {
    ptr->ref_count++;
    other.clear();
}


M3i& M3i::operator=(M3i&& other) {
    clear();
    
    ptr = other.ptr;
    ptr->ref_count++;
    other.clear();

    return *this;
}


M3i::~M3i() {
    clear();
}


M3i M3i::clone() const {
    M3i other(ptr->size[0], ptr->size[1], ptr->size[2]);

    for (int i = 0; i < volume(); ++i) {
        other.ptr->data[i] = ptr->data[i];
    }

    return other;
}


void M3i::resize(const int x, const int y, const int z) {
    int *old_data = ptr->data;
    int old_size[3] = {ptr->size[0], ptr->size[1], ptr->size[2]};

    if (x > 0 && y > 0 && z > 0) {
        ptr->size[0] = x;
        ptr->size[1] = y;
        ptr->size[2] = z;
        ptr->data = new int[volume()];

        fill(0);

        for (int x_id = 0; x_id < std::min(ptr->size[0], old_size[0]); ++x_id) {
            for (int y_id = 0; y_id < std::min(ptr->size[1], old_size[1]); ++y_id) {
                for (int z_id = 0; z_id < std::min(ptr->size[2], old_size[2]); ++z_id) {
                    at(x_id, y_id, z_id) = old_data[x_id * old_size[1] * old_size[2] + y_id * old_size[2] + z_id];
                }
            }
        }
    }
    else {
        ptr->size[0] = 0;
        ptr->size[1] = 0;
        ptr->size[2] = 0;
        ptr->data = nullptr;
    }

    delete[] old_data;
}



int& M3i::at(const int x, const int y, const int z) {
    return ptr->data[x * ptr->size[1] * ptr->size[2] + y * ptr->size[2] + z];
}


int M3i::at(const int x, const int y, const int z) const {
    return ptr->data[x * ptr->size[1] * ptr->size[2] + y * ptr->size[2] + z];
}


int M3i::size(const int dim) {
    return ptr->size[dim];
}


void M3i::fill(const int val) {
    for (int i = 0; i < volume(); ++i) {
        ptr->data[i] = val;
    }
}


std::istream& M3i::read_from(std::istream& is) {
    return is;
}


std::ostream& M3i::write_to(std::ostream& os) const noexcept {
    for (int x_id = 0; x_id < ptr->size[0]; ++x_id) {
        for (int y_id = 0; y_id < ptr->size[1]; ++y_id) {
            for (int z_id = 0; z_id < ptr->size[2]; ++z_id) {
                os << at(x_id, y_id, z_id) << " ";
            }

            os << "\n";
        }

        os << "\n";
    }

    return os;
}


void M3i::clear() {
    if (ptr == nullptr) {
        return;
    }

    ptr->ref_count--;

    if (0 == ptr->ref_count) {
        if (ptr->data != nullptr) {
            delete[] ptr->data;
        }

        delete ptr;
    }

    ptr = nullptr;
}
