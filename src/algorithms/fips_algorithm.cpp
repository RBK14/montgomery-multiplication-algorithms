#include "fips_algorithm.h"

#include "montgomery_algorithm.h"
#include "../util/binary_helper.h"

std::vector<int> FIPSAlgorithm::monExp(const int128_t a, const int128_t e, const int128_t n, const int w) {
    auto [k, r, n_prime] = MontgomeryAlgorithm::prepare(n);
    const int s = k / w;

    const int128_t a_bar_val = (a * r) % n;
    const std::vector<int> a_bar = BinaryHelper::toBinaryVector(a_bar_val, s);

    const int128_t x_bar_val = (1 * r) % n;
    std::vector<int> x_bar = BinaryHelper::toBinaryVector(x_bar_val, s);

    const std::vector<int> n_bin = BinaryHelper::toBinaryVector(n, s);
    const std::vector<int> n_prime_bin = BinaryHelper::toBinaryVector(n_prime, s);

    for (int i = k - 1; i >= 0; i--) {
        x_bar = multiplyFIPS(x_bar, x_bar, n_bin, n_prime_bin, s, w);

        if ((e >> i) & 1) {
            x_bar = multiplyFIPS(x_bar, a_bar, n_bin, n_prime_bin, s, w);
        }
    }

    std::vector<int> one_bin(s, 0);
    one_bin[0] = 1;

    std::vector<int> u = multiplyFIPS(x_bar, one_bin, n_bin, n_prime_bin, s, w);
    return u;
}

std::vector<int> FIPSAlgorithm::multiplyFIPS(const std::vector<int> &a, const std::vector<int> &b,
                                             const std::vector<int> &n, const std::vector<int> &n_prime,
                                             const int s, const int w) {
    std::vector<int> t(3, 0);
    std::vector<int> m(s, 0);
    std::vector<int> u(s, 0);

    int carry, sum;
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < i; j++) {
            std::tie(carry, sum) = BinaryHelper::addc(t[0], a[j] * b[i - j]);
            BinaryHelper::propagate(t, 1, carry);
            std::tie(carry, sum) = BinaryHelper::addc(sum, m[j] * n[i - j]);
            t[0] = sum;
            BinaryHelper::propagate(t, 1, carry);
        }

        std::tie(carry, sum) = BinaryHelper::addc(t[0], a[i] * b[0]);
        BinaryHelper::propagate(t, 1, carry);
        m[i] = sum * n_prime[0] % (1 << w);
        std::tie(carry, sum) = BinaryHelper::addc(sum, m[i] * n[0]);
        BinaryHelper::propagate(t, 1, carry);
        t[0] = t[1];
        t[1] = t[2];
        t[2] = 0;
    }

    for (int i = s; i < 2 * s; i++) {
        for (int j = i - s + 1; j < s; j++) {
            std::tie(carry, sum) = BinaryHelper::addc(t[0], a[j] * b[i - j]);
            BinaryHelper::propagate(t, 1, carry);
            std::tie(carry, sum) = BinaryHelper::addc(sum, m[j] * n[i - j]);
            t[0] = sum;
            BinaryHelper::propagate(t, 1, carry);
        }

        m[i - s] = t[0];
        t[0] = t[1];
        t[1] = t[2];
        t[2] = 0;
    }

    int borrow = 0, diff;
    for (int i = 0; i < s; i++) {
        std::tie(borrow, diff) = BinaryHelper::subc(m[i], n[i], borrow);
        u[i] = diff;
    }
    std::tie(borrow, diff) = BinaryHelper::subc(t[0], borrow);
    t[0] = diff;

    if (borrow == 0) {
        return u;
    }
    return m;
}
