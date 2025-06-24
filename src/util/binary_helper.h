//
// Created by Maciej on 22.04.2025.
//

#ifndef BINARY_HELPER_H
#define BINARY_HELPER_H

#include <tuple>
#include <vector>

typedef __int128 int128_t;

class BinaryHelper {
public:
    // Metoda dodawania z uwzględnieniem przeniesienia
    static std::tuple<int, int> addc(int x, int y, int carry = 0);

    // Metoda odejmowania z uwzględnieniem pożyczki
    static std::tuple<int, int> subc(int x, int y, int borrow = 0);

    // Metoda propagowania przeniesienia
    static std::vector<int> propagate(std::vector<int>& bits, int i, int carry);

    static std::vector<int> toBinaryVector(int128_t value, int length);

    static void printVector(const std::vector<int> &vector);

    static bool validate(int128_t number, const std::vector<int>& binary_vector);

    static bool validate(const std::vector<int>& a, const std::vector<int>& b);
};

#endif //BINARY_HELPER_H
