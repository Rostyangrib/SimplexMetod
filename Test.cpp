//
// Created by Rostislav on 10.11.2024.
//
#include <iostream>
#include <cassert>
#include "AbstractSimplexMetod.h"
#include "SimplexMetodMax.h"
#include "SimplexMetodMin.h"
#include "Test.h"

void TestSimplexMetod() {
    SimplexMetodMin test_simplex_min(2, 3, false,
        std::vector<double>{12, 16,  0, 0, 0, 0, 0, 0, 0},
        std::vector<std::vector<double>>({{2, 6, 0, 0, 0, 0, 0, 0, 0}, {5, 4, 0, 0 ,0 ,0 , 0 ,0, 0}, {2, 3, 0, 0 ,0 ,0 , 0 ,0, 0}}),
        std::vector<double>{24, 31, 18});

    SimplexMetodMax test_simplex_max(2, 3, true,
     std::vector<double>{4, 3,  0, 0, 0, 0, 0, 0, 0},
     std::vector<std::vector<double>>({{1, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 0, 0 ,0 ,0, 0, 0, 0}, {1, 0.66, 0, 0 ,0 ,0, 0, 0, 0}}),
     std::vector<double>{4000, 6000, 6000});

    assert(test_simplex_min.FindSolve() == 100);
    assert(test_simplex_max.FindSolve() == 26160);
    std::cout << "OK";
}