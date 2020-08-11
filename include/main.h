#ifndef HUFFMAN_MAIN_H
#define HUFFMAN_MAIN_H

#include <string>


struct data_size_s final {
    std::size_t origin, compress, dict;
};

enum class operation_e {
    COMPRESS, EXTRACT
};

struct request_s final {
    std::string input_file, output_file;
    operation_e operation;
};


#endif //HUFFMAN_MAIN_H
