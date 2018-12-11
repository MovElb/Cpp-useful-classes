#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>

template <typename T>
class Matrix;

template <typename T>
class Matrix_iter {
 private:
    Matrix<T>& matrix;
    size_t i, j;

 public:
    Matrix_iter(Matrix<T>& in_matr, size_t _i, size_t _j) : matrix(in_matr), i(_i), j(_j) {
    }

    bool operator ==(const Matrix_iter& other) const {
        return (*this).i == other.i && (*this).j == other.j;
    }

    bool operator !=(const Matrix_iter& other) const {
        return !(*this == other);
    }

    T& operator *() {
        return matrix(i, j);
    }

    Matrix_iter& operator ++() {
        ++j;
        if (j == matrix.size().second) {
            j = 0;
            ++i;
        }
        return *this;
    }

    Matrix_iter operator ++(int) {
        Matrix_iter temp = *this;
        ++(*this);
        return temp;
    }
};

template <typename T>
class Matrix_c_iter {
 private:
    Matrix<T> matrix;
    size_t i, j;

 public:
    Matrix_c_iter(const Matrix<T>& in_matr, size_t _i, size_t _j) : matrix(in_matr), i(_i), j(_j) {
    }

    bool operator ==(const Matrix_c_iter& other) const {
        return (*this).i == other.i && (*this).j == other.j;
    }

    bool operator !=(const Matrix_c_iter& other) const {
        return !(*this == other);
    }

    T& operator *() {
        return matrix(i, j);
    }

    Matrix_c_iter& operator ++() {
        ++j;
        if (j == matrix.size().second) {
            j = 0;
            ++i;
        }
        return *this;
    }

    Matrix_c_iter operator ++(int) {
        Matrix_c_iter temp = *this;
        ++(*this);
        return temp;
    }
};


template <typename T>
class Matrix{
 private:
    std::vector<std::vector<T>> matr;

 public:
    Matrix(const std::vector<std::vector<T>>& d) : matr(d) {
    }

    Matrix_iter<T> begin() {
        return Matrix_iter<T>(*this, 0, 0);
    }

    Matrix_c_iter<T> begin() const {
        return Matrix_c_iter<T>(*this, 0, 0);
    }

    Matrix_iter<T> end() {
        return Matrix_iter<T>(*this, (*this).size().first, 0);
    }

    Matrix_c_iter<T> end() const {
        return Matrix_c_iter<T>(*this, (*this).size().first, 0);
    }

    T& operator() (size_t i, size_t j) {
        return matr[i][j];
    }

    const T& operator() (size_t i, size_t j) const {
        return matr[i][j];
    }

    std::pair<size_t, size_t> size() const {
        return std::make_pair(static_cast<size_t >(matr.size()),
                              static_cast<size_t >(matr[0].size()));
    }

    Matrix& transpose() {
        std::vector<std::vector<T>> m((*this).size().second, std::vector<T>((*this).size().first));
        for (size_t i = 0; i != (*this).size().first; ++i) {
            for (size_t j = 0; j != (*this).size().second; ++j) {
                m[j][i] = (*this)(i, j);
            }
        }
        matr = m;
        return *this;
    }

    Matrix transposed() const {
        Matrix temp = *this;
        return temp.transpose();
    }

    Matrix<T>& operator +=(const Matrix<T>& other) {
        for (size_t i = 0; i != (*this).size().first; ++i) {
            for (size_t j = 0; j != (*this).size().second; ++j) {
                (*this)(i, j) += other(i, j);
            }
        }
        return *this;
    }

    Matrix<T> operator +(const Matrix<T>& other) const {
        Matrix<T> temp= *this;
        temp += other;
        return temp;
    }

    template <typename scal>
    Matrix<T>& operator *= (const scal& val) {
        for (size_t i = 0; i != (*this).size().first; ++i) {
            for (size_t j = 0; j != (*this).size().second; ++j) {
                (*this)(i, j) *= val;
            }
        }
        return *this;
    }

    template <typename scal>
    Matrix<T> operator *(const scal& val) const {
        Matrix<T> temp = *this;
        temp *= val;
        return temp;
    }

    Matrix<T>& operator *=(const Matrix<T>& other) {
        std::vector<std::vector<T>> t_matr((*this).size().first,
                                           std::vector<T>(other.size().second));

        for (size_t i = 0; i != (*this).size().first; ++i) {
            for (size_t j = 0; j != other.size().second; ++j) {
                for (size_t k = 0; k != ((*this).size().second); ++k) {
                    t_matr[i][j] += (*this)(i, k) * other(k, j);
                }
            }
        }
        (*this).matr = t_matr;
        return *this;
    }

    Matrix<T> operator *(const Matrix& other) const {
        Matrix<T> temp = *this;
        temp *= other;
        return temp;
    }

    template <typename U>
    std::vector<U> solve(const std::vector<U>& b) {
        std::vector<std::vector<T>> kf((*this).matr);
        std::vector<U> vec(b);

        for (int j = 0; j != (*this).size().second; ++j) {
            int i = j;
            while (i <= j && kf[i][j] == 0) {
                ++i;
            }

            std::swap(kf[j], kf[i]);
            std::swap(vec[j], vec[i]);

            vec[j] /= kf[j][j];
            auto kf_jj = kf[j][j];
            for (auto& el : kf[j]) {
                el /= kf_jj;
            }

            for (int k = j + 1; k != (*this).size().first; ++k) {
                auto mkf = kf[k][j];
                vec[k] -= vec[j] * mkf;

                for (int t = 0; t != kf[j].size(); ++t) {
                    kf[k][t] = kf[k][t] - kf[j][t] * mkf;
                }
            }
        }

        for (int i = 0; i != (*this).size().first; ++i) {
            for (int j = i + 1; j != (*this).size().second; ++j) {
                auto sbk = kf[i][j];
                vec[i] = vec[i] - kf[i][j] * vec[j];
                for (int t = 0; t != kf[j].size(); ++t) {
                    kf[i][t] = kf[i][t] - kf[j][t] * sbk;
                }
            }
        }

        return vec;
    }
};



template <typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i != m.size().first; ++i) {
        for (size_t j = 0; j != m.size().second; ++j) {
            out << m(i, j);
            if (j != m.size().second - 1) {
                out << '\t';
            }
        }
        if (i != m.size().first - 1) {
            out << '\n';
        }
    }
    return out;
}
