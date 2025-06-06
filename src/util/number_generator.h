//
// Created by Maciej on 8.05.2025.
//

#ifndef NUMBER_GENERATOR_H
#define NUMBER_GENERATOR_H
#include <optional>
#include <string>

typedef __int128 int128_t;

class NumberGenerator {
public:
    static int128_t generate(int bits, bool force_odd, const std::optional<int128_t> &maxValue = std::nullopt);

    static std::string numberToString(int128_t number);
};



#endif //NUMBER_GENERATOR_H
