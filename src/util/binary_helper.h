//
// Created by Maciej on 22.04.2025.
//

#ifndef BINARY_HELPER_H
#define BINARY_HELPER_H

#include <tuple>
#include <vector>

typedef unsigned __int128 uint128_t;

class BinaryHelper {
public:
    // Metoda dodawania z uwzględnieniem przeniesienia
    static std::tuple<int, int> addc(int x, int y, int carry = 0);

    // Metoda odejmowania z uwzględnieniem pożyczki
    static std::tuple<int, int> subc(int x, int y, int borrow = 0);

    // Metoda propagowania przeniesienia
    static std::vector<int> propagate(std::vector<int>& bits, int i, int carry);

    static std::vector<int> toBinaryVector(uint128_t value, int length);

    static void printVector(const std::vector<int> &vector);

    static bool validate(uint128_t number, const std::vector<int>& binary_vector);

};

#endif //BINARY_HELPER_H
