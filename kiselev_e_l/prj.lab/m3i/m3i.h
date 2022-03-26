#ifndef LAB_M3I_H_
#define LAB_M3I_H_

#include <atomic>
#include <initializer_list>
#include <iostream>
#include <mutex>

class M3i {
public:
    M3i();
    M3i(const int x, const int y, const int z);
    M3i(const std::initializer_list<std::initializer_list<std::initializer_list<int>>>& list_3d);

    M3i(const M3i& other);
    M3i& operator=(const M3i& other);

    M3i(M3i&& other);
    M3i& operator=(M3i&& other);
    
    ~M3i();

    M3i Clone() const;
    
    void Resize(const int x, const int y, const int z);
    
    int& At(const int x, const int y, const int z);
    int At(const int x, const int y, const int z) const;
    
    int Size(const int dim) const;
    
    void Fill(const int val);

    std::istream& ReadFrom(std::istream& is);
    std::ostream& WriteTo(std::ostream& os) const noexcept;

    class WrongSize: public std::exception {
        virtual const char* what() const throw();
    };

private:
    // Clear this copy
    void Clear();

    inline int Volume() const noexcept {
        return ptr->size[0] * ptr->size[1] * ptr->size[2];
    }

    struct SharedData {
        SharedData(int* _data, const int x, const int y, const int z, const int _ref_count);

        int* data = nullptr;
        int size[3] = {0, 0, 0};
        std::atomic<int> ref_count{0}; // Сделать счетчик атомарным
        std::mutex data_mutex;
    };

    SharedData* ptr = nullptr;
};

std::istream& operator>>(std::istream& is, M3i& r);
std::ostream& operator<<(std::ostream& os, const M3i& r) noexcept;

#endif // LAB_M3I_H_