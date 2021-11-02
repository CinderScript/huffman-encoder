cmake_minimum_required(VERSION 3.16)
project(huffman_maynag)

set(CMAKE_CXX_STANDARD 14)

add_executable(huffman_maynag main.cpp huffman.cpp huffman.h general_functions.cpp general_functions.h)