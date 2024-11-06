#include <iostream>

#include "SimplexMetodMax.h"
#include "SimplexMetodMin.h"

int main() {
    int m, n;
    bool type;
   // std::cout << "Enter count coefficients target function and count limits \n";
    std::cin >> n >> m;
  //  std::cout << "Enter type your task: 1->max, 0->min\n";
    std::cin >> type;
   // std::cout << "Enter your coefficients target function and matrix limits in format: ctf-> 1 0 4000 - the first limit ->(next line) 0 1 6000-> (next line) 1 0.66 6000->enter \n";
    if (type == 0) {
        SimplexMetodMin simplex(n, m, type);
        simplex.FindSolve();
    } else {
        SimplexMetodMax simplex(n, m, type);
        simplex.FindSolve();
    }
    return 0;
}
