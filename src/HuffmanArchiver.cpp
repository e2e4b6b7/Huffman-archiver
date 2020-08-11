#include "HuffmanArchiver.hpp"
#include "Huffman.hpp"

data_size_s HuffmanArchiver::compress(std::istream &input_stream, std::ostream &output_stream) {
    if (input_stream.peek() == EOF) return {0, 0, 0};

    auto tree = HuffmanTree::build_tree(std::istreambuf_iterator<char>(input_stream));
    input_stream.clear();
    input_stream.seekg(0);

    tree->out_dict(std::ostreambuf_iterator<char>(output_stream));
    tree->encode(std::istreambuf_iterator<char>(input_stream), std::ostreambuf_iterator<char>(output_stream));
    return {tree->get_original_size(), tree->get_compressed_size().value(), tree->get_dict_size()};
}

data_size_s HuffmanArchiver::extract(std::istream &input_stream, std::ostream &output_stream) {
    if (input_stream.peek() == EOF) return {0, 0, 0};

    auto tree = HuffmanTree::read_tree(std::istreambuf_iterator<char>(input_stream));
    tree->decode(std::istreambuf_iterator<char>(input_stream), std::ostreambuf_iterator<char>(output_stream));
    return {tree->get_original_size(), tree->get_compressed_size().value(), tree->get_dict_size()};
}
