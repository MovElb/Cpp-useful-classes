#include <iostream>
#include <cmath>

class Complex {
 private:
    double x, y;

 public:
    Complex(double a, double b): x(a), y(b) {
    }

    explicit Complex(double a): x(a) {
        y = 0;
    }

    double Re() const {
        return x;
    }

    double Im() const {
        return y;
    }

    double abs() const {
        return std::sqrt(Re() * Re() + Im() * Im());
    }
};

Complex operator + (const Complex& z) {
    return z;
}

Complex operator - (const Complex& z) {
    return Complex(-z.Re(), -z.Im());
}

Complex operator + (const Complex& a, const Complex& b) {
    return Complex(a.Re() + b.Re(), a.Im() + b.Im());
}

Complex operator - (const Complex& a, const Complex& b) {
    return Complex(a.Re() - b.Re(), a.Im() - b.Im());
}

Complex operator * (const Complex& a, const Complex& b) {
    return Complex(a.Re() * b.Re() - a.Im() * b.Im(),
                   a.Re() * b.Im() + a.Im() * b.Re());
}

template <typename scal>
Complex operator * (scal a, const Complex& b) {
    return Complex(a * b.Re(), a * b.Im());
}

template <typename scal>
Complex operator * (const Complex& b, scal a) {
    return Complex(a * b.Re(), a * b.Im());
}

Complex operator / (const Complex& a, const Complex& b) {
    double real = a.Re() * b.Re() + a.Im() * b.Im();
    double imag = a.Im() * b.Re() - a.Re() * b.Im();
    double denom = b.Re() * b.Re() + b.Im() * b.Im();
    return Complex(real / denom, imag / denom);
}

bool operator == (const Complex& a, const Complex& b) {
    return a.Re() == b.Re() && a.Im() == b.Im();
}

bool operator != (const Complex& a, const Complex& b) {
    return a.Re() != b.Re() || a.Im() != b.Im();
}

double abs(const Complex& a) {
    return std::sqrt(a.Re() * a.Re() + a.Im() * a.Im());
}

std::ostream& operator << (std::ostream& out, const Complex& z) {
    std::string Im = "";
    if (z.Im() != 0.0) {
        std::string i = "i";
        if (std::abs(z.Im() - 1) > 1e-8) {
            i = std::to_string(z.Im()) + i;
        }
        i = ' ' + i;
        if (z.Im() > 0) {
            Im = " +" + i;
        } else {
            Im = " -" + i;
        }
    }
    out << z.Re() << Im;
    return out;
}
