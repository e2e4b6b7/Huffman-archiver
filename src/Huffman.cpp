#include <algorithm>
#include <sstream>
#include <iostream>

#include "Huffman.hpp"


HuffmanTree::HuffmanTree(std::unique_ptr<HuffmanNode> &&node,
                         std::size_t uniq_symbol_count,
                         std::size_t symbol_count) :
        root_{std::move(node)},
        uniq_symbol_count_{uniq_symbol_count},
        symbol_count_{symbol_count} {}


namespace {

std::vector<std::pair<int, std::unique_ptr<HuffmanNode>>> count_symbols(std::istreambuf_iterator<char> &input) {
    int count[HUFFMAN_UNIQUE_SYMBOLS_COUNT];
    std::fill(count, count + HUFFMAN_UNIQUE_SYMBOLS_COUNT, 0);
    std::for_each(input, std::istreambuf_iterator<char>{}, [&count](char c) { ++count[static_cast<unsigned char>(c)]; });
    std::vector<std::pair<int, std::unique_ptr<HuffmanNode>>> symbols;
    for (std::size_t i = 0; i < HUFFMAN_UNIQUE_SYMBOLS_COUNT; ++i) {
        if (count[i] != 0) {
            symbols.emplace_back(count[i], std::make_unique<HuffmanNode>(static_cast<char>(i)));
        }
    }
    return symbols;
}

template<typename T>
struct two_queues {

    explicit two_queues(std::vector<std::pair<int, T>> init_elements) : first{std::move(init_elements)} {
        std::sort(first.begin(), first.end(), [](const auto &f, const auto &s) { return f.first < s.first; });
    }

    inline std::pair<int, T> top_pop() {
        if (pos_first == first.size() || (pos_second != second.size() && second[pos_second].first < first[pos_first].first)) {
            return std::move(second[pos_second++]);
        } else return std::move(first[pos_first++]);
    }

    inline void emplace_back(int val, T data) {
        second.emplace_back(val, std::move(data));
    }

    inline std::size_t size() {
        return first.size() - pos_first + second.size() - pos_second;
    }

private:
    std::size_t pos_first = 0, pos_second = 0;
    std::vector<std::pair<int, T>> first, second;
};

}

std::unique_ptr<HuffmanTree> HuffmanTree::build_tree(std::istreambuf_iterator<char> input) {
    std::vector<std::pair<int, std::unique_ptr<HuffmanNode>>> symbols = count_symbols(input);
    if (symbols.size() == 1) {
        symbols.emplace_back(0, std::make_unique<HuffmanNode>(symbols[0].second->code_.value() + 1));
    }

    std::size_t dict_count = symbols.size();

    two_queues queue{std::move(symbols)};
    while (queue.size() > 1) {
        auto first = queue.top_pop();
        auto second = queue.top_pop();
        queue.emplace_back(first.first + second.first, std::make_unique<HuffmanNode>(std::move(first.second), std::move(second.second)));
    }
    auto root = queue.top_pop();

    std::size_t symbols_count = root.first;
    return std::make_unique<HuffmanTree>(std::move(root.second), dict_count, symbols_count);
}


namespace {

inline void out_bit_code(const std::vector<bool> &bit_code, std::ostreambuf_iterator<char> &out) {
    std::size_t out_size = (bit_code.size() + 7) / 8;
    for (std::size_t i = 0; i < out_size; ++i) {
        unsigned char symbol = 0;
        for (std::size_t j = 0; j < 8; ++j) {
            if (i * 8 + j < bit_code.size()) {
                symbol += bit_code[i * 8 + j] << (7u - j);
            }
        }
        out = symbol;
    }
}

inline uint32_t read_uint32(std::istreambuf_iterator<char> &input) {
    uint32_t int32{0};
    for (int i = 0; i < 4; ++i) {
        int32 <<= 8u;
        int32 |= static_cast<uint8_t>(*input);
        ++input;
    }
    return int32;
}

inline void out_uint32(std::ostreambuf_iterator<char> &out, uint32_t int32) {
    for (uint8_t i = 24; i < 32; i -= 8) {
        out = static_cast<unsigned char>(int32 >> i);
    }
}

}


void HuffmanTree::out_dict(std::ostreambuf_iterator<char> out) const {
    std::vector<bool> out_order;
    std::string codes_order;
    root_->out_dfs(out_order, codes_order);

    out = static_cast<unsigned char>(uniq_symbol_count_ - 1);
    out_uint32(out, symbol_count_);

    std::copy(codes_order.begin(), codes_order.end(), out);

    out_bit_code(out_order, out);
}

