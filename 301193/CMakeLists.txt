cmake_minimum_required(VERSION 3.1.4)
project(sample VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

# PyBind11 を取得
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG        v2.6.0
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(sample main.cpp)
