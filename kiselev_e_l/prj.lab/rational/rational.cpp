#include <iostream>
#include <cmath>

#include "rational.h"


Rational::Rational(const int _nmr, const int _dmr) :
    nmr(_nmr),
    dmr(_dmr)
{
    if (dmr == 0)
    {
        throw DenominatorIsZero();
    }

    shorten();
}


const Rational& Rational::operator+=(const Rational& r)
{
    if (dmr == r.dmr)
    {
        nmr += r.nmr;
    }
    else
    {
        nmr *= r.dmr;
        nmr += (r.nmr * dmr);
        dmr *= r.dmr;
    }

    shorten();

    return *this;
}


const Rational& Rational::operator-=(const Rational& r)
{
    if (dmr == r.dmr)
    {
        nmr -= r.nmr;
    }
    else
    {
        nmr *= r.dmr;
        nmr -= (r.nmr * dmr);
        dmr *= r.dmr;
    }

    shorten();

    return *this;
}


const Rational& Rational::operator*=(const Rational& r)
{
    nmr *= r.nmr;
    dmr *= r.dmr;

    shorten();

    return *this;
}


const Rational& Rational::operator/=(const Rational& r)
{
    if (0 == r.nmr)
    {
        throw DenominatorIsZero();
    }

    nmr *= r.dmr;
    dmr *= r.nmr;

    shorten();

    return *this;
}


Rational Rational::operator-()
{
    return Rational(-nmr, dmr);
}


bool Rational::operator==(const Rational& r) const
{
    return (nmr == r.nmr) && (dmr == r.dmr);
}


bool Rational::operator!=(const Rational& r) const
{
    return (nmr != r.nmr) || (dmr != r.dmr);
}


bool Rational::operator>=(const Rational& r) const
{
    return (*this > r) || (*this == r);
}


bool Rational::operator<=(const Rational& r) const
{
    return (*this < r) || (*this == r);;
}


bool Rational::operator>(const Rational& r) const
{
    return (nmr * r.dmr) > (r.nmr * dmr);
}


bool Rational::operator<(const Rational& r) const
{
    return (nmr * r.dmr) < (r.nmr * dmr);
}


double Rational::getReal() const
{
    return static_cast<double>(nmr) / dmr;
}


int Rational::getNum() const
{
    return nmr;
}


int Rational::getDen() const
{
    return dmr;
}


void Rational::shorten()
{
    if (dmr < 0)
    {
        dmr = -dmr;
        nmr = -nmr;
    }

    int n = abs(nmr);
    int d = dmr;

    while (n != 0 && d != 0)
    {
        if (d > n)
        {
            d %= n;
        }
        else
        {
            n %= d;
        }
    }

    int gcd = n + d;

    nmr /= gcd;
    dmr /= gcd;
}


Rational operator+(const Rational& l, const Rational& r)
{
    return Rational(l) += r;
}


Rational operator-(const Rational& l, const Rational& r)
{
    return Rational(l) -= r;
}


Rational operator*(const Rational& l, const Rational& r)
{
    return Rational(l) *= r;
}


Rational operator/(const Rational& l, const Rational& r)
{
    return Rational(l) /= r;
}


std::ostream& operator<<(std::ostream& os, const Rational& r)
{
    os << r.getNum() << '/' << r.getDen();
    return os;
}


std::istream& operator>>(std::istream& is, Rational& r)
{
    int num = 0;
    int den = 1;
    is >> num >> den;
    r = Rational(num, den);
    return is;
}


const char* DenominatorIsZero::what() const throw()
{
    return "Dividing by zero";
}