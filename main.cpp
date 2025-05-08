#include <iostream>
#include <chrono>

#include "src/algorithms/cihs_algorithm.h"
#include "src/algorithms/fips_algorithm.h"
#include "src/algorithms/montgomery_algorithm.h"
#include "src/util/binary_helper.h"

int main() {
    //CIHS
    /*constexpr int a = 7, e = 10, n = 13;

    std::cout << "Result Montgomery: "  << MontgomeryAlgorithm::monExp(a, e, n) << std::endl;

    const std::vector<int> result = CIHSAlgorithm::monExp(a, e, n);
    std::cout << "Result CIHS: ";
    BinaryHelper::printVector(result);

    std::cout << std::endl;
*/
    //FIPS
    unsigned long long x = 123;
    unsigned long long y = 456;
    unsigned long long m = 1009;

    // r = 2^64 mod m
    __uint128_t r128 = (__uint128_t(1) << 64);
    unsigned long long r = (unsigned long long)(r128 % m);

    unsigned long long m_inv = 1;
    for (int i = 0; i < 6; ++i)
        m_inv *= 2 - m * m_inv;

    unsigned long long m_prime = -m_inv;

    unsigned long long x_mont = ((__uint128_t)x * r) % m;
    unsigned long long y_mont = ((__uint128_t)y * r) % m;

    auto start = std::chrono::high_resolution_clock::now();

    unsigned long long result_mont = montgomery_multiply_fips(x_mont, y_mont, m, m_prime, r);
    unsigned long long result = montgomery_multiply_fips(result_mont, 1, m, m_prime, r);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;

    std::cout << "Poprawny wynik: " << (x * y) % m << std::endl;

    std::cout << "Wynik mnożenia Montgomery (FIPS): " << result << std::endl;
    std::cout << "Czas wykonania: " << elapsed.count() / 1000 << " ms" << std::endl;

    return 0;
}