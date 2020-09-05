#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int, char **)
{
    std::ifstream ifs("sample.json");
    json j;
    ifs >> j;

    // write prettified JSON to another file
    std::ofstream o("pretty.json");
    o << std::setw(4) << j << std::endl;
}
