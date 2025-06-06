//
// Created by Maciej on 7.05.2025.
//

#include "montgomery_algorithm.h"

#include <chrono>
#include <iostream>
#include <numeric>
#include <stdexcept>

int128_t MontgomeryAlgorithm::monExp(const int128_t a, const int128_t e, const int128_t n) {
    auto [k, r, n_prime] = prepare(n);

    // Krok 1
    const int128_t a_bar = (a * r) % n;

    // Krok 2
    int128_t x_bar = (1 * r) % n;

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


std::tuple<int, int128_t, int128_t> MontgomeryAlgorithm::prepare(const int128_t n) {
    // Obliczenie k: liczby bitów w n
    int k = 0;
    int128_t tmp = n;
    while (tmp > 0) {
        k++;
        tmp = tmp >> 1;
    }

    // Wyliczanie r = 2^k
    const int128_t r = 1 << k;

    // Obliczanie r_inv = r^(-1) mod n
    int128_t r_inv;
    try {
         r_inv = modInverse(r, n);
    } catch (const std::exception &e) {
        throw std::runtime_error(std::string("Exception: ") + e.what());
    }

    // Obliczanie n_prime ze wzoru: r * r^(-1) - n * n' = 1
    int128_t n_prime = (r * r_inv - 1) / n;

    return std::make_tuple(k, r, n_prime);
}

int128_t MontgomeryAlgorithm::monPro(const int128_t a_bar, const int128_t b_bar, const int128_t n, const int128_t n_prime, const int128_t r) {
    // Krok 1
    const int128_t t = a_bar * b_bar;

    // Krok 2
    const int128_t m = t * n_prime % r;
    const int128_t u = (t + m * n) / r;

    // Krok 3
    return (u >= n) ? (u - n) : u;
}

int128_t MontgomeryAlgorithm::gcdExtended(const int128_t a, const int b, int128_t& x, int128_t& y) {
    if (a == 0) {
        x = 0;
        y = 1;

        return b;
    }

    int128_t x1, y1;
    const int128_t gcd = gcdExtended(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

int128_t MontgomeryAlgorithm::modInverse(const int128_t r, const int128_t n) {
    int128_t x = 1, y = 1;
    const int128_t gcd = gcdExtended(r, n, x, y);

    if (gcd != 1) {
        throw std::invalid_argument("Modular inverse does not exist");
    }

    return (x % n + n) % n;
}
