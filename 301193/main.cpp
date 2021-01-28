#include <iostream>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

const double PI = 3.1415;

int add(int i, int j)
{
    return i + j;
}

void print_list(const std::vector<int> &list)
{
    for (auto x : list)
        std::cout << x << std::endl;
}

// PYBIND11_MODULE() の第一引数は pybind11_add_module() で設定した名前と同じにすること
PYBIND11_MODULE(sample, m)
{
    // モジュールの説明
    m.doc() = "pybind11 sample library";

    // 関数を登録
    m.def("add", &add, "A function which adds tow numbers");
    m.def("print_list", &print_list, "A function which print numbers");

    // 定数を登録
    m.attr("PI") = PI;
}
