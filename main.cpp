#include <iostream>
#include <chrono>

#include "src/algorithms/montgomery_algorithm.h"
#include "src/algorithms/sos_algorithm.h"
#include "src/algorithms/fips_algorithm.h"
#include "src/util/binary_helper.h"
#include "src/util/number_generator.h"
#include "src/util/result_parser.h"

typedef __int128 int128_t;

void runTest();



void runSimulation(int trails, const std::vector<int>& bits, const std::vector<Algorithm>& algorithms);

int main() {
    // runSimulation(1, {2, 4, 8, 16, 32, 64, 128}, {Algorithm::SOS});

    // SOS
    constexpr int128_t a = 9, e = 30, n = 15;

    const int128_t result = MontgomeryAlgorithm::monExp(a, e, n);
    std::cout << "Result Montgomery: " << NumberGenerator::numberToString(result) << std::endl;

    const std::vector<int> result_bin = SOSAlgorithm::monExp(a, e, n);
    std::cout << "Result SOS: ";
    BinaryHelper::printVector(result_bin);

    const std::vector<int> result_fips = FIPSAlgorithm::monExp(a, e, n, 1);
    std::cout << "Result FIPS: ";
    BinaryHelper::printVector(result_fips);

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
                    result_bin = FIPSAlgorithm::monExp(a, e, n, 1);
                    end = std::chrono::high_resolution_clock::now();
                } else if (algorithm == Algorithm::SOS) {
                    start = std::chrono::high_resolution_clock::now();
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
