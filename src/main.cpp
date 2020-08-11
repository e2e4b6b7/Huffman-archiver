#include <fstream>
#include <iostream>
#include <exception>

#include "CLI.h"
#include "HuffmanArchiver.hpp"

void process_request(std::size_t argc, const char *const *argv, CLI &cli) {
    request_s request = cli.get_request(argc - 1, argv + 1);
    std::ifstream input_stream{request.input_file, std::ios::binary};
    std::ofstream output_stream{request.output_file, std::ios::binary};
    if (!input_stream.is_open() || !output_stream.is_open()) {
        throw std::ios_base::failure("Cannot open file");
    }
    data_size_s data_size{};
    if (request.operation == operation_e::EXTRACT) {
        data_size = HuffmanArchiver::extract(input_stream, output_stream);
    } else if (request.operation == operation_e::COMPRESS) {
        data_size = HuffmanArchiver::compress(input_stream, output_stream);
    }
    cli.out_statistic(data_size);
}

int main(int argc, char **argv) {
    CLI cli{};
    try {
        process_request(argc, argv, cli);
    } catch (cli_exception &e) {
        std::cout << e.what() << std::endl;
        exit(1);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        exit(1);
    }
}
