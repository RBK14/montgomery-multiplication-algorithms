//
// Created by Maciej on 22.04.2025.
//

#ifndef CIHS_ALGORITHM_H
#define CIHS_ALGORITHM_H

#include <vector>

class CIHSAlgorithm {
public:
    static std::vector<int> monExp(int a, int e, int n, int w = 1);

private:
    // Metoda realizująca algorytm CIHS
    static std::vector<int> CIHS(std::vector<int> a, std::vector<int> b, std::vector<int> n, std::vector<int> n_prime, int s, int w);
};



#endif //CIHS_ALGORITHM_H
