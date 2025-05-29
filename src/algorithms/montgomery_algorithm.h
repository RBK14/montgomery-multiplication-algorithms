//
// Created by Maciej on 7.05.2025.
//

#ifndef MONTGOMERY_ALGORITHM_H
#define MONTGOMERY_ALGORITHM_H
#include <tuple>

typedef unsigned __int128 uint128_t;

class MontgomeryAlgorithm {
public:
    static uint128_t monExp(uint128_t a, uint128_t e, uint128_t n);

    static std::tuple<int, uint128_t, uint128_t> prepare(uint128_t n);

// private:
    static uint128_t monPro(uint128_t a_bar, uint128_t b_bar, uint128_t n, uint128_t n_prime, uint128_t r);

    // Metoda realizująca rozszerzony algorytm Euklidesa
    static uint128_t gcdExtended(uint128_t a, uint128_t b, uint128_t &x, uint128_t &y);

    // Metoda wyliczająca odwrotność modularną
    static uint128_t modInverse(uint128_t r, uint128_t n);
};

#endif //MONTGOMERY_ALGORITHM_H
