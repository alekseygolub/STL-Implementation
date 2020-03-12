#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>
#include<map>
#include<iterator>

template<class T>
T Tabs(T x) {
    if (x < T(0))
        x = x * T(-1);
    return x;
}

template<class T>
T MyPower(T x, size_t n) {
    if (n == 0) return T(1);
    if (n & 1) return MyPower(x, n - 1) * x;
    T tmp = MyPower(x, n >> 1);
    return (tmp * tmp);
}

template<class T>
class Polynomial {
private:
    std::map<size_t, T> data;

    static Polynomial gcd(Polynomial &a, Polynomial &b) {
        while (a != T(0)) {
            b = b % a;
            std::swap(a.data, b.data);
        }
        b.normal();
        return b;
    }

    void normal() {
        std::vector<size_t> need_erase;
        for (auto &x : data)
            if (x.second == T(0)) need_erase.push_back(x.first);
        for (auto &x : need_erase) data.erase(x);
    }

public:
    template<class Iter>
    Polynomial(Iter first, Iter last) {
        size_t cur = 0;
        while (first != last) {
            if (*first != T(0)) data[cur] = *first;
            ++cur;
            ++first;
        }
        normal();
    }

    Polynomial(const std::vector<T> &other) {
        for (size_t i = 0; i < other.size(); ++i) {
            if (other[i] != T(0)) data[i] = other[i];
        }
        normal();
    }

    Polynomial(T coefficient = T()) {
        if (coefficient != T(0))
            data[0] = coefficient;
        normal();
    }

    Polynomial &operator+=(const Polynomial &other) {
        for (const auto &x : other) {
            data[x.first] += x.second;
        }
        normal();
        return (*this);
    }

    Polynomial &operator-=(const Polynomial &other) {
        for (const auto &x : other) {
            data[x.first] -= x.second;
        }
        normal();
        return (*this);
    }

    Polynomial &operator*=(const Polynomial &other) {
        std::map<size_t, T> tmp;
        for (const auto &x : data) {
            for (const auto &y : other.data) {
                tmp[x.first + y.first] += x.second * y.second;
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
        if (data.empty()) return static_cast<long long>(-1);
        return static_cast<long long>((*data.rbegin()).first);
    }

    T operator[](size_t i) const {
        if (!data.count(i)) return T(0);
        return data.at(i);
    }

    auto begin() const {
        return data.begin();
    }

    auto end() const {
        return data.end();
    }

    T operator()(T x) const {
        T S = T(0);
        for (auto &el : data) {
            S += el.second * MyPower(x, el.first);
        }
        return S;
    }

    template<class F>
    friend std::ostream &operator<<(std::ostream &out, const Polynomial<F> &P);

    Polynomial operator&(const Polynomial &other) const {
        Polynomial result(T(0));
        Polynomial deg(T(1));
        for (auto &el : data) {
            size_t i = el.first;
            result += MyPower(other, i) * el.second;
        }
        result.normal();
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
    for (auto itr = P.data.rbegin(); itr != P.data.rend(); itr++) {
        size_t i = (*itr).first;
        if (P[i] == T(0)) continue;
        if (P[i] < T(0)) {
            out << '-';
        } else {
            if (itr != P.data.rbegin()) {
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
    while (a.Degree() >= b.Degree()) {
        cur.data.clear();
        size_t curLen = static_cast<size_t>(a.Degree() - b.Degree());
        cur.data[curLen] = (*a.data.rbegin()).second / (*b.data.rbegin()).second;
        result += cur;

        Polynomial dx(T(0));
        for (auto x : b.data) dx.data[x.first + curLen] = x.second;

        T num = (*a.data.rbegin()).second;
        T den = (*b.data.rbegin()).second;

        for (auto &x : dx.data) {
            x.second *= num;
            x.second /= den;
        }
        a -= dx;
    }
    result.normal();
    return result;
}

template<class T>
Polynomial<T> operator%(Polynomial<T> a, Polynomial<T> b) {
    return a - ((a / b) * b);
}

template<class T>
Polynomial<T> operator,(Polynomial<T> a, Polynomial<T> b) {
    Polynomial<T> result = Polynomial<T>::gcd(a, b);
    T den = (*result.data.rbegin()).second;
    for (auto &x : result.data) {
        x.second /= den;
    }
    result.normal();
    return result;
}