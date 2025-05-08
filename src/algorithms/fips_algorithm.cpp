#include "fips_algorithm.h"

unsigned long long montgomery_multiply_fips(unsigned long long x, unsigned long long y, unsigned long long m, unsigned long long m_prime, unsigned long long /* r */) {
    __uint128_t t = 0;

    for (int i = 0; i < 64; ++i) {
        unsigned long long xi = (x >> i) & 1;
        __uint128_t tmp = t + (__uint128_t)(xi) * y;
        unsigned long long u = ((unsigned long long)tmp * m_prime) & 1;
        t = (tmp + (__uint128_t)u * m) >> 1;
    }

    if (t >= m)
        t -= m;

    return (unsigned long long)t;
}