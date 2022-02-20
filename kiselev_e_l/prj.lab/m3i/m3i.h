#ifndef LAB_M3I_H_
#define LAB_M3I_H_

#include <iostream>

class M3i {
public:
    struct Shape {
        inline int volume() const noexcept {
            return x * y * z;
        }

        int x = 0;
        int y = 0;
        int z = 0;
    };

    M3i() = default;
    M3i(const M3i& other);
    M3i(M3i&& other);
    M3i(const int x, const int y, const int z, const int val=0);
    M3i(const Shape& sz, const int val=0);
    M3i& operator=(const M3i& other);
    M3i& operator=(M3i&& other);
    ~M3i();

    void clear();
    void fill(const int val);
    int& at(const int x, const int y, const int z);
    const int& at(const int x, const int y, const int z) const;
    void resize(const int x, const int y, const int z, const int val=0);
    const Shape& get_size();

    std::istream& read_from(std::istream& is);
    std::ostream& write_to(std::ostream& os) const noexcept;

private:
    int *data = nullptr;
    Shape size = {0, 0, 0};
};


inline bool operator==(const M3i::Shape& l, const M3i::Shape& r) {
    return (l.x == r.x && l.y == r.y && l.z == r.z);
}

#endif // LAB_M3I_H_