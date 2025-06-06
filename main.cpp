
#include <iostream>
#include <chrono>

#include "src/algorithms/montgomery_algorithm.h"
#include "src/algorithms/sos_algorithm.h"
#include "src/util/binary_helper.h"
#include "src/util/number_generator.h"
#include "src/util/result_parser.h"

typedef __int128 int128_t;

void runTest();



void runSimulation(int trails, const std::vector<int>& bits, const std::vector<Algorithm>& algorithms);

int main() {
    // runSimulation(1, {2, 4, 8, 16, 32, 64, 128}, {Algorithm::SOS});

    // SOS
    constexpr int128_t a = 7, e = 10, n = 13;

    const int128_t result = MontgomeryAlgorithm::monExp(a, e, n);
    std::cout << "Result Montgomery: " << NumberGenerator::numberToString(result) << std::endl;

    const std::vector<int> result_bin = SOSAlgorithm::monExp(a, e, n);
    std::cout << "Result SOS: ";
    BinaryHelper::printVector(result_bin);


    // FIPS
    // unsigned long long x = 123;
    // unsigned long long y = 456;
    // unsigned long long m = 1009;
    //
    // // r = 2^64 mod m
    // __uint128_t r128 = (__uint128_t(1) << 64);
    // unsigned long long r = (unsigned long long)(r128 % m);
    //
    // unsigned long long m_inv = 1;
    // for (int i = 0; i < 6; ++i)
    //     m_inv *= 2 - m * m_inv;
    //
    // unsigned long long m_prime = -m_inv;
    //
    // unsigned long long x_mont = ((__uint128_t)x * r) % m;
    // unsigned long long y_mont = ((__uint128_t)y * r) % m;
    //
    // auto start = std::chrono::high_resolution_clock::now();
    //
    // unsigned long long result_mont = montgomery_multiply_fips(x_mont, y_mont, m, m_prime, r);
    // unsigned long long result = montgomery_multiply_fips(result_mont, 1, m, m_prime, r);
    //
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double, std::micro> elapsed = end - start;
    //
    // std::cout << "Poprawny wynik: " << (x * y) % m << std::endl;
    //
    // std::cout << "Wynik mnożenia Montgomery (FIPS): " << result << std::endl;
    // std::cout << "Czas wykonania: " << elapsed.count() / 1000 << " ms" << std::endl;

    return 0;
}

void runSimulation(const int trails, const std::vector<int>& bits, const std::vector<Algorithm>& algorithms) {
    ResultParser parser;

    for (const auto& bit_num : bits) {
        for (int i = 0; i < trails; i++) {

            const int128_t n = NumberGenerator::generate(bit_num, true);
            const int128_t a = NumberGenerator::generate(bit_num, false, n);
            constexpr int e = 2;

            std::vector<int> result_bin;
            for (const auto& algorithm : algorithms) {
                std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

                if (algorithm == Algorithm::FIPS) {
                    start = std::chrono::high_resolution_clock::now();
                    // result_bin = FIPSAlgorithm::monExp(a, e, n);
                    end = std::chrono::high_resolution_clock::now();
                } else if (algorithm == Algorithm::SOS) {
                    start = std::chrono::system_clock::now();
                    result_bin = SOSAlgorithm::monExp(a, e, n);
                    end = std::chrono::high_resolution_clock::now();
                }

                const std::chrono::duration<double, std::milli> duration = end - start;
                parser.addScore(duration.count(), algorithm);
            }
            std::cout << "Bits: " << bit_num << "; " << i + 1 << "/" << trails << std::endl;
        }

        parser.calculateAverageScores(bit_num);
    }

    parser.saveScores("results.txt");
}


