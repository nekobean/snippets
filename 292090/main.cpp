#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

#define N 1000            // テスト回数
#define TYPE unsigned int // ビットを保持する型

// 質問記載のやり方
////////////////////////////////////////////////////////////

/**
 * @brief カウント
 */
int sum_origin(TYPE bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i)
        cnt += (bits >> (i * 3)) & 0b111;

    return cnt;
}

/**
 * @brief 1以上の要素数
 */
int ge1_origin(TYPE bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i) {
        if ((bits >> i * 3) & 0b111)
            cnt++;
    }

    return cnt;
}

/**
 * @brief 2以上の要素数
 */
int ge2_origin(TYPE bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i) {
        if (((bits >> i * 3) & 0b111) >= 2)
            cnt++;
    }

    return cnt;
}

/**
 * @brief 2の要素数
 */
int eq2_origin(TYPE bits)
{
    int cnt = 0;
    for (int i = 0; i < 9; ++i) {
        if (((bits >> i * 3) & 0b111) == 2)
            cnt++;
    }

    return cnt;
}

// https://teratail.com/questions/292090#reply-413269
// SHOMIさんに回答していただいたコード
////////////////////////////////////////////////////////////

int sum_shomi(TYPE bits)
{
    bits = (bits & 0b00000111000111000111000111000111) +
           (bits >> 3 & 0b00000000000111000111000111000111);
    bits = (bits & 0b00001111000000001111000000001111) +
           (bits >> 6 & 0b00000000000000001111000000001111);
    bits = (bits & 0b00011111000000000000000000011111) +
           (bits >> 12 & 0b00000000000000000000000000011111);
    return (bits & 0b00000000000000000000000000111111) + (bits >> 24);
}

int ge1_shomi(TYPE bits)
{
    bits |= (bits & 0b00000010010010010010010010010010) >> 1;
    bits |= (bits & 0b00000100100100100100100100100100) >> 2;
    bits = (bits & 0b00000001000001000001000001000001) +
           (bits >> 3 & 0b00000000000001000001000001000001);
    bits = (bits & 0b00000011000000000011000000000011) +
           (bits >> 6 & 0b00000000000000000011000000000011);
    bits = (bits & 0b00000111000000000000000000000111) +
           (bits >> 12 & 0b00000000000000000000000000000111);
    return (bits & 0b00000000000000000000000000001111) + (bits >> 24);
}

int ge2_shomi(TYPE bits)
{
    bits |= bits >> 1;
    bits >>= 1;
    bits = (bits & 0b00000001000001000001000001000001) +
           (bits >> 3 & 0b00000000000001000001000001000001);
    bits = (bits & 0b00000011000000000011000000000011) +
           (bits >> 6 & 0b00000000000000000011000000000011);
    bits = (bits & 0b00000111000000000000000000000111) +
           (bits >> 12 & 0b00000000000000000000000000000111);
    return (bits & 0b00000000000000000000000000001111) + (bits >> 24);
}

int eq2_shomi(TYPE bits)
{
    bits ^= 0b00000010010010010010010010010010;
    bits |= (bits & 0b00000010010010010010010010010010) >> 1;
    bits |= (bits & 0b00000100100100100100100100100100) >> 2;
    bits = (bits & 0b00000001000001000001000001000001) +
           (bits >> 3 & 0b00000000000001000001000001000001);
    bits = (bits & 0b00000011000000000011000000000011) +
           (bits >> 6 & 0b00000000000000000011000000000011);
    bits = (bits & 0b00000111000000000000000000000111) +
           (bits >> 12 & 0b00000000000000000000000000000111);
    return 9 - ((bits & 0b00000000000000000000000000001111) + (bits >> 24));
}

// https://teratail.com/questions/292090#reply-413272
// kazuma-s さんに回答していただいたコード (高速化バージョン)
////////////////////////////////////////////////////////////

#define A3(x) x, x + 1, x + 2, x + 3, x + 4, x + 5, x + 6, x + 7
#define A6(x) A3(x), A3(x + 1), A3(x + 2), A3(x + 3), A3(x + 4), A3(x + 5), A3(x + 6), A3(x + 7)
#define A9(x) A6(x), A6(x + 1), A6(x + 2), A6(x + 3), A6(x + 4), A6(x + 5), A6(x + 6), A6(x + 7)
char a[512] = {A9(0)};

