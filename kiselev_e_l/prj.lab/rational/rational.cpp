#include <cmath>

#include <rational/rational.h>


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


Rational& Rational::operator+=(const Rational& r) noexcept {
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


Rational& Rational::operator-=(const Rational& r) noexcept {
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


Rational& Rational::operator*=(const Rational& r) noexcept {
    nmr *= r.nmr;
    dmr *= r.dmr;

    shorten();

    return *this;
}


Rational& Rational::operator/=(const Rational& r) {
    if (0 == r.nmr) {
        throw DenominatorIsZero();
    }

    nmr *= r.dmr;
    dmr *= r.nmr;

    shorten();

    return *this;
}


Rational& Rational::operator+=(const int r) noexcept {
    return *this += Rational(r);
}


Rational& Rational::operator-=(const int r) noexcept {
    return *this -= Rational(r);
}


Rational& Rational::operator*=(const int r) noexcept {
    return *this *= Rational(r);
}


Rational& Rational::operator/=(const int r) {
    return *this /= Rational(r);
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
    is >> nmr;

    if (is.get() != '/' || !isdigit(is.peek())) {
        is.setstate(std::ios_base::failbit);
    }

    is >> dmr;

    if (dmr <= 0) {
        is.setstate(std::ios_base::failbit);
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
