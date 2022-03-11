#include "m3i.h"

#include <algorithm>


M3i::Shared_data::Shared_data(int *_data, const int x, const int y, const int z, const int _ref_count) :
    data(_data),
    size{x, y, z},
    ref_count{_ref_count} {} 


M3i::M3i() :
    ptr(new Shared_data(nullptr, 0, 0, 0, 1)) {}


M3i::M3i(const int x, const int y, const int z) :
    ptr(new Shared_data(new int[x * y * z], x, y, z, 1)) {
    if (x <= 0 || y <= 0 || z <= 0) {
        throw WrongSize();
    }

    std::lock_guard<std::mutex> guard(ptr->data_mutex);
    fill(0);
}


M3i::M3i(const M3i& other) :
    ptr(other.ptr) {
    ptr->ref_count++;
}


M3i& M3i::operator=(const M3i& other) {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    clear();

    ptr = other.ptr;
    ptr->ref_count++;

    return *this;
}


M3i::M3i(M3i&& other) :
    ptr(other.ptr) {
    ptr->ref_count++;

    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    other.clear();
}


M3i& M3i::operator=(M3i&& other) {
    std::lock_guard<std::mutex> guard_1(ptr->data_mutex);

    clear();

    ptr = other.ptr;
    ptr->ref_count++;

    std::lock_guard<std::mutex> guard_2(ptr->data_mutex);

    other.clear();

    return *this;
}


M3i::~M3i() {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);
    clear();
}


M3i M3i::clone() const {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    M3i other(ptr->size[0], ptr->size[1], ptr->size[2]);

    for (int i = 0; i < volume(); ++i) {
        other.ptr->data[i] = ptr->data[i];
    }

    return other;
}


void M3i::resize(const int x, const int y, const int z) {
    if (x <= 0 || y <= 0 || z <= 0) {
        throw WrongSize();
    }

    std::lock_guard<std::mutex> guard(ptr->data_mutex);

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
    std::lock_guard<std::mutex> guard(ptr->data_mutex);
    return ptr->data[x * ptr->size[1] * ptr->size[2] + y * ptr->size[2] + z];
}


int M3i::at(const int x, const int y, const int z) const {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);
    return ptr->data[x * ptr->size[1] * ptr->size[2] + y * ptr->size[2] + z];
}


int M3i::size(const int dim) const {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);
    return ptr->size[dim];
}


void M3i::fill(const int val) {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    for (int i = 0; i < volume(); ++i) {
        ptr->data[i] = val;
    }
}


std::istream& M3i::read_from(std::istream& is) {
    int size[3] = {};

    is >> size[0] >> size[1] >> size[2];
    
    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    resize(size[0], size[1], size[2]);

    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                is >> at(x_id, y_id, z_id);
            }
        }
    }

    return is;
}


std::ostream& M3i::write_to(std::ostream& os) const noexcept {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);
    
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


const char* M3i::WrongSize::what() const throw() {
    return "Wrong size";
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


std::istream& operator>>(std::istream& is, M3i& r) {
    r.read_from(is);
    return is;
}


std::ostream& operator<<(std::ostream& os, const M3i& r) noexcept {
    r.write_to(os);
    return os;
}
