//
// Created by Maciej on 8.05.2025.
//

#include "number_generator.h"

#include <iostream>
#include <stdexcept>
#include <random>

uint128_t NumberGenerator::generate(const int bits, const bool odd) {
    if (bits <= 0 || bits >= 128) {
        throw std::invalid_argument("Bits must be in range 1-128");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint128_t> distrib(0, 1);

    uint128_t result = 0;
    for (int i = 0; i < bits; ++i) {
        result = (result << 1) | distrib(gen);

    }

    if (odd) {
        result |= 1;
    }

    return result;
}

std::string NumberGenerator::numberToString(uint128_t number) {
    if (number == 0) {
        return "0";
    }

    std::string result;
    while (number > 0) {
        result.insert(result.begin(), static_cast<char>('0' + static_cast<int>(number % 10)));
        number /= 10;
    }
    return result;
}