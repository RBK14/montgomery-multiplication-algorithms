
#include <iostream>
#include <chrono>

#include "src/algorithms/fips_algorithm.h"
#include "src/algorithms/montgomery_algorithm.h"
#include "src/algorithms/sos_algorithm.h"
#include "src/util/binary_helper.h"
#include "src/util/number_generator.h"
#include "src/util/result_parser.h"

typedef __int128 int128_t;

void launchTest(int128_t a, int128_t e, int128_t n, Algorithm algorithm);

void launchSimulation(int trails, const std::vector<int>& bits, const std::vector<Algorithm>& algorithms);

std::tuple<int128_t, int128_t, int128_t> readInputData();

int128_t stringToInt128(const std::string& str);

int main() {
    std::cout << "Select mode:\n";
    std::cout << "1. Simulation\n";
    std::cout << "2. Testing\n";
    std::cout << "Choice: ";

    int mode_choice;
    std::cin >> mode_choice;
    std::cin.ignore();

    if (mode_choice == 1) {
        std::string bit_sizes_line;
        std::cout << "Enter bit sizes (space-separated): ";
        std::getline(std::cin, bit_sizes_line);

        std::stringstream ss(bit_sizes_line);
        std::vector<int> bit_sizes;
        int bit_size;
        while (ss >> bit_size) {
            bit_sizes.push_back(bit_size);
        }

        int trials;
        std::cout << "Enter number of trials: ";
        std::cin >> trials;

        // uruchamiamy symulację z podanymi parametrami
        launchSimulation(trials, bit_sizes, {Algorithm::FIPS, Algorithm::SOS});
    }
    else if (mode_choice == 2) {
        std::cout << "Select algorithm:\n";
        std::cout << "1. FIPS\n";
        std::cout << "2. SOS\n";
        std::cout << "3. MonPro\n";
        std::cout << "Choice: ";

        int alg_choice;
        std::cin >> alg_choice;

        Algorithm selectedAlgorithm;
        if (alg_choice == 1) {
            selectedAlgorithm = Algorithm::FIPS;
        } else if (alg_choice == 2) {
            selectedAlgorithm = Algorithm::SOS;
        } else if (alg_choice == 3) {
            selectedAlgorithm = Algorithm::MONPRO;
        } else {
            std::cerr << "[ERROR] Invalid algorithm choice.\n";
            return 1;
        }

        const auto [a, e, n] = readInputData();
        launchTest(a, e, n, selectedAlgorithm);
    }
    else {
        std::cerr << "[ERROR] Invalid mode choice.\n";
        return 1;
    }

    return 0;
}

std::tuple<int128_t, int128_t, int128_t> readInputData() {
    std::string a_str, e_str, n_str;

    std::cout << "Enter a: ";
    std::cin >> a_str;
    std::cout << "Enter e: ";
    std::cin >> e_str;
    std::cout << "Enter n: ";
    std::cin >> n_str;

    int128_t a = stringToInt128(a_str);
    int128_t e = stringToInt128(e_str);
    int128_t n = stringToInt128(n_str);

    return std::make_tuple(a, e, n);
}

void launchTest(const int128_t a, const int128_t e, const int128_t n, const Algorithm algorithm) {
    if (algorithm == Algorithm::FIPS) {
        const std::vector<int> result = FIPSAlgorithm::monExp(a, e, n);
        std::cout << "Result (FIPS): ";
        BinaryHelper::printVector(result);
    } else if (algorithm == Algorithm::SOS) {
        const std::vector<int> result = SOSAlgorithm::monExp(a, e, n);
        std::cout << "Result (SOS): ";
        BinaryHelper::printVector(result);
    } else if (algorithm == Algorithm::MONPRO) {
        const int128_t result = MontgomeryAlgorithm::monExp(a, e, n);
        std::cout << "Result (MonPro): " << NumberGenerator::numberToString(result) << "\n";
    }
}

void launchSimulation(const int trails, const std::vector<int>& bits, const std::vector<Algorithm>& algorithms) {
    ResultParser parser;

    for (const auto& bit_num : bits) {
        int i = 0;
        while (i < trails) {
            try {
                const int128_t n = NumberGenerator::generate(bit_num, true);
                const int128_t a = NumberGenerator::generate(bit_num, false, n);

                std::vector<int> result_fips_bin, result_sos_bin;
                std::map<Algorithm, double> tempResults;
                constexpr int e = 2;

                for (const auto& algorithm : algorithms) {
                    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
                    std::chrono::duration<double, std::milli> duration{};

                    start = std::chrono::high_resolution_clock::now();

                    if (algorithm == Algorithm::FIPS) {
                        result_fips_bin = FIPSAlgorithm::monExp(a, e, n);
                    } else if (algorithm == Algorithm::SOS) {
                        result_sos_bin = SOSAlgorithm::monExp(a, e, n);
                    }

                    end = std::chrono::high_resolution_clock::now();
                    duration = end - start;

                    tempResults[algorithm] = duration.count();
                }

                // Sprawdzanie, czy oba algorytmu zwróciły ten sam wynik
                // if (!BinaryHelper::validate(result_fips_bin, result_sos_bin)) {
                //     throw std::runtime_error("FIPS and SOS results does not match");
                // }

                // Jeśli wszystkie algorytmy się udały – dodajemy wynik
                for (const auto& [alg, time] : tempResults) {
                    parser.addScore(time, alg);
                }

                std::cout << "[SUCCESS] Bits: " << bit_num << "; " << i + 1 << "/" << trails << std::endl;
                i++;  // iteracja zakończona sukcesem

            } catch (const std::exception& exception) {
                std::cerr << "[ERROR] " << exception.what() << " - repeating iteration..." << std::endl;
                // Nie dodajemy żadnych wyników, iteracja powtarzana
            }
        }

        parser.calculateAverageScores(bit_num);
    }

    parser.saveScores("results.txt");
}

int128_t stringToInt128(const std::string& str) {
    int128_t result = 0;
    bool isNegative = false;
    size_t i = 0;

    if (str[0] == '-') {
        isNegative = true;
        i = 1;
    }

    for (; i < str.size(); ++i) {
        result = result * 10 + (str[i] - '0');
    }

    return isNegative ? -result : result;
}
