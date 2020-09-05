cmake_minimum_required(VERSION 3.0.0)
project(sample VERSION 0.1.0)

# Force MSVC to interpret source files as utf8 without bom.
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(sample main.cpp)
configure_file(sample.csv ${CMAKE_BINARY_DIR})