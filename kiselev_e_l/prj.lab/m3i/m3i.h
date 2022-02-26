#ifndef LAB_M3I_H_
#define LAB_M3I_H_

#include <iostream>

class M3i {
public:
    M3i();
    M3i(const int x, const int y, const int z);

    M3i(const M3i& other);
    M3i& operator=(const M3i& other);

    M3i(M3i&& other);
    M3i& operator=(M3i&& other);
    
    ~M3i();

    M3i clone() const;
    
    void resize(const int x, const int y, const int z);
    
    int& at(const int x, const int y, const int z);
    int at(const int x, const int y, const int z) const;
    
    int size(const int dim);
    
    void fill(const int val);

    std::istream& read_from(std::istream& is);
    std::ostream& write_to(std::ostream& os) const noexcept;

private:
    // Clear this copy
    void clear();

    inline int volume() const noexcept {
        return ptr->size[0] * ptr->size[1] * ptr->size[2];
    }

    struct Shared_data {
        int* data = nullptr;
        int size[3] = {0, 0, 0};
        //Shape size = {0, 0, 0};
        int ref_count = 0;
    };

    Shared_data* ptr = nullptr;
};

#endif // LAB_M3I_H_