//
// Created by Maciej on 7.05.2025.
//

#ifndef MONTGOMERY_ALGORITHM_H
#define MONTGOMERY_ALGORITHM_H
#include <tuple>


typedef __int128 int128_t;

class MontgomeryAlgorithm {
public:
    static int128_t monExp(int128_t a, int128_t e, int128_t n);

    static std::tuple<int, int128_t, int128_t> prepare(int128_t n);

// private:
    static int128_t monPro(int128_t a_bar, int128_t b_bar, int128_t n, int128_t n_prime, int128_t r);

    // Metoda realizująca rozszerzony algorytm Euklidesa
    static int128_t gcdExtended(int128_t a, int128_t b, int128_t &x, int128_t &y);

    // Metoda wyliczająca odwrotność modularną
    static int128_t modInverse(int128_t r, int128_t n);
};

#endif //MONTGOMERY_ALGORITHM_H
