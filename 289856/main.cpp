#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * @brief 1行分パースする。
 * 
 * @param line 行
 * @param delim 区切り文字
 * @return std::vector<double> 値の一覧
 */
std::vector<double> parse_line(const std::string &line, char delim = ',')
{
    std::vector<double> values;

    std::istringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delim))
        values.push_back(std::stod(token));

    return values;
}

int main(int, char **)
{
    std::ifstream ifs("sample.csv");
    std::string line;
    std::vector<std::vector<double>> mat;

    while (std::getline(ifs, line)) {
        // 1行分解析する。
        std::vector<double> values = parse_line(line);
        mat.push_back(values);
    }

    // 出力する。
    if (!mat.empty()) {
        std::cout << "matrix size: " << mat.size() << "x" << mat.front().size() << std::endl;

        for (auto row : mat) {
            for (auto x : row)
                std::cout << x << " ";
            std::cout << std::endl;
        }
    }
}
