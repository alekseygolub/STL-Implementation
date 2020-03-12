#include<iostream>
#include<utility>
#include<cstddef>
#include<memory>

struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;

    Optional(const T& elem) {
        defined = true;
        new (data) T(elem);
    }

    Optional(T&& elem) {
        defined = true;
        new (data) T(std::move(elem));
    }

    Optional(const Optional& other) {
        defined = other.defined;
        data = other.data;
    }

    Optional& operator=(const Optional& other) {
        if (defined) {
            if (other.defined) {
                *reinterpret_cast<T*>(data) = *reinterpret_cast<const T*>(other.data);
            } else {
                reinterpret_cast<T*>(data)->~T();
                defined = false;
            }
        } else {
            if (other.defined) {
                new (data) T(*reinterpret_cast<const T*>(other.data));
                defined = true;
            }
        }
        return (*this);
    }

    Optional& operator=(const T& elem) {
        if (defined) {
            reinterpret_cast<T*>(data)->~T();
            defined = false;
        }
        defined = true;
        new (data) T(elem);
        return (*this);
    }

    Optional& operator=(T&& elem) {
        if (defined) {
            *reinterpret_cast<T*>(data) = std::move(elem);
        } else {
            new (data) T(std::move(elem));
            defined = true;
        }
        return (*this);
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return *reinterpret_cast<T*>(data);
    }

    const T& operator*() const {
        return *reinterpret_cast<const T*>(data);
    }

    T* operator->() {
        return reinterpret_cast<T*>(data);
    }

    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }

    T& value() {
        if (!defined) throw BadOptionalAccess();
        return *reinterpret_cast<T*>(data);
    }

    const T& value() const {
        if (!defined) throw BadOptionalAccess();
        return *reinterpret_cast<const T*>(data);
    }

    void reset() {
        if (defined) reinterpret_cast<T*>(data)->~T();
        defined = false;
    }

    ~Optional() {
        if (defined) reinterpret_cast<T*>(data)->~T();
    }
};