void HuffmanNode::out_dfs(std::vector<bool> &out_order, std::string &codes) const {
    if (code_) {
        out_order.push_back(false);
        codes.push_back(code_.value());
    } else {
        out_order.push_back(true);
        left_->out_dfs(out_order, codes);
        right_->out_dfs(out_order, codes);
    }
}


void HuffmanTree::encode(std::istreambuf_iterator<char> input, std::ostreambuf_iterator<char> out) const {
    const std::array<std::vector<bool>, HUFFMAN_UNIQUE_SYMBOLS_COUNT> dict = make_dict();

    out_bit_iterator out_bit{out};
    std::for_each(input, std::istreambuf_iterator<char>{}, [&out_bit, dict](char c) {
        const std::vector<bool> &code = dict[static_cast<unsigned char>(c)];
        //std::copy(code.begin(), code.end(), out_bit);
        for (bool s : code) {
            out_bit = s;
        }
    });

    out_bit.flush();
    compress_size_ = out_bit.get_count();
}

std::array<std::vector<bool>, HUFFMAN_UNIQUE_SYMBOLS_COUNT> HuffmanTree::make_dict() const {
    std::array<std::vector<bool>, HUFFMAN_UNIQUE_SYMBOLS_COUNT> dict{};
    std::vector<bool> code;
    root_->make_dict(code, dict);
    return dict;
}

void HuffmanNode::make_dict(std::vector<bool> &code, std::array<std::vector<bool>, HUFFMAN_UNIQUE_SYMBOLS_COUNT> &dict) const {
    if (code_) {
        dict[static_cast<unsigned char>(code_.value())] = code;
    } else {
        code.push_back(false);
        left_->make_dict(code, dict);
        code.pop_back();
        code.push_back(true);
        right_->make_dict(code, dict);
        code.pop_back();
    }
}

std::unique_ptr<HuffmanTree> HuffmanTree::read_tree(std::istreambuf_iterator<char> input) {
    std::size_t dict_size = 1 + static_cast<std::size_t>(static_cast<unsigned char>(*input));
    ++input;
    std::size_t text_size{read_uint32(input)};

    std::string symbols;
    std::copy_n(input, dict_size, std::back_insert_iterator(symbols));
    std::stringstream ss{symbols};
    std::istreambuf_iterator<char> out_symbol{ss};

    input_bit_iterator input_bit{++input};
    std::unique_ptr<HuffmanNode> root_node = HuffmanNode::read_tree(out_symbol, input_bit);
    input_bit.flush();
    return std::make_unique<HuffmanTree>(std::move(root_node), dict_size, text_size);
}


std::unique_ptr<HuffmanNode> HuffmanNode::read_tree(std::istreambuf_iterator<char> &symbols, input_bit_iterator &input_bit) {
    if (*input_bit) {
        ++input_bit;
        std::unique_ptr<HuffmanNode> left = read_tree(symbols, input_bit);
        std::unique_ptr<HuffmanNode> right = read_tree(symbols, input_bit);
        return std::make_unique<HuffmanNode>(std::move(left), std::move(right));
    } else {
        ++input_bit;
        return std::make_unique<HuffmanNode>(*(symbols++));
    }
}


void HuffmanTree::decode(std::istreambuf_iterator<char> input, std::ostreambuf_iterator<char> out) const {
    HuffmanNode *root = root_.get(), *pos = root_.get();
    input_bit_iterator input_bit(input);
    std::size_t symbol_count{0};
    for (; symbol_count < symbol_count_; ++input_bit) {
        if (*input_bit) {
            pos = pos->right_.get();
        } else {
            pos = pos->left_.get();
        }
        if (pos->code_) {
            out = pos->code_.value();
            pos = root;
            ++symbol_count;
        }
    }

    compress_size_ = input_bit.get_count();
}

std::size_t HuffmanTree::get_original_size() const {
    return symbol_count_;
}

std::size_t HuffmanTree::get_dict_size() const {
    return 1 + 4 + uniq_symbol_count_ + (((2 * uniq_symbol_count_ - 1) + 7) / 8);
}

std::optional<std::size_t> HuffmanTree::get_compressed_size() const {
    return compress_size_;
}