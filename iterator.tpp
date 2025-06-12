#ifndef MY_STD_MAP_ITERATOR_TPP
#define MY_STD_MAP_ITERATOR_TPP

#include "iterator.hpp"

namespace my {

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::iterator::iterator()
    : node_(nullptr), root_(nullptr) {}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::iterator::iterator(Node* node, Node* root)
    : node_(node), root_(root) {}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::iterator::iterator(const iterator& other)
    : node_(other.node_), root_(other.root_) {}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator&
map<Key, T, Compare>::iterator::operator=(const iterator& other) {
    node_ = other.node_;
    root_ = other.root_;
    return *this;
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::iterator::~iterator() = default;

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator::reference
map<Key, T, Compare>::iterator::operator*() const {
    return node_->value;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator::pointer
map<Key, T, Compare>::iterator::operator->() const {
    return &node_->value;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator&
map<Key, T, Compare>::iterator::operator++() {
    if (!node_) return *this;
    if (node_->right) {
        Node* tmp = node_->right;
        while (tmp->left) tmp = tmp->left;
        node_ = tmp;
    } else {
        Node* tmp = node_;
        while (tmp->parent && tmp == tmp->parent->right)
            tmp = tmp->parent;
        node_ = tmp->parent;
    }
    return *this;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::iterator::operator++(int) {
    iterator tmp(*this);
    ++(*this);
    return tmp;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator&
map<Key, T, Compare>::iterator::operator--() {
    if (!node_) {
        node_ = root_;
        while (node_->right) node_ = node_->right;
        return *this;
    }
    if (node_->left) {
        Node* tmp = node_->left;
        while (tmp->right) tmp = tmp->right;
        node_ = tmp;
    } else {
        Node* tmp = node_;
        while (tmp->parent && tmp == tmp->parent->left)
            tmp = tmp->parent;
        node_ = tmp->parent;
    }
    return *this;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::iterator::operator--(int) {
    iterator tmp(*this);
    --(*this);
    return tmp;
}

template<typename Key, typename T, typename Compare>
bool map<Key, T, Compare>::iterator::operator==(const iterator& other) const {
    return node_ == other.node_;
}

template<typename Key, typename T, typename Compare>
bool map<Key, T, Compare>::iterator::operator!=(const iterator& other) const {
    return node_ != other.node_;
}

} // namespace my



namespace my {

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::const_iterator::const_iterator()
    : node_(nullptr), root_(nullptr) {}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::const_iterator::const_iterator(Node* node, Node* root)
    : node_(node), root_(root) {}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::const_iterator::const_iterator(const const_iterator& other)
    : node_(other.node_), root_(other.root_) {}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::const_iterator::const_iterator(const iterator& other)
    : node_(other.node_), root_(other.root_) {}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator&
map<Key, T, Compare>::const_iterator::operator=(const const_iterator& other) {
    node_ = other.node_;
    root_ = other.root_;
    return *this;
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::const_iterator::~const_iterator() = default;

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator::reference
map<Key, T, Compare>::const_iterator::operator*() const {
    return node_->value;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator::pointer
map<Key, T, Compare>::const_iterator::operator->() const {
    return &node_->value;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator&
map<Key, T, Compare>::const_iterator::operator++() {
    if (!node_) return *this;
    if (node_->right) {
        Node* tmp = node_->right;
        while (tmp->left) tmp = tmp->left;
        node_ = tmp;
    } else {
        Node* tmp = node_;
        while (tmp->parent && tmp == tmp->parent->right)
            tmp = tmp->parent;
        node_ = tmp->parent;
    }
    return *this;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator
map<Key, T, Compare>::const_iterator::operator++(int) {
    const_iterator tmp(*this);
    ++(*this);
    return tmp;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator&
map<Key, T, Compare>::const_iterator::operator--() {
    if (!node_) {
        node_ = root_;
        while (node_->right) node_ = node_->right;
        return *this;
    }
    if (node_->left) {
        Node* tmp = node_->left;
        while (tmp->right) tmp = tmp->right;
        node_ = tmp;
    } else {
        Node* tmp = node_;
        while (tmp->parent && tmp == tmp->parent->left)
            tmp = tmp->parent;
        node_ = tmp->parent;
    }
    return *this;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator
map<Key, T, Compare>::const_iterator::operator--(int) {
    const_iterator tmp(*this);
    --(*this);
    return tmp;
}

template<typename Key, typename T, typename Compare>
bool map<Key, T, Compare>::const_iterator::operator==(const const_iterator& other) const {
    return node_ == other.node_;
}

template<typename Key, typename T, typename Compare>
bool map<Key, T, Compare>::const_iterator::operator!=(const const_iterator& other) const {
    return node_ != other.node_;
}

} // namespace my

#endif // MY_STD_MAP_ITERATOR_TPP
