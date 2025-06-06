#ifndef FIPS_ALGORITHM_H
#define FIPS_ALGORITHM_H

#include <vector>
#include <cstdint>

typedef __int128 int128_t;

namespace FIPSAlgorithm {
    std::vector<int> montgomery_multiply_fips(const std::vector<int> &x,
                                               const std::vector<int> &y,
                                               const std::vector<int> &m,
                                               const std::vector<int> &m_prime,
                                               int s, int w);

    std::vector<int> monExp(int128_t a, int128_t e, int128_t n, int w);
}

#endif // FIPS_ALGORITHM_H