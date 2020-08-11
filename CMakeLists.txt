cmake_minimum_required(VERSION 3.15)
project(hw_03)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

add_executable(hw_03 src/main.cpp src/HuffmanArchiver.cpp include/HuffmanArchiver.hpp src/Huffman.cpp include/Huffman.hpp src/CLI.cpp include/CLI.h include/main.h test/doctest.h src/IOBitIterator.cpp include/IOBitIterator.hpp)

target_include_directories(hw_03 PUBLIC include)

project(test_hw_03)

add_executable(test_hw_03 test/TestMain.cpp test/EndToEndTests.cpp src/HuffmanArchiver.cpp src/Huffman.cpp src/IOBitIterator.cpp test/doctest.h test/CompressTests.cpp test/TreeBuildTest.cpp test/IOBitIteratorTest.cpp test/CLITest.cpp src/CLI.cpp)

target_include_directories(test_hw_03 PUBLIC include tests)
