//
// Created by Rostislav on 30.10.2024.
//

#include "iostream"
#include <cmath>
#include "SimplexMetodMax.h"


double SimplexMetodMax::FindSolve() {
    solve_vector_ = RoundArray(solve_vector_);
    CreateTableMin();
    Print();
    for (int i = 0; ; i++) {
        min_vector_.clear();
        int idx_column = FindMinSimplex();
        if (idx_column == -1) {
            break;
        }
        int idx_str = CalculateMin(idx_column);
        cur_base_[idx_str] = idx_column;
        TransformationMatrix(idx_column, idx_str);
        solve_vector_ = RoundArray(solve_vector_);
        CalculateSimplexDelta(idx_str, idx_column);
        Print();
    }
    PrintAnswer();
    return trunc(simplex_delta_[size_ - 1].second);
}

double SimplexMetodMax::Round(double x, double delta) {
    x = round(x * pow(10, 4)) / pow(10, 4);

    if (x > 0 && x <= delta || x < 0 && x >= -delta) {
        x = 0;
    }
    return x;
}

std::vector<std::vector<double> > SimplexMetodMax::RoundArray(std::vector<std::vector<double> > &vector) {
    // Round matrix
    for (auto &i: vector) {
        for (auto &j: i) {
            j = Round(j, 1e-3);
        }
    }
    return vector;
}

SimplexMetodMax::SimplexMetodMax(int n, int m, bool type, const std::vector<double>& coefficient_func,
                                 const std::vector<std::vector<double> >& solve_vector, const std::vector<double>& cur_base): AbstractSimplexMetod(
    n, m, type, coefficient_func, solve_vector, cur_base) {}


void SimplexMetodMax::CreateTableMin() {
    int tmp = count_coefficient_func_;

    for (int i = 0; i < count_limit_; i++) {
        for (int j = tmp; j < count_limit_ + count_coefficient_func_; j++) {
            solve_vector_[i][j] = 1;
            ++tmp;
            break;
        }
    }

    for (int i = 0; i < count_limit_; i++) {
        solve_vector_[i][size_ - 1] = cur_base_[i];
    }

    for (int i = 0; i < simplex_delta_.size(); i++) {
        simplex_delta_[i].second = -coefficient_func_[i];
    }
}


void SimplexMetodMax::TransformationMatrix(int idx_column, int idx_str) {
    double coefficient = solve_vector_[idx_str][idx_column];
    for (int i = 0; i < size_; i++) {
        solve_vector_[idx_str][i] = Round(solve_vector_[idx_str][i] / coefficient, 1e-3);
    }
    for (int i = 0; i < count_limit_; i++) {
        double coefficient_1 = solve_vector_[i][idx_column];
        for (int j = 0; j < size_; j++) {
            if (i == idx_str) {
                break;
            }
            solve_vector_[i][j] = Round(solve_vector_[i][j] - solve_vector_[idx_str][j] * coefficient_1, 1e-3);
        }
    }
}

void SimplexMetodMax::CalculateSimplexDelta(int idx_str, int idx_column) {
    double coefficient_2 = simplex_delta_[idx_column].second;
    for (int i = 0; i < size_; i++) {
        simplex_delta_[i].second = Round(simplex_delta_[i].second - solve_vector_[idx_str][i] * coefficient_2, 1e-1);
    }
}

int SimplexMetodMax::FindMinSimplex() {
    double min = -10000000;
    int idx = 0;
    for (int i = 0; i < simplex_delta_.size(); i++) {
        if (simplex_delta_[i].second < 0) {
            min = simplex_delta_[i].first;
            idx = i;
            break;
        }
    }
    if (min == -10000000) {
        return -1;
    }
    return idx;
}

void SimplexMetodMax::PrintAnswer() {
    std::cout << std::endl << "Basic solution" << std::endl;

    for (int i = 0; i < count_coefficient_func_ && i < count_limit_; i++) {
        std::cout << "x" << i + 1 << " = " << solve_vector_[i][size_ - 1] << std::endl;
    }
    if (count_coefficient_func_ > count_limit_) {
        for (int i = count_limit_; i < count_coefficient_func_; i++) {
            std::cout << "x" << i + 1 << " = " << 0  << std::endl;
        }
    }
    std::cout << "Target function value = " << trunc(simplex_delta_[size_ - 1].second) << std::endl;
}


void SimplexMetodMax::Print() {
    std::cout << "Current table:" << std::endl;
    for (int i = 0; i < count_limit_; i++) {
        for (int j = 0; j < size_; j++) {
            std::cout << solve_vector_[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "Simplex delta:" << std::endl;
    for (int i = 0; i < size_; i++) {
        std::cout << simplex_delta_[i].second << "; ";
    }
    std::cout << std::endl << std::endl;
}


int SimplexMetodMax::CalculateMin(int idx) {
    double min = 10000000;
    int str_idx = 0;
    for (int i = 0; i < cur_base_.size(); i++) {
        if (solve_vector_[i][idx] > 0 && Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx], 1e-3) < min) {
            min = Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx], 1e-3);
            min_vector_.push_back(min);
            str_idx = i;
        }
    }
    cur_base_[str_idx] = idx;
    return str_idx;
}
