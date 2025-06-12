

#ifndef MY_STD_MAP_HPP
#define MY_STD_MAP_HPP

#include <functional>
#include <utility>
#include <initializer_list>
#include <cstddef>

namespace my {

template <typename Key, typename T, typename Compare = std::less<Key>>
class map {
public:
    // Member types
    using key_type        = Key;
    using mapped_type     = T;
    using value_type      = std::pair<const Key, T>;
    using key_compare     = Compare;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    class iterator;
    class const_iterator;
    using size_type       = std::size_t;

    // Constructors and destructor
    map();
    explicit map(const Compare& comp);
    template <typename InputIt>
    map(InputIt first, InputIt last, const Compare& comp = Compare());
    map(const map& other);
    map(map&& other) noexcept;
    map(std::initializer_list<value_type> init, const Compare& comp = Compare());
    ~map();

    // Assignment
    map& operator=(const map& other);
    map& operator=(map&& other) noexcept;
    map& operator=(std::initializer_list<value_type> init);

    // Iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    // w sekcji public:
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend()   const noexcept { return end(); }

    // Capacity
    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    // Element access
    T& at(const Key& key);
    const T& at(const Key& key) const;
    T& operator[](const Key& key);

    // Modifiers
    std::pair<iterator,bool> insert(const value_type& value);
    iterator insert(const_iterator hint, const value_type& value);
    template <typename InputIt>
    void insert(InputIt first, InputIt last);
    iterator erase(const_iterator pos);
    size_type erase(const Key& key);
    iterator erase(const_iterator first, const_iterator last);
    void clear() noexcept;

    // Lookup
    size_type count(const Key& key) const;
    iterator find(const Key& key);
    const_iterator find(const Key& key) const;
    std::pair<iterator,iterator> equal_range(const Key& key);
    std::pair<const_iterator,const_iterator> equal_range(const Key& key) const;
    iterator lower_bound(const Key& key);
    const_iterator lower_bound(const Key& key) const;
    iterator upper_bound(const Key& key);
    const_iterator upper_bound(const Key& key) const;

    // Observers
    key_compare key_comp() const;

private:
    // Node structure for red-black or binary tree implementation
    struct Node {
        value_type value;
        Node* left;
        Node* right;
        Node* parent;
        // Constructor to initialize node with value and optional parent
        explicit Node(const value_type& val, Node* p = nullptr)
            : value(val), left(nullptr), right(nullptr), parent(p) {}
    };

    Node* root_;
    size_type size_;
    Compare comp_;

    // Helper functions
    void copy_from(const map& other);
    void clear_node(Node* node);
};

} // namespace my

#include "iterator.hpp"
#include "stdMap.tpp"

#endif // MY_STD_MAP_HPP