#define B3(x) x, x + 1, x + 1, x + 1, x + 1, x + 1, x + 1, x + 1
#define B6(x) B3(x), B3(x + 1), B3(x + 1), B3(x + 1), B3(x + 1), B3(x + 1), B3(x + 1), B3(x + 1)
#define B9(x) B6(x), B6(x + 1), B6(x + 1), B6(x + 1), B6(x + 1), B6(x + 1), B6(x + 1), B6(x + 1)
char b[512] = {B9(0)};

#define C3(x) x, x, x + 1, x + 1, x + 1, x + 1, x + 1, x + 1
#define C6(x) C3(x), C3(x), C3(x + 1), C3(x + 1), C3(x + 1), C3(x + 1), C3(x + 1), C3(x + 1)
#define C9(x) C6(x), C6(x), C6(x + 1), C6(x + 1), C6(x + 1), C6(x + 1), C6(x + 1), C6(x + 1)
char c[512] = {C9(0)};

#define D3(x) x, x, x + 1, x, x, x, x, x
#define D6(x) D3(x), D3(x), D3(x + 1), D3(x), D3(x), D3(x), D3(x), D3(x)
#define D9(x) D6(x), D6(x), D6(x + 1), D6(x), D6(x), D6(x), D6(x), D6(x)
char d[512] = {D9(0)};

int sum_kazuma(TYPE bits)
{
    int cnt = (bits >> 3 & 07070707) + (bits & 07070707);
    cnt = (cnt >> 6 & 0170017) + (cnt & 0170017);
    cnt = (cnt >> 12 & 037) + (cnt & 037);
    cnt += bits >> 24;
    return cnt;
}

int ge1_kazuma(TYPE bits)
{
    int cnt = bits >> 2 | bits >> 1 | bits;
    cnt = (cnt >> 3 & 01010101) + (cnt & 01010101) + (cnt >> 24 & 1);
    cnt = (cnt >> 6 & 030003) + (cnt & 030003);
    cnt = (cnt >> 12 & 7) + (cnt & 7);

    return cnt;
}

int ge2_kazuma(TYPE bits)
{
    int cnt = bits >> 2 | bits >> 1;
    cnt = (cnt >> 3 & 01010101) + (cnt & 01010101) + (cnt >> 24 & 1);
    cnt = (cnt >> 6 & 030003) + (cnt & 030003);
    cnt = (cnt >> 12 & 7) + (cnt & 7);

    return cnt;
}

int eq2_kazuma(TYPE bits)
{
    int cnt = bits ^ 0555555555;
    cnt &= cnt >> 2 & cnt >> 1;
    cnt = (cnt >> 3 & 01010101) + (cnt & 01010101) + (cnt >> 24);
    cnt = (cnt >> 6 & 030003) + (cnt & 030003);
    cnt = (cnt >> 12 & 7) + (cnt & 7);

    return cnt;
}

// https://teratail.com/questions/292090#reply-413272
// kazuma-s さんに回答していただいたコード (高速化バージョン)
////////////////////////////////////////////////////////////

int sum_kazuma_fast(TYPE bits)
{
    return a[bits >> 18 & 0777] + a[bits >> 9 & 0777] + a[bits & 0777];
}

int ge1_kazuma_fast(TYPE bits)
{
    return b[bits >> 18 & 0777] + b[bits >> 9 & 0777] + b[bits & 0777];
}

int ge2_kazuma_fast(TYPE bits)
{
    return c[bits >> 18 & 0777] + c[bits >> 9 & 0777] + c[bits & 0777];
}

int eq2_kazuma_fast(TYPE bits)
{
    return d[bits >> 18 & 0777] + d[bits >> 9 & 0777] + d[bits & 0777];
}

// https://teratail.com/questions/292090#reply-413519
// kichirb3 さんに回答していただいたコード
////////////////////////////////////////////////////////////

#include <intrin.h>

int sum_kichirb(TYPE bits)
{
    return _mm_popcnt_u64(bits & 0x4924924) * 4 + _mm_popcnt_u64(bits & 0x2492492) * 2 +
           _mm_popcnt_u64(bits & 0x1249249);
}

