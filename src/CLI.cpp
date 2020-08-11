#include <iostream>
#include <cstring>

#include "CLI.h"

namespace {

struct cli_request {
    request_s request{};
    bool flag_c = false, flag_u = false, flag_f = false, flag_o = false;
};

inline void read_file_flag(cli_request &request, size_t &argc, const char *const *&argv) {
    if (request.flag_f) throw cli_exception{"Flags repetition is incorrect"};
    request.request.input_file = argv[1];
    request.flag_f = true;
    argc -= 2;
    argv += 2;
}

inline void read_output_flag(cli_request &request, size_t &argc, const char *const *&argv) {
    if (request.flag_o) throw cli_exception{"Flags repetition is incorrect"};
    request.request.output_file = argv[1];
    request.flag_o = true;
    argc -= 2;
    argv += 2;
}

bool read_flag(cli_request &request, size_t &argc, const char *const *&argv) {
    if (argc == 0) return false;
    if (argv[0][0] != '-') throw cli_exception{"Unrecognised symbol. Expect flag"};
    switch (argv[0][1]) {
        case 'c': {
            if (request.flag_c) throw cli_exception{"Flags repetition is incorrect"};
            if (request.flag_u) throw cli_exception{"The flag combination is not compatible"};
            request.request.operation = operation_e::COMPRESS;
            request.flag_c = true;
            --argc;
            ++argv;
            break;
        }
        case 'u': {
            if (request.flag_u) throw cli_exception{"Flags repetition is incorrect"};
            if (request.flag_c) throw cli_exception{"The flag combination is not compatible"};
            request.request.operation = operation_e::EXTRACT;
            request.flag_u = true;
            --argc;
            ++argv;
            break;
        }
        case 'f': {
            read_file_flag(request, argc, argv);
            break;
        }
        case 'o': {
            read_output_flag(request, argc, argv);
            break;
        }
        case '-' : {
            switch (argv[0][2]) {
                case 'f' : {
                    if (std::strcmp("file", argv[0] + 2) != 0)
                        throw cli_exception{"Find unsupported flag"};
                    read_file_flag(request, argc, argv);
                    break;
                }
                case 'o' : {
                    if (std::strcmp("output", argv[0] + 2) != 0)
                        throw cli_exception{"Find unsupported flag"};
                    read_output_flag(request, argc, argv);
                    break;
                }
                default:
                    throw cli_exception{"Find unsupported flag"};
            }
            break;
        }
        default:
            throw cli_exception{"Find unsupported flag"};
    }
    return true;
}

}


request_s CLI::get_request(std::size_t argc, const char *const *argv) {
    cli_request request{};
    while (read_flag(request, argc, argv)) {}
    if (!request.flag_f || !request.flag_o || (!request.flag_c && !request.flag_u)) throw cli_exception{"All flags are required: [-f] [-o] [-c|-u]"};
    request_type = request.request.operation;
    return request.request;
}


void CLI::out_statistic(data_size_s data_size) const {
    if (request_type == operation_e::COMPRESS) {
        std::cout << data_size.origin << '\n' << data_size.compress << '\n' << data_size.dict << std::endl;
    } else if (request_type == operation_e::EXTRACT) {
        std::cout << data_size.compress << '\n' << data_size.origin << '\n' << data_size.dict << std::endl;
    }
}

cli_exception::cli_exception(const char *definition) : what_{definition} {}

const char *cli_exception::what() const noexcept {
    return what_.c_str();
}
