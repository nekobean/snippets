#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using Mat = std::vector<std::vector<double>>;

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

/**
 * @brief CSV を解析する。
 * 
 * @param path CSV ファイルのパス
 * @return Mat 行列
 */
Mat parse_csv(const std::string &path)
{
    Mat mat;

    std::ifstream ifs(path);
    if (!ifs) {
        std::cout << "Failed to open csv path" << std::endl;
        return mat;
    }
    std::string line;

    while (std::getline(ifs, line)) {
        // 1行分解析する。
        std::vector<double> values = parse_line(line);
        mat.push_back(values);
    }

    return mat;
}

int main(int, char **)
{
    Mat mat;
    try {
        mat = parse_csv("sample.csv");
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
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
