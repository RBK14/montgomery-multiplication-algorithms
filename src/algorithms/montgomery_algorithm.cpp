//
// Created by Maciej on 7.05.2025.
//

#include "montgomery_algorithm.h"

#include <chrono>
#include <iostream>
#include <numeric>
#include <stdexcept>

uint128_t MontgomeryAlgorithm::monExp(const uint128_t a, const uint128_t e, const uint128_t n) {
    auto [k, r, n_prime] = prepare(n);

    // Krok 1
    const uint128_t a_bar = (a * r) % n;

    // Krok 2
    uint128_t x_bar = (1 * r) % n;

    // Krok 3
    for (int i  = k - 1; i >= 0; i--) {
        x_bar = monPro(x_bar, x_bar, n, n_prime, r);

        // Jeśli e[i] wynosi 1 to wykonujemy operacje MonPro
        if ((e >> i) & 1) {
            x_bar = monPro(x_bar, a_bar, n, n_prime, r);
        }
    }

    // Krok 4
    return monPro(x_bar, 1, n, n_prime, r);
}


std::tuple<int, uint128_t, uint128_t> MontgomeryAlgorithm::prepare(const uint128_t n) {
    // Obliczenie k: liczby bitów w n
    int k = 0;
    uint128_t tmp = n;
    while (tmp > 0) {
        k++;
        tmp = tmp >> 1;
    }

    // Wyliczanie r = 2^k
    const uint128_t r = 1 << k;

    // Obliczanie r_inv = r^(-1) mod n
    uint128_t r_inv;
    try {
         r_inv = modInverse(r, n);
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("Exception: ") + e.what());
    }

    // Obliczanie n_prime ze wzoru: r * r^(-1) - n * n' = 1
    uint128_t n_prime = (r * r_inv - 1) / n;

    return std::make_tuple(k, r, n_prime);
}

uint128_t MontgomeryAlgorithm::monPro(const uint128_t a_bar, const uint128_t b_bar, const uint128_t n, const uint128_t n_prime, const uint128_t r) {
    // Krok 1
    const uint128_t t = a_bar * b_bar;

    // Krok 2
    const uint128_t m = t * n_prime % r;
    const uint128_t u = (t + m * n) / r;

    // Krok 3
    return (u >= n) ? (u - n) : u;
}

uint128_t MontgomeryAlgorithm::gcdExtended(const uint128_t a, const uint128_t b, uint128_t& x, uint128_t& y) {
    if (a == 0) {
        x = 0;
        y = 1;

        return b;
    }

    uint128_t x1, y1;
    const uint128_t gcd = gcdExtended(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

uint128_t MontgomeryAlgorithm::modInverse(const uint128_t r, const uint128_t n) {
    uint128_t x = 1, y = 1;
    const uint128_t gcd = gcdExtended(r, n, x, y);

    if (gcd != 1) {
        throw std::invalid_argument("Modular inverse does not exist");
    }

    return (x % n + n) % n;
}
