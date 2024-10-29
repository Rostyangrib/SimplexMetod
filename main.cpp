#include <iostream>

#include "SimplexMetodMax.h"
#include "SimplexMetodMin.h"

int main() {
    int n, m;
    bool type;
    std::cin >> n >> m >> type;
    if (type == 0) {
        SimplexMetodMin simplex(n, m, type);
        simplex.FindSolve();
    } else {
        SimplexMetodMax simplex(n, m, type);
        simplex.FindSolve();
    }
    return 0;
}
