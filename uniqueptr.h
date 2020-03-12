#include<iostream>
#include<utility>
#include<cstddef>
#include<memory>
#include<tuple>

template<class T, class Deleter_type = std::default_delete<T>>
struct UniquePtr {
private:
    std::tuple<Deleter_type, T*> Data;

public:
    UniquePtr() {
        Data = std::make_tuple(Deleter_type(), nullptr);
    }

     UniquePtr(T *other) {
        Data = std::make_tuple(Deleter_type(), nullptr);
        std::swap(other, std::get<1>(Data));
    }

    UniquePtr(T *other, Deleter_type D) {
        Data = std::make_tuple(D, nullptr);
        std::swap(other, std::get<1>(Data));
    }

    UniquePtr(UniquePtr &&other) {
        Data = std::make_tuple(Deleter_type(), nullptr);
        std::swap(std::get<1>(Data), std::get<1>(other.Data));
    }

    UniquePtr(const UniquePtr &other) = delete;

    UniquePtr &operator=(const UniquePtr &other) = delete;

    UniquePtr &operator=(std::nullptr_t null) noexcept {
        std::get<0>(Data)(std::get<1>(Data));
        std::get<1>(Data) = null;
        return (*this);
    }

    UniquePtr &operator=(UniquePtr &&other) noexcept {
        UniquePtr(std::move(other)).swap(*this);
        return (*this);
    }

    ~UniquePtr() noexcept {
        std::get<0>(Data)(std::get<1>(Data));
    }

    const T& operator*() const noexcept {
        return *(std::get<1>(Data));
    }

    const T* operator->() const noexcept {
        return std::get<1>(Data);
    }

    T* release() noexcept {
        auto cp = std::get<1>(Data);
        std::get<1>(Data) = nullptr;
        return cp;
    }

    void reset(T *other) noexcept {
        std::get<0>(Data)(std::get<1>(Data));
        std::get<1>(Data) = nullptr;
        std::swap(std::get<1>(Data), other);
    }

     void swap(UniquePtr &other) noexcept {
        std::swap(std::get<1>(Data), std::get<1>(other.Data));
     }

     T* get() const noexcept {
        return std::get<1>(Data);
     }

     explicit operator bool() const noexcept {
        return static_cast<bool>(std::get<1>(Data) != nullptr);
     }

     const Deleter_type& get_deleter() const {
        return std::get<0>(Data);
     }

     Deleter_type& get_deleter() {
        return std::get<0>(Data);
     }
};