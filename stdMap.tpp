#ifndef MY_STD_MAP_TPP
#define MY_STD_MAP_TPP

#include "stdMap.hpp"
#include "iterator.tpp"
#include <limits>
#include <stdexcept>
#include <vector>

namespace my {

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::map()
    : root_(nullptr), size_(0), comp_(Compare()) {}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::map(const Compare& comp)
    : root_(nullptr), size_(0), comp_(comp) {}

template<typename Key, typename T, typename Compare>
template<typename InputIt>
map<Key, T, Compare>::map(InputIt first, InputIt last, const Compare& comp)
    : root_(nullptr), size_(0), comp_(comp) {
    insert(first, last);
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::map(const map& other)
    : root_(nullptr), size_(0), comp_(other.comp_) {
    copy_from(other);
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::map(map&& other) noexcept
    : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_)) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::map(std::initializer_list<value_type> init, const Compare& comp)
    : root_(nullptr), size_(0), comp_(comp) {
    for (const auto& v : init) {
        insert(v);
    }
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>::~map() {
    clear();
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>& map<Key, T, Compare>::operator=(const map& other) {
    if (this != &other) {
        clear();
        comp_ = other.comp_;
        copy_from(other);
    }
    return *this;
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>& map<Key, T, Compare>::operator=(map&& other) noexcept {
    if (this != &other) {
        clear();
        root_ = other.root_;
        size_ = other.size_;
        comp_ = std::move(other.comp_);
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template<typename Key, typename T, typename Compare>
map<Key, T, Compare>& map<Key, T, Compare>::operator=(std::initializer_list<value_type> init) {
    clear();
    for (const auto& v : init) {
        insert(v);
    }
    return *this;
}

// Iterators
template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator map<Key, T, Compare>::begin() noexcept {
    Node* curr = root_;
    while (curr && curr->left) curr = curr->left;
    return iterator(curr, root_);
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator map<Key, T, Compare>::begin() const noexcept {
    Node* curr = root_;
    while (curr && curr->left) curr = curr->left;
    return const_iterator(curr, root_);
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator map<Key, T, Compare>::end() noexcept {
    return iterator(nullptr, root_);
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator map<Key, T, Compare>::end() const noexcept {
    return const_iterator(nullptr, root_);
}

// Capacity
template<typename Key, typename T, typename Compare>
bool map<Key, T, Compare>::empty() const noexcept {
    return size_ == 0;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::size_type map<Key, T, Compare>::size() const noexcept {
    return size_;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::size_type map<Key, T, Compare>::max_size() const noexcept {
    return std::numeric_limits<size_type>::max();
}

// Element access
template<typename Key, typename T, typename Compare>
T& map<Key, T, Compare>::at(const Key& key) {
    auto it = find(key);
    if (it == end()) throw std::out_of_range("key not found");
    return it->second;
}

template<typename Key, typename T, typename Compare>
const T& map<Key, T, Compare>::at(const Key& key) const {
    auto it = find(key);
    if (it == end()) throw std::out_of_range("key not found");
    return it->second;
}

template<typename Key, typename T, typename Compare>
T& map<Key, T, Compare>::operator[](const Key& key) {
    auto p = insert(value_type(key, T()));
    return p.first->second;
}

// Modifiers
template<typename Key, typename T, typename Compare>
std::pair<typename map<Key, T, Compare>::iterator,bool>
map<Key, T, Compare>::insert(const value_type& value) {
    Node* parent = nullptr;
    Node* curr = root_;
    // Search for insertion point or existing key
    while (curr) {
        parent = curr;
        if (comp_(value.first, curr->value.first))
            curr = curr->left;
        else if (comp_(curr->value.first, value.first))
            curr = curr->right;
        else {
            // Key already exists: do not insert
            return {iterator(curr, root_), false};
        }
    }
    // Create new node
    Node* node = new Node(value, parent);
    if (!parent) {
        // Tree was empty
        root_ = node;
    } else if (comp_(value.first, parent->value.first)) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    ++size_;
    return {iterator(node, root_), true};
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::insert(const_iterator hint, const value_type& value) {
    (void)hint; // ignore hint for now
    return insert(value).first;
}

template<typename Key, typename T, typename Compare>
template<typename InputIt>
void map<Key, T, Compare>::insert(InputIt first, InputIt last) {
    for (; first != last; ++first) insert(*first);
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::erase(const_iterator pos)
{
    Node* node = pos.node_;
    if (!node) return end();
    // Find successor
    Node* succ = nullptr;
    if (node->right) {
        succ = node->right;
        while (succ->left) succ = succ->left;
    } else {
        Node* p = node->parent;
        Node* c = node;
        while (p && c == p->right) {
            c = p;
            p = p->parent;
        }
        succ = p;
    }
    // Remove node by transplanting child
    Node* child = node->left ? node->left : node->right;
    if (child) child->parent = node->parent;
    if (!node->parent) {
        root_ = child;
    } else if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }
    delete node;
    --size_;
    return iterator(succ, root_);
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::size_type
map<Key, T, Compare>::erase(const Key& key)
{
    auto it = find(key);
    if (it == end()) return 0;
    erase(it);
    return 1;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::erase(const_iterator first, const_iterator last)
{
    std::vector<Key> to_delete;
    for (auto it = first; it != last; ++it) {
        to_delete.push_back(it->first);
    }
    for (const Key& k : to_delete) {
        erase(k);
    }
    return iterator(nullptr, root_);
}

template<typename Key, typename T, typename Compare>
void map<Key, T, Compare>::clear() noexcept {
    clear_node(root_);
    root_ = nullptr;
    size_ = 0;
}

// Lookup
template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::size_type
map<Key, T, Compare>::count(const Key& key) const {
    return find(key) != end() ? 1 : 0;
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::find(const Key& key) {
    Node* curr = root_;
    while (curr) {
        if (comp_(key, curr->value.first)) curr = curr->left;
        else if (comp_(curr->value.first, key)) curr = curr->right;
        else return iterator(curr, root_);
    }
    return end();
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator
map<Key, T, Compare>::find(const Key& key) const {
    Node* curr = root_;
    while (curr) {
        if (comp_(key, curr->value.first)) curr = curr->left;
        else if (comp_(curr->value.first, key)) curr = curr->right;
        else return const_iterator(curr, root_);
    }
    return end();
}

template<typename Key, typename T, typename Compare>
std::pair<typename map<Key, T, Compare>::iterator,
          typename map<Key, T, Compare>::iterator>
map<Key, T, Compare>::equal_range(const Key& key) {
    return {lower_bound(key), upper_bound(key)};
}

template<typename Key, typename T, typename Compare>
std::pair<typename map<Key, T, Compare>::const_iterator,
          typename map<Key, T, Compare>::const_iterator>
map<Key, T, Compare>::equal_range(const Key& key) const {
    return {lower_bound(key), upper_bound(key)};
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::lower_bound(const Key& key) {
    Node* curr = root_;
    Node* result = nullptr;
    while (curr) {
        if (!comp_(curr->value.first, key)) {
            result = curr;
            curr = curr->left;
        } else curr = curr->right;
    }
    return result ? iterator(result, root_) : end();
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator
map<Key, T, Compare>::lower_bound(const Key& key) const {
    Node* curr = root_;
    Node* result = nullptr;
    while (curr) {
        if (!comp_(curr->value.first, key)) {
            result = curr;
            curr = curr->left;
        } else curr = curr->right;
    }
    return result ? const_iterator(result, root_) : end();
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::iterator
map<Key, T, Compare>::upper_bound(const Key& key) {
    Node* curr = root_;
    Node* result = nullptr;
    while (curr) {
        if (comp_(key, curr->value.first)) {
            result = curr;
            curr = curr->left;
        } else curr = curr->right;
    }
    return result ? iterator(result, root_) : end();
}

template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::const_iterator
map<Key, T, Compare>::upper_bound(const Key& key) const {
    Node* curr = root_;
    Node* result = nullptr;
    while (curr) {
        if (comp_(key, curr->value.first)) {
            result = curr;
            curr = curr->left;
        } else curr = curr->right;
    }
    return result ? const_iterator(result, root_) : end();
}

// Observers
template<typename Key, typename T, typename Compare>
typename map<Key, T, Compare>::key_compare map<Key, T, Compare>::key_comp() const {
    return comp_;
}

// Helper functions
template<typename Key, typename T, typename Compare>
void map<Key, T, Compare>::copy_from(const map& other) {
    for (auto it = other.begin(); it != other.end(); ++it) {
        insert(*it);
    }
}

template<typename Key, typename T, typename Compare>
void map<Key, T, Compare>::clear_node(Node* node) {
    if (!node) return;
    clear_node(node->left);
    clear_node(node->right);
    delete node;
}

} // namespace my

#endif // MY_STD_MAP_TPP
