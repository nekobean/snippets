cmake_minimum_required(VERSION 3.0.0)
project(sample)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC の場合、UTF-8 として解釈するように指定する。
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


configure_file(testcase.txt ${CMAKE_BINARY_DIR}/Release)

add_executable(${PROJECT_NAME} main.cpp)