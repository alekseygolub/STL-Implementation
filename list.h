#include<iostream>
#include<utility>
#include<algorithm>
#include<memory>
#include<optional>

template<class T>
struct Node {
    std::optional<T> value;
    Node<T> *next = nullptr;
    Node<T> *prev = nullptr;

    Node() {
        next = prev = this;
    }

    Node(const T &val, Node<T> *nxt = nullptr, Node<T> *prv = nullptr) : next(nxt), prev(prv) {
        value = val;
        if (nxt == nullptr) next = this;
        if (prv == nullptr) prev = this;
    }

    Node(const Node &other) = delete;

    Node(Node &&other) {
        std::swap(next, other.next);
        std::swap(prev, other.prev);
        std::swap(value, other.value);
    }

    ~Node() {
        delete next;
    }
};

template<class T>
struct listIter {
    Node<T> *cur = nullptr;

    listIter(Node<T> *node) : cur(node) {}

    listIter& operator++() {
        cur = cur->next;
        return (*this);
    }

    listIter& operator--() {
        cur = cur->prev;
        return (*this);
    }

    T& operator*() {
        return (cur->value).value();
    }

    bool operator==(const listIter<T> &other) {
        return cur == other.cur;
    }

    bool operator!=(const listIter<T> &other) {
        return cur != other.cur;
    }

    const T& operator*() const {
        return (cur->value).value();
    }

    ~listIter() {}
};

template<class T>
class List {
private:
    Node<T> *root = nullptr;
    size_t sz = 0;

public:
    List() {
        root = new Node<T>;
    }

    void push_back(const T &x) {
        Node<T> *newNode = new Node<T>(x, root, root->prev);
        root->prev->next = newNode;
        root->prev = newNode;
        ++sz;
    }

    void push_front(const T &x) {
        Node<T> *newNode  = new Node<T>(x, root->next, root);
        root->next->prev = newNode;
        root->next = newNode;
        ++sz;
    }

    List &operator=(const List<T> &other) {
        List<T> newList;
        for (const Node<T> *it = other.root->next; it != other.root; it = it->next) {
            newList.push_back(T((it->value).value()));
        }
        std::swap(root, newList.root);
        std::swap(sz, newList.sz);
        return (*this);
    }

    List(const List<T> &other) {
        root = new Node<T>;
        try {
            for (const Node<T> *it = other.root->next; it != other.root; it = it->next) {
                push_back(T((it->value).value()));
            }
        } catch (...) {
            root->prev->next = nullptr;
            root->prev = nullptr;
            delete root;
        }
    }

    void pop_back() noexcept {
        Node<T> *oldNode = root->prev;
        root->prev = oldNode->prev;
        oldNode->prev->next = root;
        oldNode->next = oldNode->prev = nullptr;
        delete oldNode;
        --sz;
    }

    void pop_front() noexcept {
        Node<T> *oldNode = root->next;
        root->next = oldNode->next;
        oldNode->next->prev = root;
        oldNode->next = oldNode->prev = nullptr;
        delete oldNode;
        --sz;
    }

    size_t size() {
        return sz;
    }

    listIter<T> begin() {
        return listIter<T>(root->next);
    }

    listIter<T> end() {
        return listIter<T>(root);
    }

    ~List() {
        root->prev->next = nullptr;
        root->prev = nullptr;
        delete root;
    }
};