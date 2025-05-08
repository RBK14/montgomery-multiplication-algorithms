#ifndef FIPS_H
#define FIPS_H

#include <cstdint>

unsigned long long montgomery_multiply_fips(unsigned long long x, unsigned long long y, unsigned long long m, unsigned long long m_prime, unsigned long long r);

#endif // FIPS_H