//
// Created by Rostislav on 30.10.2024.
//

#ifndef SIMPLEXMETODMAX_H
#define SIMPLEXMETODMAX_H
#include "AbstractSimplexMetod.h"

class SimplexMetodMax : public AbstractSimplexMetod {
public:
    SimplexMetodMax(int n, int m, bool type, const std::vector<double>& coefficient_func,
                    const std::vector<std::vector<double> >& solve_vector, const std::vector<double>& cur_base);

    void CalculateSimplexDelta(int idx_str, int idx_column) override;

    int FindMinSimplex() override;

    int CalculateMin(int idx) override;

    void Print() override;

    void PrintAnswer() override;

    void TransformationMatrix(int idx_column, int idx_str) override;

    double FindSolve() override;

    void CreateTableMin() override;

    double Round(double x, double delta) override;

    std::vector<std::vector<double> > RoundArray(std::vector<std::vector<double> > &vector) override;
};

#endif //SIMPLEXMETODMAX_H
