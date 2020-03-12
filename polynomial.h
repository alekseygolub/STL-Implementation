#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>

template<class T>
T Tabs(T x) {
    if (x < T(0))
        x = T(0) - x;
    return x;
}

template<class T>
class Polynomial {
private:
    std::vector<T> data;

    void normal() {
        while (!data.empty() && data.back() == T(0)) {
            data.pop_back();
        }
    }

    static Polynomial gcd(Polynomial &a, Polynomial &b) {
        while (a != T(0)) {
            b = b % a;
            std::swap(a.data, b.data);
        }
        return b;
    }

public:
    template<class Iter>
    Polynomial(Iter first, Iter last) {
        data = std::vector<T>(first, last);
        normal();
    }

    Polynomial(const std::vector<T> &other) : data(other) {
        normal();
    }

    Polynomial(T coefficient = T()) {
        data.push_back(coefficient);
        normal();
    }

    Polynomial &operator+=(const Polynomial &other) {
        size_t cursize = std::min(data.size(), other.data.size());
        for (size_t i = 0; i < cursize; ++i) {
            data[i] += other.data[i];
        }
        while (data.size() < other.data.size()) {
            data.push_back(other.data[data.size()]);
        }
        normal();
        return (*this);
    }

    Polynomial &operator-=(const Polynomial &other) {
        size_t cursize = std::min(data.size(), other.data.size());
        for (size_t i = 0; i < cursize; ++i) {
            data[i] -= other.data[i];
        }
        while (data.size() < other.data.size()) {
            data.push_back(T(0) - other.data[data.size()]);
        }
        normal();
        return (*this);
    }

    Polynomial &operator*=(const Polynomial &other) {
        std::vector<T> tmp(data.size() + other.data.size());
        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = 0; j < other.data.size(); ++j) {
                tmp[i + j] += data[i] * other.data[j];
            }
        }
        std::swap(data, tmp);
        normal();
        return (*this);
    }

    Polynomial operator+(const Polynomial &other) const {
        Polynomial<T> tmp = (*this);
        tmp += other;
        return tmp;
    }

    Polynomial operator-(const Polynomial &other) const {
        Polynomial<T> tmp = (*this);
        tmp -= other;
        return tmp;
    }

    Polynomial operator*(const Polynomial &other) const {
        Polynomial<T> tmp = (*this);
        tmp *= other;
        return tmp;
    }


    bool operator==(const Polynomial &other) const {
        return data == other.data;
    }

    bool operator!=(const Polynomial &other) const {
        return data != other.data;
    }

    long long Degree() const {
        return static_cast<long long>(data.size()) - 1;
    }

    T operator[](size_t i) const {
        if (i < 0 || i >= data.size()) return T(0);
        return data[i];
    }

    auto begin() const {
        return data.begin();
    }

    auto end() const {
        return data.end();
    }

    T operator()(T x) const {
        T S = T(0);
        T coefficient = T(1);
        for (size_t i = 0; i < data.size(); ++i) {
            S += data[i] * coefficient;
            coefficient *= x;
        }
        return S;
    }

    template<class F>
    friend std::ostream &operator<<(std::ostream &out, const Polynomial<F> &P);

    Polynomial operator&(const Polynomial &other) const {
        Polynomial result(T(0));
        Polynomial deg(T(1));
        for (size_t i = 0; i < data.size(); ++i) {
            result += (deg * data[i]);
            deg *= other;
        }
        return result;
    }

    template<class F>
    friend Polynomial<F> operator/(Polynomial<F> a, Polynomial<F> b);

    template<class F>
    friend Polynomial<F> operator%(Polynomial<F> a, Polynomial<F> b);

    template<class F>
    friend Polynomial<F> operator,(Polynomial<F> a, Polynomial<F> b);
};

template<class T>
std::ostream &operator<<(std::ostream &out, const Polynomial<T> &P) {
    if (P.data.empty()) {
        out << '0';
        return out;
    }
    for (size_t i = P.data.size() - 1; i <= P.data.size(); --i) {
        if (P[i] == T(0)) continue;
        if (P[i] < T(0)) {
            out << '-';
        } else {
            if (i != P.data.size() - 1) {
                out << '+';
            }
        }
        if ((P[i] != T(1) && P[i] != T(-1)) || i == 0) {
            out << Tabs(P[i]);
            if (i != 0) out << '*';
        }
        if (i != 0) {
            out << "x";
            if (i > 1)
                out << '^' << i;
        }
    }
    return out;
}

template<class T>
Polynomial<T> operator/(Polynomial<T> a, Polynomial<T> b) {
    Polynomial<T> result(T(0)), cur;
    while (a.data.size() >= b.data.size()) {
        cur.data.clear();
        size_t curLen = static_cast<size_t>(a.Degree() - b.Degree());
        cur.data.resize(curLen + 1);
        cur.data[curLen] = a.data.back() / b.data.back();
        result += cur;

        std::vector<T> Tdx(curLen, T(0));
        for (auto x : b.data) Tdx.push_back(x);

        Polynomial dx(Tdx);

        for (size_t i = 0; i < dx.data.size(); ++i) {
            dx.data[i] *= a.data.back();
            dx.data[i] /= b.data.back();
        }
        a -= dx;
    }
    return result;
}

template<class T>
Polynomial<T> operator%(Polynomial<T> a, Polynomial<T> b) {
    return a - ((a / b) * b);
}

template<class T>
Polynomial<T> operator,(Polynomial<T> a, Polynomial<T> b) {
    Polynomial<T> result = Polynomial<T>::gcd(a, b);
    for (size_t i = 0; i < result.data.size(); ++i) {
        result.data[i] /= result.data.back();
    }
    result.normal();
    return result;
}