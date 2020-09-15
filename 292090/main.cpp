#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

// カウント
////////////////////////////////////////////////////////////

/**
 * @brief https://teratail.com/questions/292090#reply-413269
 *        SHOMIさんに回答していただいたコード
 */
int sum1(unsigned int bits)
{
    bits = (bits & 0b00000111000111000111000111000111) +
           (bits >> 3 & 0b00000111000111000111000111000111);
    bits = (bits & 0b00001111000000001111000000001111) +
           (bits >> 6 & 0b00000000000000001111000000001111);
    bits = (bits & 0b00011111000000000000000000011111) +
           (bits >> 12 & 0b00000000000000000000000000001111);
    return (bits & 0b00000000000000000000000000111111) + (bits >> 24);
}

/**
 * @brief 質問記載のやり方
 */
int sum_origin(unsigned int bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i)
        cnt += (bits >> (i * 3)) & 0b111;

    return cnt;
}

// 1以上の要素数
////////////////////////////////////////////////////////////

/**
 * @brief 質問記載のやり方
 */
int ge1_origin(unsigned int bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i) {
        if ((bits >> i * 3) & 0b111)
            cnt++;
    }

    return cnt;
}

// 2以上の要素数
////////////////////////////////////////////////////////////

/**
 * @brief 質問記載のやり方
 */
int ge2_origin(unsigned int bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i) {
        if (((bits >> i * 3) & 0b111) >= 2)
            cnt++;
    }

    return cnt;
}

// 2の要素数
////////////////////////////////////////////////////////////

/**
 * @brief 質問記載のやり方
 */
int eq2_origin(unsigned int bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i) {
        if (((bits >> i * 3) & 0b111) == 2)
            cnt++;
    }

    return cnt;
}

/**
 * @brief ベンチマーク用
 */
class Timer
{
public:
    Timer() : begin_(std::chrono::steady_clock::now())
    {
    }

    ~Timer()
    {
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_).count();
        std::cout << "===> elapsed time: " << elapsed << "s" << std::endl;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> begin_;
};

using Case =
    std::tuple<std::string, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>;

/**
 * @brief テストケース読み込み
 */
std::vector<Case> load_testcase()
{
    std::vector<Case> cases;
    std::ifstream ifs("testcase.txt");
    std::string line;
    std::getline(ifs, line); // skip header

    while (std::getline(ifs, line)) {
        unsigned int bits, cnt, ge1, ge2, eq2;
        std::string key;
        std::stringstream iss(line);

        // a[0]a[1]...a[9] ビット値 個数 >=1の個数 >=2の個数 ==2の個数
        iss >> key >> bits >> cnt >> ge1 >> ge2 >> eq2;
        cases.emplace_back(key, bits, cnt, ge1, ge2, eq2);
    }

    return cases;
}

void test_sum(const std::vector<Case> &cases)
{
    std::cout << "[test case: sum]" << std::endl;
    Timer timer;
    const size_t N = 1000;

    for (size_t i = 0; i < N; ++i) {
        for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
            unsigned int actual = sum_origin(bits);

            if (cnt != actual)
                std::cout << "wrong " << cnt << " vs " << actual << std::endl;
        }
    }
}

int main()
{

    // テストケースを読み込む
    /////////////////////////////////////////////////////////////////////
    std::vector<Case> cases;
    {
        Timer timer;
        cases = load_testcase();
        std::cout << cases.size() << " testcase loaded!" << std::endl;
    }

    // ベンチマーク
    /////////////////////////////////////////////////////////////////////
    test_sum(cases);
}