int ge1_kichirb(TYPE bits)
{
    bits |= (bits << 1);
    bits |= (bits << 1);
    return _mm_popcnt_u64(bits & 0x4924924);
}

int ge2_kichirb(TYPE bits)
{
    bits |= (bits << 1);
    return _mm_popcnt_u64(bits & 0x4924924);
}

int eq2_kichirb(TYPE bits)
{
    unsigned bit = bits & 0x5b6db6d;
    bit |= (bit >> 1);
    bit |= (bit << 1);
    bit &= 0x2492492;
    return _mm_popcnt_u64((bits & ~bit) & 0x2492492);
}

////////////////////////////////////////////////////////////

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

using Case = std::tuple<std::string, TYPE, TYPE, TYPE, TYPE, TYPE>;

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
        TYPE bits, cnt, ge1, ge2, eq2;
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
    {
        std::cout << "original" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = sum_origin(bits);

                if (cnt != actual)
                    std::cout << "wrong " << cnt << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "SHOMI san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = sum_kazuma(bits);

                if (cnt != actual)
                    std::cout << "wrong " << cnt << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = sum_kazuma(bits);

                if (cnt != actual)
                    std::cout << "wrong " << cnt << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san (fast)" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = sum_kazuma_fast(bits);

                if (cnt != actual)
                    std::cout << "wrong " << cnt << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kichirb san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = sum_kichirb(bits);

                if (cnt != actual)
                    std::cout << "wrong " << cnt << " vs " << actual << std::endl;
            }
        }
    }
}

void test_ge1(const std::vector<Case> &cases)
{
    std::cout << "[test case: ge1]" << std::endl;
    {
        std::cout << "original" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge1_origin(bits);

                if (ge1 != actual)
                    std::cout << "wrong " << ge1 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "SHOMI san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge1_shomi(bits);

                if (ge1 != actual)
                    std::cout << "wrong " << ge1 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge1_kazuma(bits);

                if (ge1 != actual)
                    std::cout << "wrong " << ge1 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san (fast)" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge1_kazuma_fast(bits);

                if (ge1 != actual)
                    std::cout << "wrong " << ge1 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kichirb3 san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge1_kichirb(bits);

                if (ge1 != actual)
                    std::cout << "wrong " << ge1 << " vs " << actual << std::endl;
            }
        }
    }
}

void test_ge2(const std::vector<Case> &cases)
{
    std::cout << "[test case: ge2]" << std::endl;
    {
        std::cout << "original" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge2_origin(bits);

                if (ge2 != actual)
                    std::cout << "wrong " << ge2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "SHOMI san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge2_shomi(bits);

                if (ge2 != actual)
                    std::cout << "wrong " << ge2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge2_kazuma(bits);

                if (ge2 != actual)
                    std::cout << "wrong " << ge2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san (fast)" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge2_kazuma_fast(bits);

                if (ge2 != actual)
                    std::cout << "wrong " << ge2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kichirb3 san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = ge2_kichirb(bits);

                if (ge2 != actual)
                    std::cout << "wrong " << ge2 << " vs " << actual << std::endl;
            }
        }
    }
}

void test_eq2(const std::vector<Case> &cases)
{
    std::cout << "[test case: eq2]" << std::endl;
    {
        std::cout << "original" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = eq2_origin(bits);

                if (eq2 != actual)
                    std::cout << "wrong " << eq2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "SHOMI san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = eq2_shomi(bits);

                if (eq2 != actual)
                    std::cout << "wrong " << eq2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = eq2_kazuma(bits);

                if (eq2 != actual)
                    std::cout << "wrong " << eq2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kazuma san (fast)" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = eq2_kazuma_fast(bits);

                if (eq2 != actual)
                    std::cout << "wrong " << eq2 << " vs " << actual << std::endl;
            }
        }
    }
    {
        std::cout << "kichirb3 san" << std::endl;
        Timer timer;
        for (size_t i = 0; i < N; ++i) {
            for (const auto &[key, bits, cnt, ge1, ge2, eq2] : cases) {
                TYPE actual = eq2_kichirb(bits);

                if (eq2 != actual)
                    std::cout << "wrong " << eq2 << " vs " << actual << std::endl;
            }
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
    test_ge1(cases);
    test_ge2(cases);
    test_eq2(cases);
}