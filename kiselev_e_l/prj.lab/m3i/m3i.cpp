#include <algorithm>
#include <stdexcept>

#include <m3i/m3i.h>



M3i::SharedData::SharedData(int *_data, const int x, const int y, const int z, const int _ref_count) :
    data(_data),
    size{x, y, z},
    ref_count{_ref_count} {} 


M3i::M3i() :
    ptr(new SharedData(nullptr, 0, 0, 0, 1)) {}


M3i::M3i(int x, int y, int z) :
    ptr(new SharedData(new int[x * y * z], x, y, z, 1)) {
    if (x <= 0 || y <= 0 || z <= 0) {
        throw std::invalid_argument("wrong dimentions");
    }
}


M3i::M3i(int x, int y, int z, int val) :
    M3i(x, y, z) {
    Fill(val);
}


M3i::M3i(const std::initializer_list<std::initializer_list<std::initializer_list<int>>>& list_3d) :
    ptr(new SharedData(new int[list_3d.size() * list_3d.begin()->size() * list_3d.begin()->begin()->size()],
        list_3d.size(),
        list_3d.begin()->size(),
        list_3d.begin()->begin()->size(),
        1)) {
    int x_id = 0;

    for (const auto &list_2d : list_3d) {
        int y_id = 0;

        for (const auto &list_1d : list_2d) {
            int z_id = 0;

            for (const auto &val : list_1d) {
                At(x_id, y_id, z_id) = val;

                ++z_id;
            }
            ++y_id;
        }
        ++x_id;
    }
}


M3i::M3i(const M3i& other) :
    ptr(other.ptr) {
    ptr->ref_count++;
}


M3i& M3i::operator=(const M3i& other) {
    Clear();

    ptr = other.ptr;
    ptr->ref_count++;

    return *this;
}


M3i::M3i(M3i&& other) :
    ptr(other.ptr) {
    ptr->ref_count++;

    other.Clear();
}


M3i& M3i::operator=(M3i&& other) {
    Clear();

    ptr = other.ptr;
    ptr->ref_count++;

    other.Clear();

    return *this;
}


M3i::~M3i() {
    Clear();
}


M3i M3i::Clone() const {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    M3i other(ptr->size[0], ptr->size[1], ptr->size[2]);

    for (int i = 0; i < Volume(); ++i) {
        other.ptr->data[i] = ptr->data[i];
    }

    return other;
}


M3i& M3i::Resize(int x, int y, int z) {
    if (x <= 0 || y <= 0 || z <= 0) {
        throw std::invalid_argument("wrong dimentions");
    }

    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    int *old_data = ptr->data;
    int old_size[3] = {ptr->size[0], ptr->size[1], ptr->size[2]};

    if (x > 0 && y > 0 && z > 0) {
        ptr->size[0] = x;
        ptr->size[1] = y;
        ptr->size[2] = z;
        ptr->data = new int[Volume()];

        //Fill(0);
        for (int i = 0; i < Volume(); ++i) {
            ptr->data[i] = 0;
        }

        for (int x_id = 0; x_id < std::min(ptr->size[0], old_size[0]); ++x_id) {
            for (int y_id = 0; y_id < std::min(ptr->size[1], old_size[1]); ++y_id) {
                for (int z_id = 0; z_id < std::min(ptr->size[2], old_size[2]); ++z_id) {
                    At(x_id, y_id, z_id) = old_data[x_id * old_size[1] * old_size[2] + y_id * old_size[2] + z_id];
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

    return *this;
}



int& M3i::At(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x >= ptr->size[0] || y >= ptr->size[1] || z >= ptr->size[2]) {
        throw std::invalid_argument("index is out of range");
    }

    return ptr->data[x * ptr->size[1] * ptr->size[2] + y * ptr->size[2] + z];
}


int M3i::At(int x, int y, int z) const {
    if (x < 0 || y < 0 || z < 0 || x >= ptr->size[0] || y >= ptr->size[1] || z >= ptr->size[2]) {
        throw std::invalid_argument("index is out of range");
    }

    return ptr->data[x * ptr->size[1] * ptr->size[2] + y * ptr->size[2] + z];
}


int M3i::Size(int dim) const {
    if (dim < 0 || dim > 2) {
        throw std::invalid_argument("wrong dimention index");
    }

    return ptr->size[dim];
}


void M3i::Fill(int val) {
    std::lock_guard<std::mutex> guard(ptr->data_mutex);

    for (int i = 0; i < Volume(); ++i) {
        ptr->data[i] = val;
    }
}


std::istream& M3i::ReadFrom(std::istream& is) {
    int size[3] = {};

    is >> size[0] >> size[1] >> size[2];

    Resize(size[0], size[1], size[2]);

    for (int x_id = 0; x_id < size[0]; ++x_id) {
        for (int y_id = 0; y_id < size[1]; ++y_id) {
            for (int z_id = 0; z_id < size[2]; ++z_id) {
                is >> At(x_id, y_id, z_id);
            }
        }
    }

    return is;
}


std::ostream& M3i::WriteTo(std::ostream& os) const noexcept {
    os << ptr->size[0] << " " << ptr->size[1] << " " << ptr->size[2] << std::endl;
    
    for (int x_id = 0; x_id < ptr->size[0]; ++x_id) {
        for (int y_id = 0; y_id < ptr->size[1]; ++y_id) {
            for (int z_id = 0; z_id < ptr->size[2]; ++z_id) {
                os << At(x_id, y_id, z_id) << " ";
            }

            os << "\n";
        }

        os << "\n";
    }

    return os;
}


void M3i::Clear() {
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
    r.ReadFrom(is);
    return is;
}


std::ostream& operator<<(std::ostream& os, const M3i& r) noexcept {
    r.WriteTo(os);
    return os;
}
