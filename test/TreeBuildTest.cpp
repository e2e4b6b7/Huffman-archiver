#include <sstream>
#include "doctest.h"
#include "Huffman.hpp"

TEST_CASE ("Uniform text") {
    std::stringstream my_file{"abcdefgh"};
    auto tree = HuffmanTree::build_tree(std::istreambuf_iterator<char>(my_file));
    auto dict = tree->make_dict();
    int count = 0;
    for (auto &elem : dict) {
        if (!elem.empty()) {
            ++count;
        }
    }
    REQUIRE_EQ(count, 8);
    REQUIRE_EQ(dict.at('a').size(), 3);
    REQUIRE_EQ(dict.at('b').size(), 3);
    REQUIRE_EQ(dict.at('c').size(), 3);
    REQUIRE_EQ(dict.at('d').size(), 3);
    REQUIRE_EQ(dict.at('e').size(), 3);
    REQUIRE_EQ(dict.at('f').size(), 3);
    REQUIRE_EQ(dict.at('g').size(), 3);
    REQUIRE_EQ(dict.at('h').size(), 3);
}

TEST_CASE ("Exp text") {
    std::stringstream my_file{"abccddddeeeeeeeeffffffffffffffffgggggggggggggggggggggggggggggggg"
                              "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"};
    auto tree = HuffmanTree::build_tree(std::istreambuf_iterator<char>(my_file));
    auto dict = tree->make_dict();
    int count = 0;
    for (auto &elem : dict) {
        if (!elem.empty()) {
            ++count;
        }
    }
    REQUIRE_EQ(count, 8);
    REQUIRE_EQ(dict.at('a').size(), 7);
    REQUIRE_EQ(dict.at('b').size(), 7);
    REQUIRE_EQ(dict.at('c').size(), 6);
    REQUIRE_EQ(dict.at('d').size(), 5);
    REQUIRE_EQ(dict.at('e').size(), 4);
    REQUIRE_EQ(dict.at('f').size(), 3);
    REQUIRE_EQ(dict.at('g').size(), 2);
    REQUIRE_EQ(dict.at('h').size(), 1);
}