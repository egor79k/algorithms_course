#ifndef RATIONAL_H
#define RATIONAL_H

#include <exception>


class Rational
{
public:
    Rational() = default;
    Rational(const Rational&) = default;
    Rational(Rational&&) = default;
    Rational(const int _nmr, const int _dmr=1);
    Rational& operator=(const Rational&) = default;
    Rational& operator=(Rational&&) = default;
    ~Rational() = default;

    const Rational& operator+=(const Rational& r);
    const Rational& operator-=(const Rational& r);
    const Rational& operator*=(const Rational& r);
    const Rational& operator/=(const Rational& r);

    Rational operator-();

    bool operator==(const Rational& r) const;
    bool operator!=(const Rational& r) const;
    bool operator>=(const Rational& r) const;
    bool operator<=(const Rational& r) const;
    bool operator>(const Rational& r) const;
    bool operator<(const Rational& r) const;

    double getReal() const;
    int getNum() const;
    int getDen() const;

private:
    void shorten();

    int nmr = 0;
    int dmr = 1; // > 0
};


Rational operator+(const Rational& l, const Rational& r);
Rational operator-(const Rational& l, const Rational& r);
Rational operator*(const Rational& l, const Rational& r);
Rational operator/(const Rational& l, const Rational& r);

std::ostream& operator<<(std::ostream& os, const Rational& r);
std::istream& operator>>(std::istream& is, Rational& r);


class DenominatorIsZero: public std::exception
{
  virtual const char* what() const throw();
};

#endif // RATIONAL_H