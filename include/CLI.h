#ifndef HUFFMAN_CLI_H
#define HUFFMAN_CLI_H

#include <optional>
#include <exception>

#include "main.h"

struct cli_exception : std::exception {
    explicit cli_exception(const char *);

    const char *what() const noexcept override;

private:
    std::string what_;
};

class CLI final {
public:
    request_s get_request(std::size_t argc, const char *const *argv);

    void out_statistic(data_size_s) const;

private:
    operation_e request_type;
};


#endif //HUFFMAN_CLI_H