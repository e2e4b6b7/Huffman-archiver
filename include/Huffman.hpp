#ifndef HUFFMAN_HUFFMAN_HPP
#define HUFFMAN_HUFFMAN_HPP


#include <memory>
#include <iosfwd>
#include <optional>
#include <vector>
#include <array>
#include <limits>

#include "IOBitIterator.hpp"

constexpr std::size_t HUFFMAN_UNIQUE_SYMBOLS_COUNT = 256;

class HuffmanTree;

class HuffmanNode final {
    friend HuffmanTree;

public:
    explicit HuffmanNode(char code) : code_{code} {}

    HuffmanNode(std::unique_ptr<HuffmanNode> &&left, std::unique_ptr<HuffmanNode> &&right) : left_{std::move(left)}, right_{std::move(right)} {}

private:
    void out_dfs(std::vector<bool> &, std::string &) const;

    void make_dict(std::vector<bool> &, std::array<std::vector<bool>, HUFFMAN_UNIQUE_SYMBOLS_COUNT> &) const;

    static std::unique_ptr<HuffmanNode> read_tree(std::istreambuf_iterator<char> &, input_bit_iterator &);

    std::unique_ptr<HuffmanNode> left_, right_;
    std::optional<char> code_;
};


class HuffmanTree final {
public:
    explicit HuffmanTree(std::unique_ptr<HuffmanNode> &&node, std::size_t uniq_symbol_count, std::size_t symbol_count);

    static std::unique_ptr<HuffmanTree> build_tree(std::istreambuf_iterator<char>);

    static std::unique_ptr<HuffmanTree> read_tree(std::istreambuf_iterator<char>);

    void out_dict(std::ostreambuf_iterator<char>) const;

    void encode(std::istreambuf_iterator<char>, std::ostreambuf_iterator<char>) const;

    void decode(std::istreambuf_iterator<char>, std::ostreambuf_iterator<char>) const;

    std::array<std::vector<bool>, HUFFMAN_UNIQUE_SYMBOLS_COUNT> make_dict() const;

    std::size_t get_original_size() const;

    std::size_t get_dict_size() const;

    std::optional<std::size_t> get_compressed_size() const;

private:

    std::unique_ptr<HuffmanNode> root_;
    std::size_t uniq_symbol_count_, symbol_count_;
    mutable std::optional<std::size_t> compress_size_;
};


#endif //HUFFMAN_HUFFMAN_HPP
