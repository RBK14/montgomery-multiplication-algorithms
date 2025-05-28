//
// Created by Maciej on 28.05.2025.
//

#ifndef RESULT_PARSER_H
#define RESULT_PARSER_H

#include <vector>
#include <map>
#include <string>

enum class Algorithm {
    FIPS,
    CIHS
};

class ResultParser {
    std::vector<double> fipsScores;
    std::vector<double> cihsScores;
    std::map<int, std::map<Algorithm, double>> averageScores;

public:
    void addScore(double score, Algorithm algorithm);
    void saveScores(const std::string& filename) const;
    void calculateAverageScores(int bits);

private:
    static double calculateAverageScore(const std::vector<double>& scores);
};

#endif //RESULT_PARSER_H
