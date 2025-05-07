
#include <iostream>
#include <ostream>
#include <vector>


#include "src/algorithms/cihs_algorithm.h"
#include "src/algorithms/montgomery_algorithm.h"
#include "src/util/binary_helper.h"

int main() {
    constexpr int a = 7, e = 10, n = 13;

    std::cout << "Result Montgomery: "  << MontgomeryAlgorithm::monExp(a, e, n) << std::endl;

    const std::vector<int> result = CIHSAlgorithm::monExp(a, e, n);
    std::cout << "Result CIHS: ";
    BinaryHelper::printVector(result);

    return 0;
}
