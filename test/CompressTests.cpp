#include <sstream>

#include "doctest.h"
#include "Huffman.hpp"

TEST_CASE ("Compress test") {
    auto node_a = std::make_unique<HuffmanNode>('a');
    auto node_b = std::make_unique<HuffmanNode>('b');
    auto node_c = std::make_unique<HuffmanNode>('c');
    auto node_d = std::make_unique<HuffmanNode>('d');
    auto node_ab = std::make_unique<HuffmanNode>(std::move(node_a), std::move(node_b));
    auto node_abc = std::make_unique<HuffmanNode>(std::move(node_ab), std::move(node_c));
    auto node_abcd = std::make_unique<HuffmanNode>(std::move(node_abc), std::move(node_d));
    HuffmanTree tree{std::move(node_abcd), 4, 0};

    SUBCASE("1") {
        std::stringstream my_file{"aa"};
        std::stringstream my_file_compressed{};
        tree.encode(std::istreambuf_iterator(my_file), std::ostreambuf_iterator(my_file_compressed));
        unsigned char c = my_file_compressed.get();
        REQUIRE_EQ(c, 0);//000000
        REQUIRE_EQ(my_file_compressed.get(), -1);
    }

    SUBCASE("2") {
        std::stringstream my_file{"da"};
        std::stringstream my_file_compressed{};
        tree.encode(std::istreambuf_iterator(my_file), std::ostreambuf_iterator(my_file_compressed));
        unsigned char c = my_file_compressed.get();
        REQUIRE_EQ(c, 128);//1000
        REQUIRE_EQ(my_file_compressed.get(), -1);
    }

    SUBCASE("3") {
        std::stringstream my_file{"ad"};
        std::stringstream my_file_compressed{};
        tree.encode(std::istreambuf_iterator(my_file), std::ostreambuf_iterator(my_file_compressed));
        unsigned char c = my_file_compressed.get();
        REQUIRE_EQ(c, 16);//0001
        REQUIRE_EQ(my_file_compressed.get(), -1);
    }


    SUBCASE("4") {
        std::stringstream my_file{"dcb"};
        std::stringstream my_file_compressed{};
        tree.encode(std::istreambuf_iterator(my_file), std::ostreambuf_iterator(my_file_compressed));
        unsigned char c = my_file_compressed.get();
        REQUIRE_EQ(c, 164);//101001
        REQUIRE_EQ(my_file_compressed.get(), -1);
    }

}