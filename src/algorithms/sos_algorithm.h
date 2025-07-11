//
// Created by Maciej on 29.05.2025.
//

#ifndef SOS_ALGORITHM_H
#define SOS_ALGORITHM_H
#include <vector>

typedef __int128 int128_t;

class SOSAlgorithm {
public:
    static std::vector<int> monExp(int128_t a, int128_t e, int128_t n, int w = 1);

// private:
    // Metoda realizująca algorytm SOS
    static std::vector<int> multiplySOS(const std::vector<int> &a, const std::vector<int> &b, const std::vector<int> &n, const std::vector<int> &n_prime, int s, int w);
};

#endif //SOS_ALGORITHM_H
