//
// Created by Rostislav on 30.10.2024.
//

#ifndef SIMPLEXMETODMIN_H
#define SIMPLEXMETODMIN_H
#include "AbstractSimplexMetod.h"

class SimplexMetodMin : public AbstractSimplexMetod {
public:
    SimplexMetodMin(int n, int m, bool type, const std::vector<double>& coefficient_func,
                    const std::vector<std::vector<double> >& solve_vector, const std::vector<double>& cur_base);

    void CreateTableMin() override;

    void CalculateSimplexDelta(int idx_str, int idx_column) override;

    int FindMinSimplex() override;

    int CalculateMin(int idx) override;

    void Print() override;

    void PrintAnswer() override;

    void TransformationMatrix(int idx_column, int idx_str) override;

    double Round(double x, double delta) override;

    double FindSolve() override;

    std::vector<std::vector<double> > RoundArray(std::vector<std::vector<double> > &vector) override;
};
#endif //SIMPLEXMETODMIN_H
