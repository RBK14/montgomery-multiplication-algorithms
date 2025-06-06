//
// Created by Maciej on 29.05.2025.
//

#include "sos_algorithm.h"

#include <iostream>

#include "montgomery_algorithm.h"
#include "../util/binary_helper.h"

std::vector<int> SOSAlgorithm::monExp(const int128_t a, const int128_t e, const int128_t n, const int w) {
    auto [k, r, n_prime] = MontgomeryAlgorithm::prepare(n);
    const int s = k / w;

    const int128_t a_bar_val = (a * r) % n;
    const std::vector<int> a_bar = BinaryHelper::toBinaryVector(a_bar_val, s);

    const int128_t x_bar_val = (1 * r) % n;
    std::vector<int> x_bar = BinaryHelper::toBinaryVector(x_bar_val, s);

    const std::vector<int> n_bin = BinaryHelper::toBinaryVector(n, s);

    const std::vector<int> n_prime_bin= BinaryHelper::toBinaryVector(n_prime, s);

    for (int i = k - 1; i >= 0; i--) {
        x_bar = SOS(x_bar, x_bar, n_bin, n_prime_bin, s, w);

        if ((e >> i) & 1) {
            x_bar = SOS(x_bar, a_bar, n_bin, n_prime_bin, s, w);
        }
    }

    std::vector<int> one_bin(s, 0);
    one_bin[0] = 1;

    std::vector<int> u = SOS(x_bar, one_bin, n_bin, n_prime_bin, s, w);
    return u;
}

std::vector<int> SOSAlgorithm::SOS(const std::vector<int> &a, const std::vector<int> &b, const std::vector<int> &n, const std::vector<int> &n_prime, int s, int w) {
    std::vector<int> t(2 * s + 1, 0);
    std::vector<int> u(2 * s + 1, 0);

    int carry, sum;
    for (int i = 0; i < s; i++) {
        carry = 0;
        for (int j = 0; j < s; j++) {
            std::tie(carry, sum) = BinaryHelper::addc(t[i + j], a[j] * b[i], carry);
            t[i + j] = sum;
        }

        t[i + s] = carry;
    }

    for (int i = 0; i < s; i++) {
        carry = 0;
        const int m = t[i] * n_prime[0] % (1 << w);
        for (int j = 0; j < s; j++) {
            std::tie(carry, sum) = BinaryHelper::addc(t[i + j], m * n[j], carry);
            t[i + j] = sum;
        }
        t = BinaryHelper::propagate(t, i + s, carry);
    }

    for (int j = 0; j < s + 1; j++) {
        u[j] = t[j + s];
    }

    int borrow = 0, diff;
    for (int i = 0; i < s; i++) {
        std::tie(borrow, diff) = BinaryHelper::subc(u[i], n[i], borrow);
        t[i] = diff;
    }
    std::tie(borrow, diff) = BinaryHelper::subc(u[s], borrow);
    t[s] = diff;

    if (borrow == 0) {
        return {t.begin(), t.begin() + s};
    }

    return  {u.begin(), u.begin() + s};
}
