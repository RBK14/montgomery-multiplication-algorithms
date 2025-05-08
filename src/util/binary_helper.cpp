//
// Created by Maciej on 22.04.2025.
//

#include "binary_helper.h"

#include <iostream>
#include <vector>

std::tuple<int, int> BinaryHelper::addc(const int x, const int y, int carry) {
    int sum = x + y + carry;

    if (sum >= 2) {
        carry = 1;
        sum = sum % 2;
    } else {
        carry = 0;
    }

    return std::make_tuple(carry, sum);
}

std::tuple<int, int> BinaryHelper::subc(const int x, const int y, int borrow) {
    int diff = x - y - borrow;

    if (diff < 0) {
        borrow = 1;
        diff += 2;
    } else {
        borrow = 0;
    }

    return std::make_tuple(borrow, diff);
}

std::vector<int> BinaryHelper::propagate(std::vector<int>& bits, int i, int carry) {
    while (carry > 0 && i < bits.size()) {
        // Dodanie przeniesienia i obliczenie nowego
        std::tie(carry, bits[i]) = BinaryHelper::addc(bits[i], carry);
        i++;
    }

    return bits;
}

std::vector<int> BinaryHelper::toBinaryVector(const int value, const int length) {
    std::vector<int> bits;
    for (int i = 0; i < length; ++i) {
        bits.push_back((value >> i) & 1);
    }
    return bits;
}

void BinaryHelper::printVector(std::vector<int> vector) {
    for (int i = vector.size() - 1; i >= 0; i--) {
        std::cout << vector[i];
    }
    std::cout << std::endl;
}

