#include<cmath>
#include<iostream>

template<class T>
T sqr(const T &x) {
    return x * x;
}

class Complex {
private:
    double x = 0, y = 0;

public:
    Complex(double Re, double Im = 0) : x(Re), y(Im) {}
    Complex(int Re, int Im = 0) : x(static_cast<double>(Re)), y(Im) {}
    Complex(const Complex &other) : x(other.Re()), y(other.Im()) {}

    double& Re() {
        return x;
    }

    double& Im() {
        return y;
    }

    double Re() const {
        return x;
    }

    double Im() const {
        return y;
    }

    Complex operator+() const {
        return {x, y};
    }

    Complex operator-() const {
        return {-x, -y};
    }

    bool operator==(const Complex other) const {
        return (x == other.Re() && y == other.Im());
    }

    bool operator!=(const Complex other) const {
        return !(operator==(other));
    }
};

Complex operator+(const Complex &a, const Complex &b) {
    return {a.Re() + b.Re(), a.Im() + b.Im()};
}

Complex operator-(const Complex &a, const Complex &b) {
    return {a.Re() - b.Re(), a.Im() - b.Im()};
}

Complex operator*(const Complex &a, const Complex &b) {
    return {a.Re() * b.Re() - a.Im() * b.Im(),
            a.Re() * b.Im() + a.Im() * b.Re()};
}

Complex operator/(const Complex &a, const Complex &b) {
    double dominator = sqr(b.Re()) + sqr(b.Im());
    return {(a.Re() * b.Re() + a.Im() * b.Im())/dominator,
            (b.Re() * a.Im() - a.Re() * b.Im())/dominator};
}

double abs(const Complex C) {
    return sqrt(sqr(C.Re()) + sqr(C.Im()));
}

int main() {
    Complex A(4, 5);
    Complex B(6, 8);
    std::cout << abs(A) << " " << abs(B) << "\n";
    std::cout << abs(A - B) << "\n";
}