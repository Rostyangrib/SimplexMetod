//
// Created by Rostislav on 30.10.2024.
//

#ifndef ABSTRACTSIMPLEXMETOD_H
#define ABSTRACTSIMPLEXMETOD_H
#include <vector>

class AbstractSimplexMetod {
public:
    virtual ~AbstractSimplexMetod() = default;

    double delta_;
    bool type_;
    int count_coefficient_func_;
    int count_limit_;
    int size_;
    std::vector<std::vector<double> > A_;
    std::vector<int> b_;
    std::vector<double> coefficient_func_;
    std::vector<std::vector<double> > solve_vector_;
    std::vector<std::pair<double, double> > simplex_delta_;
    std::vector<double> min_vector_;
    std::vector<int> cur_base_;

    AbstractSimplexMetod(int n, int m, bool type) : size_(n + m + m + 1)
      , delta_(1e-3)
      , type_(type)
      , count_coefficient_func_(n)
      , count_limit_(m)
      , coefficient_func_(size_, 0)
      , A_(m, std::vector<double>(n))
      , b_(m, 0)
      , solve_vector_(m, std::vector<double>(m + m + n + 1, 0))
      , simplex_delta_(m + m + n + 1, std::pair<int, int>(0, 0))
      , cur_base_(m) {}

    virtual void TransformationMatrix(int idx_column, int idx_str) = 0;

    virtual void FindSolve() = 0;

    virtual void CreateTableMin() = 0;

    virtual void CalculateSimplexDelta(int idx_str, int idx_column) = 0;

    virtual int FindMinSimplex() = 0;

    virtual int CalculateMin(int idx) = 0;

    virtual void Print() = 0;

    virtual void PrintAnswer() = 0;

    virtual double Round(double x) = 0;

    virtual std::vector<std::vector<double>> RoundArray(std::vector<std::vector<double>> &vector) = 0;
};
#endif //ABSTRACTSIMPLEXMETOD_H
