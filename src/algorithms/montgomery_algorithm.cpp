//
// Created by Maciej on 7.05.2025.
//

#include "montgomery_algorithm.h"

#include <iostream>
#include <stdexcept>

int MontgomeryAlgorithm::monExp(const int a, const int e, const int n) {
    auto [k, r, n_prime] = prepare(n);

    // Krok 1
    const int a_bar = (a * r) % n;

    // Krok 2
    int x_bar = (1 * r) % n;

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

std::tuple<int, int, int> MontgomeryAlgorithm::prepare(int n) {
    // Obliczenie k: liczby bitów w n
    int k = 0, tmp = n;
    while (tmp > 0) {
        k++;
        tmp = tmp >> 1;
    }

    // Wyliczanie r = 2^k
    const int r = 1 << k;

    // Obliczanie r_inv = r^(-1) mod n
    const int r_inv = modInverse(r, n);
    if (r_inv == -1) {
        throw std::invalid_argument("Modular inverse does not exist");
    }

    // Obliczanie n_prime ze wzoru: r * r^(-1) - n * n' = 1
    int n_prime = (r * r_inv + 1) / n;

    std::cout << k << " " << r << " " << n_prime << std::endl;
    return std::make_tuple(k, r, n_prime);
}

int MontgomeryAlgorithm::monPro(const int a_bar, const int b_bar, const int n, const int n_prime, const int r) {
    // Krok 1
    const int t = a_bar * b_bar;

    // Krok 2
    const int m = t * n_prime % r;
    const int u = (t + m * n) / r;

    // Krok 3
    return (u >= n) ? (u - n) : u;
}

int MontgomeryAlgorithm::gcdExtended(const int a, const int b, int& x, int& y) {
    if (a == 0) {
        x = 0;
        y = 1;

        return b;
    }

    int x1, y1;
    const int gcd = gcdExtended(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

int MontgomeryAlgorithm::findGCD(const int a, const int b) {
    int x = 1, y = 1;

    return gcdExtended(a, b, x, y);
}

int MontgomeryAlgorithm::modInverse(const int a, const int n) {
    if (const int gcd = findGCD(a, n); gcd > 1) {
        // Odwrotność modularna nie istnieje
        return -1;
    }

    for (int x = 1; x < n; x++) {
        if (((a % n) * (x % n)) % n == 1) {
            return x;
        }
    }

    // Błąd przy wyliczaniu odwrotności
    return -1;
}
