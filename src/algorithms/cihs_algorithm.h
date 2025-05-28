//
// Created by Maciej on 22.04.2025.
//

#ifndef CIHS_ALGORITHM_H
#define CIHS_ALGORITHM_H

#include <vector>

#include "montgomery_algorithm.h"

typedef unsigned __int128 uint128_t;

class CIHSAlgorithm {
public:
    static std::vector<int> monExp(uint128_t a, uint128_t e, uint128_t n, int w = 1);

// private:
    // Metoda realizująca algorytm CIHS
    static std::vector<int> CIHS(const std::vector<int> &a, const std::vector<int> &b, const std::vector<int> &n, const std::vector<int> &n_prime, int s, int w);
};



#endif //CIHS_ALGORITHM_H
