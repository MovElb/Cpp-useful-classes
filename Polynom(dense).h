#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <sstream>

template <typename T>
class Polynomial {
 private:
    std::vector<T> coef;

    void del_lead_zeros() {
        while (coef.size() > 0 && coef.back() == 0) {
            coef.pop_back();
        }
    }

    std::pair<Polynomial<T>, Polynomial<T>>
    div_and_rem(const Polynomial& num, const Polynomial& denum) const {
        // q - quotient, r - reminder
        Polynomial q, r = num;

        while (!r.coef.empty() && r.Degree() >= denum.Degree()) {
            int t_deg = r.Degree() - denum.Degree();
            T lead_cfs = r.coef.back() / denum.coef.back();

            std::vector<T> null(num.Degree() - denum.Degree() + 1);
            null[t_deg] = lead_cfs;

            if (q.Degree() == -1) {
                q = Polynomial(null);
            } else {
                q.coef[t_deg] += lead_cfs;
            }

            Polynomial tmp(null);
            r -= denum * tmp;
        }

        r.del_lead_zeros();
        q.del_lead_zeros();
        return std::make_pair(q, r);
    }

 public:
    Polynomial() {
        del_lead_zeros();
    }

    Polynomial(const std::vector<T>& inp) : coef(inp) {
        del_lead_zeros();
    }

    Polynomial(const T& val) {
        coef.push_back(val);
        del_lead_zeros();
    }

    template <typename It>
    Polynomial(const It& begin, const It& end) {
        std::copy(begin, end, std::back_inserter(coef));
        del_lead_zeros();
    }

    bool operator ==(const Polynomial& other) const {
        return (*this).coef == other.coef;
    }

    bool operator !=(const Polynomial& other) const {
        return (*this).coef != other.coef;
    }

    int Degree() const {
        return static_cast<int>(this->coef.size()) - 1;
    }

    T operator[](int degr) const {
        if (degr <= (*this).Degree())
            return this->coef[degr];
        else
            return 0;
    }

    T operator ()(const T& scal) const {
        T ans = T();
        for (int i = Degree(); i >= 0; --i) {
            ans *= scal;
            ans += coef[i];
        }
        return ans;
    }

    Polynomial& operator &=(const Polynomial& other) {
        Polynomial ans;
        for (int i = Degree(); i >= 0; --i) {
            ans *= other;
            ans += coef[i];
        }
        *this = ans;
        return *this;
    }

    Polynomial operator &(const Polynomial& other) const {
        auto temp = *this;
        temp &= other;
        return temp;
    }

    Polynomial& operator +=(const Polynomial& other) {
        for (size_t i = 0; i < std::max(coef.size(), other.coef.size()); i++) {
            if (i >= coef.size()) {
                coef.emplace_back(other.coef[i]);
            } else if (i < other.coef.size()) {
                coef[i] += other.coef[i];
            }
        }

        del_lead_zeros();
        return (*this);
    }

    Polynomial operator +(const Polynomial& other) const {
        auto temp = *this;
        temp += other;
        return temp;
    }

    Polynomial& operator -=(const Polynomial& other) {
        for (size_t i = 0; i < std::max(coef.size(), other.coef.size()); i++) {
            if (i >= coef.size()) {
                coef.emplace_back(-other.coef[i]);
            } else if (i < other.coef.size()) {
                coef[i] -= other.coef[i];
            }
        }

        del_lead_zeros();
        return (*this);
    }

    Polynomial operator -(const Polynomial& other) const {
        auto temp = *this;
        temp -= other;
        return temp;
    }

    Polynomial& operator *=(const Polynomial& other) {
        std::vector<T> mul(coef.size() + other.coef.size());

        for (int i = 0; i != coef.size(); ++i) {
            for (int j = 0; j != other.coef.size(); ++j) {
                mul[i + j] += coef[i] * other[j];
            }
        }

        coef = mul;
        del_lead_zeros();
        return *this;
    }

    Polynomial operator *(const Polynomial& other) const {
        auto temp = *this;
        temp *= other;
        return temp;
    }

    Polynomial& operator /=(const Polynomial& d) {
        *this = div_and_rem(*this, d).first;
        return *this;
    }

    Polynomial operator /(const Polynomial& d) const {
        return div_and_rem(*this, d).first;
    }

    Polynomial& operator %=(const Polynomial& d) {
        *this = div_and_rem(*this, d).second;
        return *this;
    }

    Polynomial operator %(const Polynomial& d) const {
        return div_and_rem(*this, d).second;
    }

    Polynomial operator ,(const Polynomial& other) const {
        auto a = *this, b = other;
        while (b.Degree() >= 0) {
            a %= b;
            std::swap(a, b);
        }

        T lead_c = a.coef.back();
        a.del_lead_zeros();
        for (int i = 0; i != a.Degree() + 1; ++i) {
            a.coef[i] /= lead_c;
        }
        return a;
    }

    typename std::vector<T>::const_iterator begin() const {
        return coef.begin();
    }

    typename std::vector<T>::const_iterator end() const {
        return coef.end();
    }
};

template <typename T>
std::ostream& operator << (std::ostream& out, const Polynomial<T>& pol) {
    if (pol.Degree() == -1) {
        out << T();
    }

    for (int i = pol.Degree(); i >= 0; --i) {
        if (pol[i] != T()) {
            if (pol[i] > 0 && i != pol.Degree()) {
                out << '+';
            }
            if (pol[i] < 0) {
                out << '-';
            }

            if (pol[i] != 1 && pol[i] != -1 && i != 0) {
                out << (pol[i] > 0 ? pol[i] : -pol[i]) << '*';
            } else if (i == 0) {
                out << (pol[i] > 0 ? pol[i] : -pol[i]);
            }

            if (i != 0) {
                out << 'x';
            }

            if (i > 1) {
                out << '^' << i;
            }
        }
    }
    return out;
}
