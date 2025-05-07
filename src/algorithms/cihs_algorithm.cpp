//
// Created by Maciej on 22.04.2025.
//

#include "cihs_algorithm.h"

#include <iostream>
#include <tuple>

#include "montgomery_algorithm.h"
#include "../util/binary_helper.h"

std::vector<int> CIHSAlgorithm::monExp(const int a, const int e, const int n, const int w) {
    auto [k, r, n_prime] = MontgomeryAlgorithm::prepare(n);

    const int s = k / w;

    const int a_bar_val = (a * r) % n;
    const std::vector<int> a_bar = BinaryHelper::toBinaryVector(a_bar_val, s);

    const int x_bar_val = (1 * r) % n;
    std::vector<int> x_bar = BinaryHelper::toBinaryVector(x_bar_val, s);

    const std::vector<int> n_bin = BinaryHelper::toBinaryVector(n, s);

    const std::vector<int> n_prime_bin= BinaryHelper::toBinaryVector(n_prime, s);

    for (int i = k - 1; i >= 0; i--) {
        x_bar = CIHS(x_bar, x_bar, n_bin, n_prime_bin, s, w);

        if ((e >> i) & 1) {
            x_bar = CIHS(x_bar, a_bar, n_bin, n_prime_bin, s, w);
        }
    }

    std::vector<int> tab(s, 0);
    tab[0] = 1;

    std::vector<int> u = CIHS(x_bar, tab, n_bin, n_prime_bin, s, w);
    return u;
}

std::vector<int> CIHSAlgorithm::CIHS(std::vector<int> a_bar, std::vector<int> b_bar, std::vector<int> n, std::vector<int> n_prime, const int s, const int w) {
    std::vector<int> t(2*s, 0);
    std::vector<int> u(2*s, 0);

    int carry, sum;

    // Krok 1
    for (int i = 0; i < s; i++) {
        carry = 0;

        for (int j = 0; j < s - i; j++) {
            const int x = t[i + j];
            const int y = a_bar[j] * b_bar[i];
            std::tie(carry, sum) = BinaryHelper::addc(x, y, carry);
            t[i + j] = sum;
        }

        std::tie(carry, sum) = BinaryHelper::addc(t[s], carry);
        t[s] = sum;
        t[s + 1] = carry;
    }

    // Krok 2
    for (int i = 0; i < s; i++) {
        const int m = (t[0] * n_prime[0]) % w;

        for (int j = 1; j < s - i; j++) {
            std::tie (carry, sum) = BinaryHelper::addc(t[j], m*n[j], carry);
            t[j - 1] = sum;
        }

        std::tie (carry, sum) = BinaryHelper::addc(t[s], carry);
        t[s - 1] = sum;
        t[s] = t[s + 1] + carry;
        t[s + 1] = 0;

        for (int j = i + 1; j < s; j++) {
            const int x = t[s - 1];
            const int y = b_bar[j] * a_bar[s - j - 1];
            std::tie(carry, sum) = BinaryHelper::addc(x, y);
            t[s-1] = sum;
            std::tie(carry, sum) = BinaryHelper::addc(t[s], carry);
            t[s] = sum;
            t[s + 1] = carry;
        }
    }

    int borrow = 0, diff;
    for (int i = 0; i < s; i++) {
        std::tie(borrow, diff) = BinaryHelper::subc(u[i], n[i], borrow);
        t[i] = diff;
    }

    std::tie(borrow, diff) = BinaryHelper::subc(u[s], borrow);
    t[s] = diff;

    if (borrow == 0) {
        return t;
    }

    return u;
}
