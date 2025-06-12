

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "stdMap.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

using Map = my::map<int, std::string>;

TEST_CASE("Default constructor and capacity", "[map]") {
    Map m;
    REQUIRE(m.empty());
    REQUIRE(m.size() == 0);
}

TEST_CASE("Insert and operator[]", "[map]") {
    Map m;
    auto res = m.insert({10, "ten"});
    REQUIRE(res.second);
    REQUIRE(res.first->first == 10);
    REQUIRE(res.first->second == "ten");

    m[20] = "twenty";
    REQUIRE(m.count(20) == 1);
    REQUIRE(m.find(20)->second == "twenty");
}

TEST_CASE("Duplicate insert", "[map]") {
    Map m;
    m.insert({10, "ten"});
    auto res = m.insert({10, "TEN"});
    REQUIRE_FALSE(res.second);
    REQUIRE(m.at(10) == "ten");
}

TEST_CASE("At and out_of_range", "[map]") {
    Map m;
    m.insert({1, "one"});
    REQUIRE(m.at(1) == "one");
    REQUIRE_THROWS_AS(m.at(2), std::out_of_range);
}

TEST_CASE("Count and find", "[map]") {
    Map m = {{1, "one"}, {2, "two"}};
    REQUIRE(m.count(1) == 1);
    REQUIRE(m.count(3) == 0);
    auto it = m.find(2);
    REQUIRE(it != m.end());
    REQUIRE(it->second == "two");
}

TEST_CASE("Erase by key and range erase", "[map]") {
    Map m = {{1, "one"}, {2, "two"}, {3, "three"}};
    REQUIRE(m.erase(2) == 1);
    REQUIRE(m.count(2) == 0);

    m.insert({2, "two"});
    auto first = m.find(1);
    auto last = m.find(3);
    m.erase(first, last);
    std::vector<int> keys;
    for (auto& p : m) keys.push_back(p.first);
    REQUIRE(keys == std::vector<int>{3});
}

TEST_CASE("Clear", "[map]") {
    Map m = {{1,"one"},{2,"two"}};
    m.clear();
    REQUIRE(m.empty());
}

TEST_CASE("Range constructor and insert range", "[map]") {
    std::vector<Map::value_type> vec = {{1,"one"},{2,"two"},{3,"three"}};
    Map m(vec.begin(), vec.end());
    REQUIRE(m.size() == 3);
    REQUIRE(m.at(2) == "two");

    Map m2;
    m2.insert(vec.begin(), vec.end());
    REQUIRE(m2.size() == 3);
}

TEST_CASE("Copy and move constructors", "[map]") {
    Map m = {{1,"one"},{2,"two"}};
    Map mc = m;
    REQUIRE(mc.size() == m.size());
    Map mm = std::move(mc);
    REQUIRE(mm.size() == 2);
    REQUIRE(mc.empty());
}

TEST_CASE("Assignment operators", "[map]") {
    Map m1 = {{1,"one"}};
    Map m2;
    m2 = m1;
    REQUIRE(m2.size() == 1);
    m2 = std::move(m2);
    REQUIRE(m2.size() == 1);
}

TEST_CASE("Lower_bound and upper_bound", "[map]") {
    Map m = {{1,"one"},{3,"three"},{5,"five"}};
    auto lb = m.lower_bound(3);
    REQUIRE(lb->first == 3);
    auto ub = m.upper_bound(3);
    REQUIRE(ub->first == 5);
}

TEST_CASE("Equal_range", "[map]") {
    Map m = {{1,"one"},{2,"two"},{3,"three"}};
    auto range = m.equal_range(2);
    REQUIRE(range.first->first == 2);
    REQUIRE(range.second->first >= 3);
}

TEST_CASE("Iterators in order", "[map]") {
    Map m = {{4,"four"},{2,"two"},{1,"one"},{3,"three"}};
    std::vector<int> keys;
    for (auto it = m.begin(); it != m.end(); ++it) {
        keys.push_back(it->first);
    }
    REQUIRE(keys == std::vector<int>{1,2,3,4});
}

TEST_CASE("Const iterators", "[map]") {
    Map m = {{2,"two"},{1,"one"}};
    const Map& cm = m;
    std::vector<int> keys;
    for (auto it = cm.cbegin(); it != cm.cend(); ++it) {
        keys.push_back(it->first);
    }
    REQUIRE(keys == std::vector<int>{1,2});
}

TEST_CASE("Observers and capacity", "[map]") {
    Map m = {{1,"one"}};
    REQUIRE(!m.empty());
    REQUIRE(m.size() == 1);
    REQUIRE(m.max_size() > 0);
    auto comp = m.key_comp();
    REQUIRE(comp(1,2));
}

TEST_CASE("Insert with hint", "[map]") {
    Map m = {{1, "one"}, {3, "three"}};
    auto it = m.begin();
    auto res = m.insert(it, {2, "two"});
    REQUIRE(res == m.find(2));
    // Inserting existing key with hint should not insert
    auto res2 = m.insert(m.begin(), {2, "TWO"});
    REQUIRE(res2 == m.find(2));
    REQUIRE(m.at(2) == "two"); // original value unchanged
}

TEST_CASE("Assignment from initializer_list", "[map]") {
    Map m;
    m = {{5, "five"}, {6, "six"}};
    REQUIRE(m.size() == 2);
    REQUIRE(m.at(5) == "five");
    REQUIRE(m.at(6) == "six");
}

TEST_CASE("Erase by iterator returns next", "[map]") {
    Map m = {{1,"one"}, {2,"two"}, {3,"three"}};
    auto it = m.find(2);
    auto next = m.erase(it);
    REQUIRE(m.count(2) == 0);
    REQUIRE(next->first == 3);
}

TEST_CASE("Iterator decrement operations", "[map]") {
    Map m = {{1,"one"}, {2,"two"}, {3,"three"}};
    auto it = m.end();
    --it;
    REQUIRE(it->first == 3);
    it--;
    REQUIRE(it->first == 2);

    const Map& cm = m;
    auto cit = cm.cend();
    --cit;
    REQUIRE(cit->first == 3);
    cit--;
    REQUIRE(cit->first == 2);
}

TEST_CASE("Const methods on const map", "[map]") {
    const Map m = {{10,"ten"}, {20,"twenty"}};
    // at() const
    REQUIRE(m.at(10) == "ten");
    // find() const returns const_iterator
    auto cit = m.find(20);
    REQUIRE(cit != m.end());
    REQUIRE(cit->second == "twenty");
}