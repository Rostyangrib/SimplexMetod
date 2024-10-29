//
// Created by Rostislav on 30.10.2024.
//

#ifndef SIMPLEXMETODMAX_H
#define SIMPLEXMETODMAX_H
#include "AbstractSimplexMetod.h"
class SimplexMetodMax : public AbstractSimplexMetod {
public:
    SimplexMetodMax(int n, int m, bool type) : AbstractSimplexMetod(n, m, type) {
    }

    void CalculateSimplexDelta(int idx_str, int idx_column) override;

    int FindMinSimplex() override;

    int CalculateMin(int idx) override;

    void Print() override;

    void PrintAnswer() override;

    void TransformationMatrix(int idx_column, int idx_str) override;

    void FindSolve() override;

    void CreateTableMin() override;
};

#endif //SIMPLEXMETODMAX_H
