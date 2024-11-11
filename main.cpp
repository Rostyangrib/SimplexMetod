#include <iostream>

#include "Test.h"
#include "SimplexMetodMax.h"
#include "SimplexMetodMin.h"

int main() {
    int m, n;
    bool type;

    //std::cout << "Enter count coefficients target function and count limits \n";
    std::cin >> n >> m;
    //std::cout << "Enter type your task: 1->max, 0->min\n";
    std::cin >> type;
    // std::cout << "Enter your coefficients target function and matrix limits in format: ctf-> 1 0 4000 - the first limit ->(next line) 0 1 6000-> (next line) 1 0.66 6000->enter \n";
    int size = m + m + n + 1;
    std::vector<double> coefficient_func(size);
    std::vector solve_vector(m, std::vector<double>(size, 0));
    std::vector<double> cur_base(m);

    for (int i = 0; i < n; i++) {
        std::cin >> coefficient_func[i];
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> solve_vector[i][j];
        }
        std::cin >> cur_base[i];
    }
    if (type == 0) {
        SimplexMetodMin simplex(n, m, type, coefficient_func, solve_vector, cur_base);
        simplex.FindSolve();
    } else {
        SimplexMetodMax simplex(n, m, type, coefficient_func, solve_vector, cur_base);
        simplex.FindSolve();
    }

   // TestSimplexMetod();
    return 0;
}
