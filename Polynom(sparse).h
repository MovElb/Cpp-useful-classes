#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

template <typename T>
class Polynomial {
 private:
    std::map<int, T> coef;

    std::pair<Polynomial<T>, Polynomial<T>>
    div_and_rem(const Polynomial& num, const Polynomial& denum) const {
        // q - quotient, r - reminder
        Polynomial  q, r = num;
        while (!r.coef.empty() && r.Degree() >= denum.Degree()) {
            int t_deg = r.Degree() - denum.Degree();
            T lead_cfs = r.coef.rbegin()->second / denum.coef.rbegin()->second;

            q.coef[t_deg] += lead_cfs;

            if (q.coef[t_deg] == 0) {
                q.coef.erase(t_deg);
            }

            Polynomial tmp = 0;
            if (lead_cfs != 0) {
                tmp.coef[t_deg] = lead_cfs;
            }

            r -= denum * tmp;
        }
        return std::make_pair(q, r);
    }

    template <typename t_val>
    t_val horners_method(const t_val& other) const {
        t_val ans = t_val();
        for (int i = Degree(); i >= 0; --i) {
            ans *= other;
            if (coef.find(i) != coef.end()) {
                ans += coef.at(i);
            }
        }
        return ans;
    }

 public:
    Polynomial() {
    }

    Polynomial(const std::vector<T>& inp) {
        int ind = 0;
        for (auto el : inp) {
            if (el != 0) {
                coef[ind] = el;
            }
            ++ind;
        }
    }

    Polynomial(const T& scal) {
        if (scal != 0) {
            coef[0] = scal;
        }
    }

    template <typename Iter>
    Polynomial(Iter begin, Iter end) {
        int ind = 0;
        for (auto i = begin; i != end; ++i) {
            if (*i != 0) {
                coef[ind] = *i;
            }
            ++ind;
        }
    }

    bool operator == (const Polynomial<T>& other) {
        return coef == other.coef;
    }

    bool operator != (const Polynomial<T>& other) {
        return coef != other.coef;
    }

    int Degree() const {
        if (coef.empty()) {
            return -1;
        }
        return coef.rbegin()->first;
    }

    const T& operator[] (int deg) const {
        if (coef.find(deg) != coef.end())
            return coef.find(deg)->second;
        return 0;
    }

    T operator ()(const T& scal) const {
        return horners_method(scal);
    }

    Polynomial operator &=(const Polynomial& other) {
        *this = horners_method(other);
        return *this;
    }

    Polynomial operator &(const Polynomial& other) const {
        auto temp = *this;
        temp &= other;
        return temp;
    }

    Polynomial& operator +=(const Polynomial<T>& other) {
        for (std::pair<int, T> kv : other.coef) {
            (*this).coef[kv.first] += kv.second;
            if ((*this).coef[kv.first] == 0) {
                (*this).coef.erase(kv.first);
            }
        }
        return *this;
    }

    Polynomial operator +(const Polynomial<T>& other) const {
        auto temp = *this;
        temp += other;
        return temp;
    }

    Polynomial& operator -=(const Polynomial<T>& other) {
        for (std::pair<int, T> kv : other.coef) {
            (*this).coef[kv.first] -= kv.second;
            if ((*this).coef[kv.first] == 0) {
                (*this).coef.erase(kv.first);
            }
        }
        return *this;
    }

    Polynomial operator -(const Polynomial<T>& other) const {
        auto temp = *this;
        temp -= other;
        return temp;
    }

    Polynomial& operator *=(const Polynomial<T>& other) {
        std::map<int, T> mul;
        for (auto i : this->coef) {
            for (auto j : other.coef) {
                mul[i.first + j.first] += i.second * j.second;
                if (mul[i.first + j.first] == 0) {
                    mul.erase(i.first + j.first);
                }
            }
        }
        coef = mul;
        return *this;
    }

    Polynomial operator *(const Polynomial<T>& other) const {
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

        T lead_c = a.coef.rbegin()->second;
        for (auto i : a.coef) {
            a.coef[i.first] /= lead_c;
            if (a.coef[i.first] == 0) {
                a.coef.erase(i.first);
            }
        }
        return a;
    }

    typename std::map<int, T>::const_iterator begin() const {
        return coef.begin();
    }

    typename std::map<int, T>::const_iterator end() const {
        return coef.end();
    }

    friend std::ostream& operator << (std::ostream& out, const Polynomial<T>& pol) {
        if (pol.Degree() == -1) {
            out << T();
        }

        for (auto i = pol.coef.rbegin(); i != pol.coef.rend(); ++i) {
            if (i->second != T()) {
                if (i->second > 0 && i->first != pol.Degree()) {
                    out << '+';
                }
                if (i->second < 0) {
                    out << '-';
                }

                if (std::abs(i->second) != 1 && i->first != 0) {
                    out << std::abs(i->second) << '*';
                } else if (i->first == 0) {
                    out << std::abs(i->second);
                }

                if (i->first != 0) {
                    out << 'x';
                }

                if (i->first > 1) {
                    out << '^' << i->first;
                }
            }
        }
        return out;
    }
};
