#include <iostream>
#include <string>
#include <vector>
#include "stdMap.hpp"

int main() {
    my::map<int, std::string> m;

    // Insert elements
    m.insert({3, "three"});
    m.insert({1, "one"});
    m.insert({2, "two"});
    m.insert({4, "four"});

    // Print elements in-order using explicit iterator
    std::cout << "Initial map contents: ";
    for (auto it = m.begin(); it != m.end(); ++it) {
        std::cout << it->first << ":" << it->second << " ";
    }
    std::cout << "\n";

    // Test operator[]
    m[5] = "five";
    std::cout << "After operator[] insert (key 5): ";
    for (auto it : m) {
        std::cout << it.first << ":" << it.second << " ";
    }
    std::cout << "\n";

    // Test find
    auto it = m.find(2);
    if (it != m.end()) {
        std::cout << "Found key 2 with value: " << it->second << "\n";
    }

    // Test at()
    try {
        std::cout << "Value at key 3: " << m.at(3) << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "at() threw: " << e.what() << "\n";
    }

    // Test count()
    std::cout << "Count of key 10: " << m.count(10) << "\n";

    // Test erase by key
    m.erase(1);
    std::cout << "After erase key 1: ";
    for (auto p : m) {
        std::cout << p.first << " ";
    }
    std::cout << "\n";

    // Test clear()
    m.clear();
    std::cout << "Map empty after clear()? " << std::boolalpha << m.empty() << "\n";

    // Test initializer-list constructor
    my::map<int, std::string> m2 = {{10, "ten"}, {20, "twenty"}, {15, "fifteen"}};
    std::cout << "m2 contents: ";
    for (const auto& kv : m2) {
        std::cout << kv.first << ":" << kv.second << " ";
    }
    std::cout << "\n";

    // Test copy constructor
    my::map<int, std::string> m3 = m2;
    std::cout << "m3 (copy of m2): ";
    for (auto kv : m3) {
        std::cout << kv.first << ":" << kv.second << " ";
    }
    std::cout << "\n";

    // Test move constructor
    my::map<int, std::string> m4 = std::move(m3);
    std::cout << "m4 (moved from m3): ";
    for (const auto& kv : m4) {
        std::cout << kv.first << ":" << kv.second << " ";
    }
    std::cout << "\n";
    std::cout << "m3 empty after move? " << std::boolalpha << m3.empty() << "\n";

    // Demonstrate const_iterator
    std::cout << "m4 contents with const_iterator: ";
    for (auto cit = m4.cbegin(); cit != m4.cend(); ++cit) {
        std::cout << cit->first << ":" << cit->second << " ";
    }
    std::cout << "\n";

    // Insert with hint
    my::map<int, std::string> m5 = {{1, "one"}, {3, "three"}};
    auto hint = m5.begin();
    auto it_hint = m5.insert(hint, {2, "two"});
    std::cout << "After hint insert: ";
    for (auto& p : m5) std::cout << p.first << ":" << p.second << " ";
    std::cout << "\n";

    // Assignment from initializer_list
    m5 = {{10, "ten"}, {20, "twenty"}, {15, "fifteen"}};
    std::cout << "After initializer_list assignment: ";
    for (auto& p : m5) std::cout << p.first << ":" << p.second << " ";
    std::cout << "\n";

    // Erase by iterator and get next
    auto it2 = m5.find(15);
    auto next2 = m5.erase(it2);
    std::cout << "After erase by iterator (15), next key: " << (next2 != m5.end() ? std::to_string(next2->first) : "end") << "\n";

    // Range insert
    std::vector<std::pair<const int, std::string>> vec = {{5,"five"},{6,"six"}};
    m5.insert(vec.begin(), vec.end());
    std::cout << "After range insert: ";
    for (auto& p : m5) std::cout << p.first << ":" << p.second << " ";
    std::cout << "\n";

    // Lookup helpers: lower_bound, upper_bound, equal_range
    auto lb2 = m5.lower_bound(10);
    auto ub2 = m5.upper_bound(10);
    auto er2 = m5.equal_range(10);
    std::cout << "lower_bound(10): " << lb2->first << ", upper_bound(10): " << ub2->first << "\n";
    std::cout << "equal_range(10): [" << er2.first->first << ", "
              << (er2.second != m5.end() ? std::to_string(er2.second->first) : "end") << "]\n";

    // Iterator decrement from end
    auto it_dec = m5.end();
    --it_dec;
    std::cout << "--end gives: " << it_dec->first << "\n";

    return 0;
}