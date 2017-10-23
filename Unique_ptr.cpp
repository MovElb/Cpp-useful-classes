#include <cstddef>
#include <iostream>
#include <tuple>
#include <memory>

template <typename T, typename D = std::default_delete<T>>
class UniquePtr {
 private:
    std::tuple<T*, D> data;

 public:
    explicit UniquePtr() noexcept {
        std::get<0>(data) = nullptr;
        std::get<1>(data) = D();
    }

    explicit UniquePtr(T * src) noexcept {
        std::get<0>(data) = src;
        std::get<1>(data) = D();
    }

    explicit UniquePtr(T * src, D foo) noexcept {
        std::get<0>(data) = src;
        std::get<1>(data) = foo;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator = (const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& tmp) noexcept {
        std::get<0>(data) = std::get<0>(tmp.data);
        std::get<1>(data) = D();
        std::get<0>(tmp.data) = nullptr;
    }

    UniquePtr& operator = (std::nullptr_t) noexcept {
        std::get<1>(data)(std::get<0>(data));
        std::get<0>(data) = nullptr;
        return *this;
    }

    UniquePtr& operator = (UniquePtr&& tmp) noexcept {
        (*this).swap(tmp);
        return *this;
    }

    T& operator * () {
        return *(std::get<0>(data));
    }

    const T& operator * () const {
        return *(std::get<0>(data));
    }

    T* operator ->() const noexcept {
        return (std::get<0>(data));
    }

    T* release() noexcept {
        T * temp = std::get<0>(data);
        (std::get<0>(data)) = nullptr;
        return temp;
    }

    void reset(T * src) noexcept {
        if ((std::get<0>(data)) != nullptr) {
            std::get<1>(data)(std::get<0>(data));
        }
        (std::get<0>(data)) = src;
    }

    void swap(UniquePtr& other) noexcept {
        std::swap((std::get<0>(data)), (std::get<0>(other.data)));
    }

    T* get() const noexcept {
        return (std::get<0>(data));
    }

    explicit operator bool () const noexcept {
        return get() != nullptr;
    }

    D& get_deleter() {
        return (std::get<1>(data));
    }

    const D& get_deleter() const {
        return (std::get<1>(data));
    }

    ~UniquePtr() noexcept {
        std::get<1>(data)(std::get<0>(data));
    }
};

struct Foo
{
    Foo() { std::cout << "Foo...\n"; }
    ~Foo() { std::cout << "~Foo...\n"; }
};

struct D
{
    void bar() { std::cout << "Call deleter D::bar()...\n"; }
    void operator()(Foo* p) const
    {
        std::cout << "Call delete for Foo object...\n";
        delete p;
    }
};

int main() {
    UniquePtr<Foo, D> up(new Foo(), D());
    D& del = up.get_deleter();
    del.bar();
}