#ifndef LAB_RATIONAL_H_
#define LAB_RATIONAL_H_

#include <exception>
#include <iostream>


class Rational {
public:
    Rational() = default;
    Rational(const Rational&) = default;
    Rational(Rational&&) = default;
    Rational(const int _nmr, const int _dmr=1);
    Rational& operator=(const Rational&) = default;
    Rational& operator=(Rational&&) = default;
    ~Rational() = default;

    double real() const noexcept;
    int num() const noexcept;
    int denum() const noexcept;

    Rational operator-() const noexcept;
    const Rational& operator+=(const Rational& r) noexcept;
    const Rational& operator-=(const Rational& r) noexcept;
    const Rational& operator*=(const Rational& r) noexcept;
    const Rational& operator/=(const Rational& r);

    bool operator==(const Rational& r) const noexcept;
    bool operator!=(const Rational& r) const noexcept;
    bool operator<(const Rational& r) const noexcept;
    bool operator<=(const Rational& r) const noexcept;
    bool operator>(const Rational& r) const noexcept;
    bool operator>=(const Rational& r) const noexcept;
    
    std::istream& read_from(std::istream& is);
    std::ostream& write_to(std::ostream& os) const noexcept;

    class DenominatorIsZero: public std::exception {
        virtual const char* what() const throw();
    };

private:
    void shorten() noexcept;

    int nmr = 0;
    int dmr = 1; // > 0
};


std::istream& operator>>(std::istream& is, Rational& r);
std::ostream& operator<<(std::ostream& os, const Rational& r) noexcept;


inline Rational operator+(const Rational& l, const Rational& r) noexcept {
    return Rational(l) += r;
}


inline Rational operator-(const Rational& l, const Rational& r) noexcept {
    return Rational(l) -= r;
}


inline Rational operator*(const Rational& l, const Rational& r) noexcept {
    return Rational(l) *= r;
}


inline Rational operator/(const Rational& l, const Rational& r) {
    return Rational(l) /= r;
}

#endif // LAB_RATIONAL_H_