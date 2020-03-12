#include<iostream>
#include<vector>
#include<utility>

template<class T>
class Matrix {
private:
    std::vector<std::vector<T>> M;
    size_t n = 0, m = 0;

public:
    Matrix(const std::vector<std::vector<T>> &Init) : M(Init) {
        n = M.size();
        if (n) m = M[0].size();
    }

    const std::vector<T>& operator[](size_t i) const {
        return M[i];
    }

    std::vector<T>& operator[](size_t i) {
        return M[i];
    }

    std::pair<size_t, size_t> size() const {
        return {n, m};
    }

    Matrix<T>& operator+=(const Matrix &Mat) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                M[i][j] += Mat[i][j];
            }
        }
        return (*this);
    }

    Matrix<T> operator+(const Matrix &Mat) const {
        Matrix<T> cp((*this));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                cp[i][j] += Mat[i][j];
            }
        }
        return cp;
    }

    template<class Number>
    Matrix<T>& operator*=(const Number &a) {
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                M[i][j] *= a;
            }
        }
        return (*this);
    }

    template<class Number>
    Matrix<T> operator*(const Number &a) const {
        Matrix<T> cp((*this));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                cp[i][j] *= a;
            }
        }
        return cp;
    }

    Matrix<T>& operator*=(const Matrix<T>& other) {
        auto [n2, m2] = other.size();
        assert(m == n2);
        std::vector<std::vector<T>> tmp(n, std::vector<T>(m2));
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m2; j++) {
                for (size_t k = 0; k < m; k++) {
                    tmp[i][j] += M[i][k] * other[k][j];
                }
            }
        }
        M = tmp;
        m = m2;
        return (*this);
    }

    Matrix<T> operator*(const Matrix<T>& other) const {
        Matrix<T> tmp(*this);
        tmp *= other;
        return tmp;
    }

    Matrix<T>& transpose() {
        if (M.empty()) return *this;
        std::vector<std::vector<T>> new_M(m, std::vector<T>(n));
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < m; ++j) {
                new_M[j][i] = M[i][j];
            }
        }
        M = new_M;
        std::swap(n, m);
        return *this;
    }

    Matrix<T> transposed() const {
        Matrix<T> new_M((*this));
        new_M.transpose();
        return new_M;
    }

    T det() const {
        T res = T(0);
    }

    void makeGauss() {
        auto Mat = M;
        size_t row = 0;
        for (size_t i = 0; i < m; i++) {
            for (size_t j = row; j < n; j++) {
                if (Mat[j][i] != T(0)) {
                    std::swap(Mat[j], Mat[row]);
                    T kek = Mat[row][i];
                    for (auto &x : Mat[row]) {
                        x /= kek;
                    }
                    for (size_t k = 0; k < n; k++) {
                        if (k == row) continue;
                        T mult = Mat[k][i];
                        for (size_t l = 0; l < m; ++l)
                            Mat[k][l] -= Mat[row][l] * mult;
                    }
                    row++;
                    break;
                }
            }

            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < m; j++) {
                    if (j != 0) std::cout << '\t';
                    std::cout << Mat[i][j];
                }
                if (i != n - 1) std::cout << "\n";
            }
            std::cout << "\n\n";
        }
    }

    void solve(Matrix<T> &other) {
        auto Mat = M;

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m; j++) {
                if (j != 0) std::cout << '\t';
                std::cout << Mat[i][j];
            }
            std::cout <<  "\t|";
            for (size_t j = 0; j < other.m; j++) {
                std::cout << '\t' << other.M[i][j];
            } 
            if (i != n - 1) std::cout << "\n";
        }
        std::cout << "\n\n";

        size_t row = 0;
        for (size_t i = 0; i < m; i++) {
            for (size_t j = row; j < n; j++) {
                if (Mat[j][i] != T(0)) {
                    std::swap(Mat[j], Mat[row]);
                    std::swap(other.M[j], other.M[row]);
                    T kek = Mat[row][i];
                    for (auto &x : Mat[row]) {
                        x /= kek;
                    }
                    for (auto &x : other.M[row]) {
                        x /= kek;
                    }
                    for (size_t k = 0; k < n; k++) {
                        if (k == row) continue;
                        T mult = Mat[k][i];
                        for (size_t l = 0; l < m; ++l)
                            Mat[k][l] -= Mat[row][l] * mult;
                        for (size_t l = 0; l < other.m; ++l)
                            other.M[k][l] -= other.M[row][l] * mult;
                    }
                    row++;
                    break;
                }
            }

            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < m; j++) {
                    if (j != 0) std::cout << '\t';
                    std::cout << Mat[i][j];
                }
                std::cout <<  "\t|";
                for (size_t j = 0; j < other.m; j++) {
                    std::cout << '\t' << other.M[i][j];
                } 
                if (i != n - 1) std::cout << "\n";
            }
            std::cout << "\n\n";
        }
    }

};

template<class T>
std::ostream& operator<<(std::ostream &out, const Matrix<T> &Mat) {
    auto [n, m] = Mat.size();
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            if (j != 0) out << '\t';
            out << Mat[i][j];
        }
        if (i != n - 1) out << "\n";
    }
    return out;
}