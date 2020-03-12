#include<iostream>
#include<utility>
#include<algorithm>
#include<memory>

template<class T>
class RawMemory {
public:
    T *data = nullptr;
    size_t capacity = 0;


    RawMemory(const RawMemory &other) = delete;
    RawMemory &operator=(const RawMemory &other) = delete;

    RawMemory &operator=(RawMemory &&other) {
        std::swap(capacity, other.capacity);
        std::swap(data, other.data);
        return (*this);
    }

    RawMemory(size_t n = 0) : capacity(n) {
        if (n > 0)
            data = reinterpret_cast<T*>(::operator new (sizeof(T) * n));
        else
            data = nullptr;
    }

    T* operator[](size_t i) {
        return data + i;
    }

    const T* operator[](size_t i) const {
        return data + i;
    }

    T* Get() {
        return data;
    }

    size_t size() const {
        return capacity;
    }

    void swap(RawMemory &other) {
        std::swap(data, other.data);
        std::swap(capacity, other.capacity);
    }

    ~RawMemory() {
        ::operator delete(data);
    }
};

template<class T>
class Vector {
private:
    RawMemory<T> Data;
    size_t sz = 0;
public:
    Vector() {}

    Vector(const Vector &other) {
        Data = RawMemory<T>(other.sz);
        sz = other.sz;
        size_t i = 0;
        try {
            for (i = 0; i < other.sz; ++i) {
                new (Data[i]) T(other[i]);
            }
        } catch (...) {
            for (size_t j = 0; j <= i; ++j) {
                Data[i]->~T();
            }
            throw;
        }
    }

    Vector &operator=(const Vector &other) {
        Vector newVector(other);
        Data.swap(newVector.Data);
        std::swap(sz, newVector.sz);
        return (*this);
    }

    void reserve(size_t n) {
        if (n <= Data.size()) return;
        Reallocate(n);
    }

    void resize(size_t n) {
        if (Data.size() < n) {
            ReallocateAnsConstruct(n);
            sz = n;
            return;
        }
        if (n > sz) {
            size_t i = sz;
            try {
                for (i = sz; i < n; ++i) {
                    new (Data[i]) T();
                }
            } catch (...) {
                for (size_t j = sz; j <= i; j++) {
                    Data[j]->~T();
                }
                throw;
            }
        }
        if (n < sz) {
            int i = static_cast<int>(sz - 1);
            for (; i >= static_cast<int>(n); --i) {
                Data[static_cast<size_t>(i)]->~T();
            }
        }
        sz = n;
    }

    void Reallocate(size_t n) {
        auto newData = RawMemory<T>(n);
        size_t i = 0;
        try {
            for (i = 0; i < sz; ++i) {
                new (newData[i]) T(*Data[i]);
            }
        } catch (...) {
            for (size_t j = 0; j <= i; ++j) {
                newData[i]->~T();
            }
            throw;
        }
        for (i = 0; i < sz; ++i) {
            Data[i]->~T();
        }
        Data.swap(newData);
    }

    void ReallocateAnsConstruct(size_t n) {
        auto newData = RawMemory<T>(n);
        size_t i = 0;
        try {
            for (i = 0; i < sz; ++i) {
                new (newData[i]) T(*Data[i]);
            }
            for (i; i < n; ++i) {
                new (newData[i]) T();
            }
        } catch (...) {
            for (size_t j = 0; j <= i; ++j) {
                newData[i]->~T();
            }
            throw;
        }
        for (i = 0; i < sz; ++i) {
            Data[i]->~T();
        }
        Data.swap(newData);
    }

    void push_back(const T &x) {
        if (sz == Data.capacity) Reallocate((sz == 0 ? 1 : sz * 2));
        new (Data[sz]) T(x);
        ++sz;
    }

    void push_back(T &&x) {
        if (sz == Data.capacity) Reallocate((sz == 0 ? 1 : sz * 2));
        new (Data[sz]) T(std::move(x));
        ++sz;
    }

    void clear() {
        for (size_t i = 0; i < sz; ++i) {
            Data[i]->~T();
        }
        Data = std::move(RawMemory<T>(0));
    }

    void pop_back() {
        --sz;
        Data[sz]->~T();
    }

    void swap(Vector other) {
        Data.swap(other.Data);
        std::swap(sz, other.sz);
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return Data.size();
    }

    T& operator[](size_t i) {
        return *Data[i];
    }

    const T& operator[](size_t i) const {
        return *Data[i];
    }

    T* begin() {
        return Data[0];
    }

    T* end() {
        return Data[sz];
    }

    ~Vector() {
        for (size_t i = 0; i < sz; ++i) {
            Data[i]->~T();
        }
    }
};