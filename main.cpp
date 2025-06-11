
#include <iostream>
#include <chrono>

#include "src/algorithms/fips_algorithm.h"
#include "src/algorithms/montgomery_algorithm.h"
#include "src/algorithms/sos_algorithm.h"
#include "src/util/binary_helper.h"
#include "src/util/number_generator.h"
#include "src/util/result_parser.h"


typedef __int128 int128_t;

void launchTest();



void launchSimulation(int trails, const std::vector<int>& bits, const std::vector<Algorithm>& algorithms);

int main() {
    launchSimulation(200, {2, 4, 8, 16, 32, 64, 127}, {Algorithm::FIPS, Algorithm::SOS});

    // constexpr int128_t a = 7, e = 10, n = 13;
    //
    // const int128_t result = MontgomeryAlgorithm::monExp(a, e, n);
    // std::cout << "Result Montgomery: " << NumberGenerator::numberToString(result) << std::endl;
    //
    // const std::vector<int> result_sos = SOSAlgorithm::monExp(a, e, n);
    // std::cout << "Result SOS: ";
    // BinaryHelper::printVector(result_sos);
    //
    // const std::vector<int> result_fips = FIPSAlgorithm::monExp(a, e, n);
    // std::cout << "Result FIPS: ";
    // BinaryHelper::printVector(result_fips);

    return 0;
}

void launchSimulation(const int trails, const std::vector<int>& bits, const std::vector<Algorithm>& algorithms) {
    ResultParser parser;

    for (const auto& bit_num : bits) {
        for (int i = 0; i < trails; i++) {

            const int128_t n = NumberGenerator::generate(bit_num, true);
            const int128_t a = NumberGenerator::generate(bit_num, false, n);

            std::vector<int> result_bin;
            for (const auto& algorithm : algorithms) {
                constexpr int e = 2;

                std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
                std::chrono::duration<double, std::milli> duration{};

                if (algorithm == Algorithm::FIPS) {
                    start = std::chrono::high_resolution_clock::now();
                    try {
                        result_bin = FIPSAlgorithm::monExp(a, e, n);
                    } catch (const std::exception &exception) {
                        std::cerr << "[ERROR]: " << exception.what() << std::endl;
                        break;
                    }
                    end = std::chrono::high_resolution_clock::now();
                    duration = end - start;
                    parser.addScore(duration.count(), algorithm);
                } else if (algorithm == Algorithm::SOS) {
                    start = std::chrono::system_clock::now();
                    try {
                        result_bin = SOSAlgorithm::monExp(a, e, n);
                    } catch (const std::exception &exception) {
                        std::cerr << "[ERROR]: " << exception.what() << std::endl;
                        break;
                    }
                    end = std::chrono::high_resolution_clock::now();
                    duration = end - start;
                    parser.addScore(duration.count(), algorithm);
                }
            }
            std::cout << "Bits: " << bit_num << "; " << i + 1 << "/" << trails << std::endl;
        }

        parser.calculateAverageScores(bit_num);
    }

    parser.saveScores("results.txt");
}


