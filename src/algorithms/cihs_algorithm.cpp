//
// Created by Maciej on 22.04.2025.
//

#include "cihs_algorithm.h"

#include <iostream>
#include <tuple>

#include "montgomery_algorithm.h"
#include "../util/binary_helper.h"

std::vector<int> CIHSAlgorithm::monExp(const uint128_t a, const uint128_t e, const uint128_t n, const int w) {
    auto [k, r, n_prime] = MontgomeryAlgorithm::prepare(n);

    const int s = k / w;

    const uint128_t a_bar_val = (a * r) % n;
    const std::vector<int> a_bar = BinaryHelper::toBinaryVector(a_bar_val, s);

    const uint128_t x_bar_val = (1 * r) % n;
    std::vector<int> x_bar = BinaryHelper::toBinaryVector(x_bar_val, s);

    const std::vector<int> n_bin = BinaryHelper::toBinaryVector(n, s);

    const std::vector<int> n_prime_bin= BinaryHelper::toBinaryVector(n_prime, s);

    for (int i = k - 1; i >= 0; i--) {
        x_bar = CIHS(x_bar, x_bar, n_bin, n_prime_bin, s, w);

        if ((e >> i) & 1) {
            x_bar = CIHS(x_bar, a_bar, n_bin, n_prime_bin, s, w);
        }
    }

    std::vector<int> one_bin(s, 0);
    one_bin[0] = 1;

    std::vector<int> u = CIHS(x_bar, one_bin, n_bin, n_prime_bin, s, w);
    return u;
}

std::vector<int> CIHSAlgorithm::CIHS(const std::vector<int> &a, const std::vector<int> &b, const std::vector<int> &n, const std::vector<int> &n_prime, const int s, const int w) {
    std::vector<int> t(s + 2, 0);
    std::vector<int> u(s + 2, 0);

    int carry, sum;

    // Krok 1
    for (int i = 0; i < s; i++) {
        carry = 0;
        for (int j = 0; j < s - i; j++) {
            const int x = t[i + j];
            const int y = a[j] * b[i];
            std::tie(carry, sum) = BinaryHelper::addc(x, y, carry);
            t[i + j] = sum;
        }

        std::tie(carry, sum) = BinaryHelper::addc(t[s], carry);
        t[s] = sum;
        t[s + 1] = carry;
    }

    // Krok 2
    for (int i = 0; i < s; i++) {
        const int m = t[0] * n_prime[0] % (1 << w);
        std::tie(carry, sum) = BinaryHelper::addc(t[0], m * n[0]);

        for (int j = 1; j < s; j++) {
            std::tie (carry, sum) = BinaryHelper::addc(t[j], m * n[j], carry);
            t[j - 1] = sum;
        }

        std::tie (carry, sum) = BinaryHelper::addc(t[s], carry);
        t[s - 1] = sum;
        t[s] = t[s + 1] + carry;
        t[s + 1] = 0;

        for (int j = i + 1; j < s; j++) {
            const int x = t[s - 1];
            const int y = b[j] * a[s - j + i];
            std::tie(carry, sum) = BinaryHelper::addc(x, y);
            t[s - 1] = sum;
            std::tie(carry, sum) = BinaryHelper::addc(t[s], carry);
            t[s] = sum;
            t[s + 1] = carry;
        }
    }

    // Krok 3
    int borrow = 0, diff;
    for (int i = 0; i < s; i++) {
        std::tie(borrow, diff) = BinaryHelper::subc(t[i], n[i], borrow);
        u[i] = diff;
        std::tie(borrow, diff) = BinaryHelper::subc(t[s], borrow);
        u[s] = diff;
    }

    if (borrow == 0) {
        return {u.begin(), u.begin() + s};
    }

    return {t.begin(), t.begin() + s};
}
