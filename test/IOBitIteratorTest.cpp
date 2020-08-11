#include <sstream>
#include <vector>
#include "doctest.h"
#include "IOBitIterator.hpp"

TEST_CASE ("Simple out test") {
    std::vector<bool> test;
    for (std::size_t i = 0; i < 8; ++i) {
        test.push_back('a' & (1u << (7u - i)));
    }
    std::stringstream ss{};
    std::ostreambuf_iterator<char> ostreambuf{ss};
    out_bit_iterator out_bit{ostreambuf};
    for (int i = 0; i < 8; ++i) {
        out_bit = test[i];
    }
    out_bit.flush();
    REQUIRE_EQ(ss.get(), 'a');
}

TEST_CASE ("Simple in test") {
    std::stringstream ss{"a"};
    std::istreambuf_iterator<char> istreambuf{ss};
    input_bit_iterator input_bit{istreambuf};
    std::vector<bool> test;
    for (int i = 0; i < 8; ++i) {
        test.push_back(*(input_bit));
        ++input_bit;
    }
    for (std::size_t i = 0; i < 8; ++i) {
        REQUIRE_EQ(test[i], 0 != ('a' & (1u << (7u - i))));
    }
}