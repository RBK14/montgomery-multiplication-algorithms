#include "fips_algorithm.h"
#include "montgomery_algorithm.h"
#include "../util/binary_helper.h"

namespace FIPSAlgorithm {

std::vector<int> montgomery_multiply_fips(const std::vector<int> &a, const std::vector<int> &b,
                                           const std::vector<int> &n, const std::vector<int> &n_prime,
                                           int s, int w) {
    const int W = 1 << w;
    std::vector<int> t(2 * s + 2, 0);
    std::vector<int> u(s + 1, 0);

    for (int i = 0; i < s; ++i) {
        int carry = 0, sum = 0;

        for (int j = 0; j < s; ++j) {
            std::tie(carry, sum) = BinaryHelper::addc(t[i + j], a[j] * b[i], carry);
            t[i + j] = sum;
        }
        t[i + s] = carry;

        int m_i = (t[i] * n_prime[0]) % W;

        carry = 0;
        for (int j = 0; j < s; ++j) {
            std::tie(carry, sum) = BinaryHelper::addc(t[i + j], m_i * n[j], carry);
            t[i + j] = sum;
        }
        t[i + s] += carry;
    }

    for (int j = 0; j < s + 1; ++j) {
        u[j] = t[j + s];
    }

    bool needsReduction = false;
    for (int i = s - 1; i >= 0; --i) {
        if (u[i] > n[i]) {
            needsReduction = true;
            break;
        } else if (u[i] < n[i]) {
            break;
        }
    }

    if (needsReduction) {
        std::vector<int> result(s);
        int borrow = 0, diff;
        for (int i = 0; i < s; ++i) {
            std::tie(borrow, diff) = BinaryHelper::subc(u[i], n[i], borrow);
            result[i] = diff;
        }
        return result;
    }

    return {u.begin(), u.begin() + s};
}


std::vector<int> monExp(const int128_t a, const int128_t e, const int128_t n, const int w) {
    auto [k, r, n_prime] = MontgomeryAlgorithm::prepare(n);
    const int s = k / w;

    const int128_t a_bar_val = (a * r) % n;
    const std::vector<int> a_bar = BinaryHelper::toBinaryVector(a_bar_val, s);

    const int128_t x_bar_val = (1 * r) % n;
    std::vector<int> x_bar = BinaryHelper::toBinaryVector(x_bar_val, s);

    const std::vector<int> n_bin = BinaryHelper::toBinaryVector(n, s);
    const std::vector<int> n_prime_bin = BinaryHelper::toBinaryVector(n_prime, s);

    for (int i = k - 1; i >= 0; i--) {
        x_bar = montgomery_multiply_fips(x_bar, x_bar, n_bin, n_prime_bin, s, w);

        if ((e >> i) & 1) {
            x_bar = montgomery_multiply_fips(x_bar, a_bar, n_bin, n_prime_bin, s, w);
        }
    }

    std::vector<int> one_bin(s, 0);
    one_bin[0] = 1;

    std::vector<int> u = montgomery_multiply_fips(x_bar, one_bin, n_bin, n_prime_bin, s, w);
    return u;
}

} // namespace FIPSAlgorithm