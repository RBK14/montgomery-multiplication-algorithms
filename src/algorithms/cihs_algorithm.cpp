//
// Created by Maciej on 22.04.2025.
//

#include "cihs_algorithm.h"

#include <iostream>
#include <tuple>

#include "../util/binary_helper.h"

int* CIHSAlgorithm::cihsBinary(const int a[], const int b[], const int n[], const int np[], const int s, const int w) {
    const auto t = new int[2 * s]{};
    const auto u = new int[2 * s]{};

    int carry, sum;

    // Krok 1:
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

    for (int i = 0; i < s; i++) {
        const int x = t[i];
        std::cout << x << std::endl;
    }

    // Krok 2:
    for (int i = 0; i < s; i++) {
        const int m = (t[0] * np[0]) % w;

        for (int j = 0; j < s - i; j++) {
            std::tie (carry, sum) = BinaryHelper::addc(t[j], m*n[j], carry);
            t[j - 1] = sum;
        }

        std::tie (carry, sum) = BinaryHelper::addc(t[s], carry);
        t[s - 1] = sum;
        t[s] = t[s + 1] + carry;
        t[s + 1] = 0;

        for (int j = i + 1; j < s; j++) {
            const int x = t[s - 1];
            const int y = b[j] * a[s - j - 1];
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
        delete[] u;
        return t;
    }

    delete[] t;
    return u;
}
