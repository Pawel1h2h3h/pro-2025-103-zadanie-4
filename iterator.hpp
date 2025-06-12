

#ifndef MY_STD_MAP_ITERATOR_HPP
#define MY_STD_MAP_ITERATOR_HPP

#include "stdMap.hpp"
#include <iterator>

namespace my {

// Forward declaration of Node inside map
template<typename Key, typename T, typename Compare>
class map<Key, T, Compare>::iterator {
public:
    // Iterator traits
    using value_type        = typename map<Key, T, Compare>::value_type;
    using pointer           = typename map<Key, T, Compare>::pointer;
    using reference         = typename map<Key, T, Compare>::reference;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator();
    iterator(const iterator& other);
    iterator& operator=(const iterator& other);
    ~iterator();

    reference operator*() const;
    pointer operator->() const;

    iterator& operator++();    // pre-increment
    iterator  operator++(int); // post-increment
    iterator& operator--();    // pre-decrement
    iterator  operator--(int); // post-decrement

    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;

private:
    using Node = typename map<Key, T, Compare>::Node;
    Node* node_;
    Node* root_;
    explicit iterator(Node* node, Node* root);
    template<typename K, typename V, typename C> friend class map;
};

template<typename Key, typename T, typename Compare>
class map<Key, T, Compare>::const_iterator {
public:
    // Iterator traits
    using value_type        = typename map<Key, T, Compare>::value_type;
    using pointer           = typename map<Key, T, Compare>::const_pointer;
    using reference         = typename map<Key, T, Compare>::const_reference;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    const_iterator();
    const_iterator(const const_iterator& other);
    const_iterator(const iterator& other);
    const_iterator& operator=(const const_iterator& other);
    ~const_iterator();

    reference operator*() const;
    pointer operator->() const;

    const_iterator& operator++();    // pre-increment
    const_iterator  operator++(int); // post-increment
    const_iterator& operator--();    // pre-decrement
    const_iterator  operator--(int); // post-decrement

    bool operator==(const const_iterator& other) const;
    bool operator!=(const const_iterator& other) const;

private:
    using Node = typename map<Key, T, Compare>::Node;
    Node* node_;
    Node* root_;
    explicit const_iterator(Node* node, Node* root);
    template<typename K, typename V, typename C> friend class map;
};

} // namespace my

#endif // MY_STD_MAP_ITERATOR_HPP