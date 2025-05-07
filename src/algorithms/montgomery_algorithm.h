//
// Created by Maciej on 7.05.2025.
//

#ifndef MONTGOMERY_ALGORITHM_H
#define MONTGOMERY_ALGORITHM_H
#include <tuple>

class MontgomeryAlgorithm {
public:
    static int monExp(int a, int e, int n);

    static std::tuple<int, int, int> prepare(int n);

private:
    static int monPro(int a_bar, int b_bar, int n, int n_prime, int r);

    // Metoda realizująca rozszerzony algorytm Euklidesa
    static int gcdExtended(int a, int b, int &x, int &y);

    // Metoda wyliczająca największy wspólny dzielnik
    static int findGCD(int a, int b);

    // Metoda wyliczająca odwrotność modularną
    static int modInverse(int a, int n);
};



#endif //MONTGOMERY_ALGORITHM_H
