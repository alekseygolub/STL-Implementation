#include<iostream>
#include<utility>
#include<algorithm>
#include<memory>
#include<optional>

template<class T>
class SharedPtr {
private:
    int *counter = nullptr;
    T *ptr = nullptr;

public:
    SharedPtr() {}

    SharedPtr(T *other) {
        ptr = other;
        if (other != nullptr) counter = new int(1);
    }

    SharedPtr(const SharedPtr<T> &other) noexcept {
        ptr = other.ptr;
        counter = other.counter;
        if (counter != nullptr) (*counter)++;
    }

    SharedPtr(SharedPtr<T> &&other) noexcept {
        std::swap(counter, other.counter);
        std::swap(ptr, other.ptr);
    }

    SharedPtr &operator=(T *other) {
        SharedPtr<T> newPtr(other);
        swap(newPtr);
        return (*this);
    }

    SharedPtr &operator=(const SharedPtr<T> &other) noexcept {
        SharedPtr<T> newPtr(other);
        swap(newPtr);
        return (*this);
    }

    SharedPtr &operator=(SharedPtr<T> &&other) noexcept {
        SharedPtr<T> newPtr(std::move(other));
        swap(newPtr);
        return (*this);
    }

    void reset() noexcept {
        if (ptr == nullptr) return;
        if (counter != nullptr && (*counter) == 1) {
            delete counter;
            delete ptr;
        } else {
            if (counter != nullptr) (*counter)--;
        }
        ptr = nullptr;
        counter = nullptr;
    }

    void reset(T *other) {
        if (counter != nullptr && (*counter) == 1) {
            delete counter;
            delete ptr;
        } else {
            if (counter != nullptr) (*counter)--;
        }
        ptr = nullptr;
        counter = nullptr;
        if (other != nullptr) counter = new int(1);
        ptr = other;
    }

    T& operator*() {
        return *ptr;
    }

    const T& operator*() const {
        return *ptr;
    }

    const T* operator->() const noexcept {
        return ptr;
    }

    void swap(SharedPtr<T> &other) noexcept {
        std::swap(counter, other.counter);
        std::swap(ptr, other.ptr);
    }

    T* get() const noexcept {
        return ptr;
    }

    explicit operator bool() const noexcept {
        return static_cast<bool>(ptr != nullptr);
    }

    ~SharedPtr() {
        reset();
    }
};