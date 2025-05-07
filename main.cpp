
#include <iostream>
#include <ostream>
#include <vector>


#include "src/algorithms/cihs_algorithm.h"
#include "src/algorithms/montgomery_algorithm.h"
#include "src/util/binary_helper.h"

int main() {
    std::cout << MontgomeryAlgorithm::monExp(7, 10, 13) << std::endl;

    std::vector<int> result = CIHSAlgorithm::monExp(7, 10, 13);
    BinaryHelper::printVector(result);

    return 0;
}
