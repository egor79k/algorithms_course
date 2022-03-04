#include "rational.h"

#include <cmath>


Rational::Rational(const int _nmr, const int _dmr) :
    nmr(_nmr),
    dmr(_dmr) {
    if (dmr == 0) {
        throw DenominatorIsZero();
    }

    shorten();
}


double Rational::real() const noexcept {
    return static_cast<double>(nmr) / dmr;
}


int Rational::num() const noexcept {
    return nmr;
}


int Rational::denum() const noexcept {
    return dmr;
}


Rational Rational::operator-() const noexcept {
    return Rational(-nmr, dmr);
}


const Rational& Rational::operator+=(const Rational& r) noexcept {
    if (dmr == r.dmr) {
        nmr += r.nmr;
    }
    else {
        nmr *= r.dmr;
        nmr += (r.nmr * dmr);
        dmr *= r.dmr;
    }

    shorten();

    return *this;
}


const Rational& Rational::operator-=(const Rational& r) noexcept {
    if (dmr == r.dmr) {
        nmr -= r.nmr;
    }
    else {
        nmr *= r.dmr;
        nmr -= (r.nmr * dmr);
        dmr *= r.dmr;
    }

    shorten();

    return *this;
}


const Rational& Rational::operator*=(const Rational& r) noexcept {
    nmr *= r.nmr;
    dmr *= r.dmr;

    shorten();

    return *this;
}


const Rational& Rational::operator/=(const Rational& r) {
    if (0 == r.nmr) {
        throw DenominatorIsZero();
    }

    nmr *= r.dmr;
    dmr *= r.nmr;

    shorten();

    return *this;
}


bool Rational::operator==(const Rational& r) const noexcept {
    return (nmr == r.nmr) && (dmr == r.dmr);
}


bool Rational::operator!=(const Rational& r) const noexcept {
    return (nmr != r.nmr) || (dmr != r.dmr);
}


bool Rational::operator<(const Rational& r) const noexcept {
    return (nmr * r.dmr) < (r.nmr * dmr);
}


bool Rational::operator<=(const Rational& r) const noexcept {
    return (*this < r) || (*this == r);;
}


bool Rational::operator>(const Rational& r) const noexcept {
    return (nmr * r.dmr) > (r.nmr * dmr);
}


bool Rational::operator>=(const Rational& r) const noexcept {
    return (*this > r) || (*this == r);
}


std::istream& Rational::read_from(std::istream& is) {
    char sep = 0;
    is >> nmr;

    if (is.get() != '/' || !isdigit(is.peek())) {
        throw FormatError();
    }

    is >> dmr;

    if (dmr <= 0) {
        throw FormatError();
    }

    shorten();

    return is;
}


std::ostream& Rational::write_to(std::ostream& os) const noexcept {
    os << nmr << '/' << dmr;
    return os;
}


const char* Rational::DenominatorIsZero::what() const throw() {
    return "Dividing by zero";
}


const char* Rational::FormatError::what() const throw() {
    return "Wrong format";
}


void Rational::shorten() noexcept {
    if (dmr < 0) {
        dmr = -dmr;
        nmr = -nmr;
    }

    int n = abs(nmr);
    int d = dmr;

    while (n != 0 && d != 0) {
        if (d > n) {
            d %= n;
        }
        else {
            n %= d;
        }
    }

    int gcd = n + d;

    nmr /= gcd;
    dmr /= gcd;
}


std::istream& operator>>(std::istream& is, Rational& r) {
    r.read_from(is);
    return is;
}


std::ostream& operator<<(std::ostream& os, const Rational& r) noexcept {
    r.write_to(os);
    return os;
}
