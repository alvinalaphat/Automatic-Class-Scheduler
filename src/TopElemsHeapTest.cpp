
#include "TopElemsHeap.h"
#include <iostream>

int main() {

    TopElemsHeap<int> top10(10);

    std::vector<int> elems = {
        74, 49, 27, 60, 70, 64, 8, 41, 75, 56, 4, 4,
        28, 96, 77, 55, 11, 64, 1, 68, 48, 15, 75,
        47, 51, 51, 20, 82, 73, 49
    };

    for (int i: elems) {
        top10.push(i);

        for (int elem: top10.getElements()) {
            std::cout << elem << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}