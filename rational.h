template<class T>
T gcd(T a, T b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

class Rational {
private:
    int x = 0, y = 1;

public:
    void reduce() {
        int g = gcd(x, y);
        x /= g;
        y /= g;

        if (y < 0) {
            y *= -1;
            x *= -1;
        }
    }

    Rational(int num = 0, int den = 1) : x(num), y(den) {
        reduce();
    }

    int numerator() const {
        return x;
    }

    int denominator() const {
        return y;
    }

    Rational operator+() {
        reduce();
        return {x, y};
    }

    Rational operator-() {
        reduce();
        return {-x, y};
    }

    bool operator==(Rational other) {
        reduce();
        other.reduce();
        if (numerator() == other.numerator() &&
            denominator() == other.denominator()) return true;
        return false;
    }

    bool operator!=(Rational other) {
        return !(operator==(other));
    }

    Rational& operator++() {
        reduce();
        x += y;
        reduce();
        return (*this);
    }

    Rational& operator--() {
        reduce();
        x -= y;
        reduce();
        return (*this);
    }

    Rational operator++(int) {
        reduce();
        Rational old(x, y);
        ++(*this);
        return old;
    }

    Rational operator--(int) {
        reduce();
        Rational old(x, y);
        --(*this);
        return old;
    }
};

Rational operator+(Rational a, Rational b) {
    a.reduce();
    b.reduce();
    return {a.numerator() * b.denominator() + b.numerator() * a.denominator(),
            a.denominator() * b.denominator()};
}

Rational operator-(Rational a, Rational b) {
    a.reduce();
    b.reduce();
    return {a.numerator() * b.denominator() - b.numerator() * a.denominator(),
            a.denominator() * b.denominator()};
}

Rational operator*(Rational a, Rational b) {
    a.reduce();
    b.reduce();
    return {a.numerator() * b.numerator(),
            a.denominator() * b.denominator()};
}

Rational operator/(Rational a, Rational b) {
    a.reduce();
    b.reduce();
    return a * Rational{b.denominator(), b.numerator()};
}

Rational& operator+=(Rational &a, Rational b) {
    a.reduce();
    b.reduce();
    a = a + b;
    return a;
}

Rational& operator-=(Rational &a, Rational b) {
    a.reduce();
    b.reduce();
    a = a - b;
    return a;
}

Rational& operator*=(Rational &a, Rational b) {
    a.reduce();
    b.reduce();
    a = a * b;
    return a;
}

Rational& operator/=(Rational &a, Rational b) {
    a.reduce();
    b.reduce();
    a = a / b;
    return a;
}

std::ostream& operator<<(std::ostream &out, const Rational &R) {
    if (R.denominator() == 1) out << R.numerator();
    else out << R.numerator() << "/" << R.denominator();
    return out;
}