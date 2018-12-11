#include <iostream>

class Rational {
 private:
    int num, den;

    int gcd(int a, int b) {
        a = a < 0 ? -a : a;
        b = b < 0 ? -b : b;

        while (b) {
            a %= b;
            int temp = a;
            a = b;
            b = temp;
        }
        return a;
    }

 public:
    Rational(int a = 0, int b = 1) {
        num = b < 0 ? -a : a;
        den = b < 0 ? -b : b;
        int gc = gcd(num, den);
        num /= gc;
        den /= gc;
    }

    int numerator() const {
        return num;
    }

    int denominator() const {
        return den;
    }

    Rational& operator ++() {
        num += den;
        return *this;
    }

    Rational operator ++(int) {
        Rational old = *this;
        ++(*this);
        return old;
    }

    Rational& operator --() {
        num -= den;
        return *this;
    }

    Rational operator --(int) {
        Rational old = *this;
        --(*this);
        return old;
    }
};

Rational operator +(const Rational& rat) {
    return rat;
}

Rational operator -(const Rational& rat) {
    return Rational(-rat.numerator(), rat.denominator());
}

Rational operator +(const Rational& fir, const Rational& sec) {
    return Rational(fir.numerator() * sec.denominator() + fir.denominator() * sec.numerator(),
                    fir.denominator() * sec.denominator());
}

Rational operator +(int intgr, const Rational& rat) {
    return Rational(intgr * rat.denominator() + rat.numerator(), rat.denominator());
}

Rational operator +(const Rational& rat, int intgr) {
    return intgr + rat;
}

Rational operator -(const Rational& fir, const Rational& sec) {
    return fir + (-sec);
}

Rational operator -(int intgr, const Rational& rat) {
    return intgr + (-rat);
}

Rational operator -(const Rational& rat, int intgr) {
    return -intgr + rat;
}

Rational operator *(const Rational& fir, const Rational& sec) {
    return Rational(fir.numerator() * sec.numerator(), fir.denominator() * sec.denominator());
}

Rational operator *(int intgr, const Rational& rat) {
    return Rational(intgr * rat.numerator(), rat.denominator());
}

Rational operator *(const Rational& rat, int intgr) {
    return intgr * rat;
}

Rational operator /(const Rational& fir, const Rational& sec) {
    return Rational(fir.numerator() * sec.denominator(), fir.denominator() * sec.numerator());
}

Rational operator /(int intgr, const Rational& rat) {
    return Rational(intgr * rat.denominator(), rat.numerator());
}

Rational operator /(const Rational& rat, int intgr) {
    return Rational(rat.numerator(), rat.denominator() * intgr);
}

template<typename T>
Rational& operator +=(Rational& rat, T val) {
    rat = rat + val;
    return rat;
}

template<typename T>
Rational& operator -=(Rational& rat, T val) {
    rat = rat - val;
    return rat;
}

template<typename T>
Rational& operator *=(Rational& rat, T val) {
    rat = rat * val;
    return rat;
}

template<typename T>
Rational& operator /=(Rational& rat, T val) {
    rat = rat / val;
    return rat;
}

bool operator ==(const Rational& fir, const Rational& sec) {
    return fir.numerator() == sec.numerator() && fir.denominator() == sec.denominator();
}

bool operator !=(const Rational& fir, const Rational& sec) {
    return fir.numerator() != sec.numerator() || fir.denominator() != sec.denominator();
}

std::ostream& operator <<(std::ostream& out, const Rational& rat) {
    out << rat.numerator() << '/' << rat.denominator();
    return out;
}
