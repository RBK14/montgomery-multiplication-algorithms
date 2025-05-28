//
// Created by Maciej on 28.05.2025.
//

#include "result_parser.h"

#include <iostream>
#include <fstream>
#include <numeric>

void ResultParser::addScore(const double score, const Algorithm algorithm) {
    if (algorithm == Algorithm::FIPS) fipsScores.push_back(score);
    if (algorithm == Algorithm::CIHS) cihsScores.push_back(score);
}

void ResultParser::saveScores(const std::string& filename) const {
    std::ofstream file("../files/results/" + filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Cannot open the file: " << filename << std::endl;
        return;
    }

    file << "n;fips;cihs\n";

    for (const auto& [size, scores] : averageScores) {
        file << size << ";"
             << scores.at(Algorithm::FIPS) << ";"
             << scores.at(Algorithm::CIHS) << ";\n";
    }

    file.close();

    std::cout << "[SUCCESS] Results has been saved to file " << filename << std::endl;
}

void ResultParser::calculateAverageScores(const int bits) {
    std::map <Algorithm, double> scores;

    scores[Algorithm::FIPS] = calculateAverageScore(fipsScores);
    scores[Algorithm::CIHS] = calculateAverageScore(cihsScores);


    averageScores.insert({bits, scores});

}

double ResultParser::calculateAverageScore(const std::vector<double>& scores) {
    if (scores.empty()) return 0.0;

    const double sum = std::accumulate(scores.begin(), scores.end(), 0.0);
    return sum / static_cast<double>(scores.size());
}