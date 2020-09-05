cmake_minimum_required(VERSION 3.0.0)
project(sample VERSION 0.1.0)

include_directories(include)

add_executable(sample main.cpp)

configure_file(sample.json ${CMAKE_BINARY_DIR})