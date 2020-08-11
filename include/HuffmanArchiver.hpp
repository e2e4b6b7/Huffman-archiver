#ifndef HUFFMAN_HUFFMANARCHIVER_HPP
#define HUFFMAN_HUFFMANARCHIVER_HPP

#include <iosfwd>

#include "main.h"

class HuffmanArchiver final {
public:
    static data_size_s compress(std::istream &input_stream, std::ostream &output_stream);

    static data_size_s extract(std::istream &input_stream, std::ostream &output_stream);
};


#endif //HUFFMAN_HUFFMANARCHIVER_HPP
