#ifndef FIPS_ALGORITHM_H
#define FIPS_ALGORITHM_H

#include <vector>
#include <cstdint>

typedef __int128 int128_t;

class FIPSAlgorithm {
public:
    static std::vector<int> monExp(int128_t a, int128_t e, int128_t n, int w = 1);

// private:
    static std::vector<int> multiplyFIPS(const std::vector<int> &a,
                                  const std::vector<int> &b,
                                  const std::vector<int> &n,
                                  const std::vector<int> &n_prime,
                                  int s, int w);
};

#endif // FIPS_ALGORITHM_H