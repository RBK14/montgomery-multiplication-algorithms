//
// Created by Maciej on 8.05.2025.
//

#ifndef NUMBER_GENERATOR_H
#define NUMBER_GENERATOR_H
#include <string>

typedef unsigned __int128 uint128_t;

class NumberGenerator {
public:
    static uint128_t generate(int bits, bool odd = false);

    static std::string numberToString(uint128_t number);
};



#endif //NUMBER_GENERATOR_H
