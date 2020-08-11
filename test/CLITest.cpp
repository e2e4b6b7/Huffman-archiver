#include <cstring>

#include "doctest.h"
#include "CLI.h"

TEST_CASE ("Simple correct input test") {
    CLI cli{};
    const char **argv = new const char *[5];
    SUBCASE("-c") {
        argv[0] = "-f";
        argv[1] = "some.txt";
        argv[2] = "-o";
        argv[3] = "some.hf";
        argv[4] = "-c";
        request_s request = cli.get_request(5, argv);
        REQUIRE(std::strcmp(argv[1], request.input_file.c_str()) == 0);
        REQUIRE(std::strcmp(argv[3], request.output_file.c_str()) == 0);
        REQUIRE_EQ(request.operation, operation_e::COMPRESS);
    }
    SUBCASE("-u") {
        argv[0] = "-f";
        argv[1] = "some.hf";
        argv[2] = "-u";
        argv[3] = "-o";
        argv[4] = "some.txt";
        request_s request = cli.get_request(5, argv);
        REQUIRE(std::strcmp(argv[1], request.input_file.c_str()) == 0);
        REQUIRE(std::strcmp(argv[4], request.output_file.c_str()) == 0);
        REQUIRE_EQ(request.operation, operation_e::EXTRACT);
    }
    SUBCASE("--file --output") {
        argv[0] = "-u";
        argv[1] = "--file";
        argv[2] = "some.hf";
        argv[3] = "--output";
        argv[4] = "some.txt";
        request_s request = cli.get_request(5, argv);
        REQUIRE(std::strcmp(argv[2], request.input_file.c_str()) == 0);
        REQUIRE(std::strcmp(argv[4], request.output_file.c_str()) == 0);
        REQUIRE_EQ(request.operation, operation_e::EXTRACT);
    }
}

TEST_CASE ("Simple errors") {
    CLI cli{};
    SUBCASE("-c -u") {
        const char **argv = new const char *[6];
        argv[0] = "-f";
        argv[1] = "some.txt";
        argv[2] = "-o";
        argv[3] = "some.hf";
        argv[4] = "-c";
        argv[5] = "-u";
        bool catch_ = false;
        try {
            request_s request = cli.get_request(6, argv);
        } catch (cli_exception &) {
            catch_ = true;
        }
        REQUIRE(catch_);
    }
    SUBCASE("double -f") {
        const char **argv = new const char *[5];
        argv[0] = "-f";
        argv[1] = "some.hf";
        argv[2] = "-u";
        argv[3] = "-f";
        argv[4] = "some.txt";
        bool catch_ = false;
        try {
            request_s request = cli.get_request(5, argv);
        } catch (cli_exception &) {
            catch_ = true;
        }
        REQUIRE(catch_);
    }
    SUBCASE(" no --file") {
        const char **argv = new const char *[5];
        argv[0] = "-u";
        argv[1] = "--output";
        argv[2] = "some.txt";
        bool catch_ = false;
        try {
            request_s request = cli.get_request(3, argv);
        } catch (cli_exception &) {
            catch_ = true;
        }
        REQUIRE(catch_);
    }
}
