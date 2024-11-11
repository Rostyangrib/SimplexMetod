//
// Created by Rostislav on 30.10.2024.
//

#include <iostream>
#include "SimplexMetodMin.h"

#include <cmath>
// find min with M metod
double SimplexMetodMin::Round(double x, double delta) {
    x = round(x * pow(10, 2)) / pow(10, 2);

    if (x > 0 && x <= delta || x < 0 && -x <= delta) {
        x = 0;
    }
    return x;
}

std::vector<std::vector<double> > SimplexMetodMin::RoundArray(std::vector<std::vector<double> > &vector) {
    // Round matrix
    for (auto &i: vector) {
        for (auto &j: i) {
            j = Round(j, 1e-3);
        }
    }
    return vector;
}

double SimplexMetodMin::FindSolve() {
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


void SimplexMetodMin::TransformationMatrix(int idx_column, int idx_str) {
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


void SimplexMetodMin::CalculateSimplexDelta(int idx_str, int idx_column) {
    double coefficient_1 = simplex_delta_[idx_column].first;
    double coefficient_2 = simplex_delta_[idx_column].second;
    for (int i = 0; i < size_; i++) {
        simplex_delta_[i].first = Round(simplex_delta_[i].first - solve_vector_[idx_str][i] * coefficient_1, 1e-1);
        simplex_delta_[i].second = Round(simplex_delta_[i].second - solve_vector_[idx_str][i] * coefficient_2, 1e-1);
    }
}

int SimplexMetodMin::CalculateMin(int idx) {
    double min = 10000000;
    int str_idx = 0;
    for (int i = 0; i < cur_base_.size(); i++) {
        if (solve_vector_[i][idx] > 0 && Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx], 1e-3) < min) {
            min = Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx], 1e-3);
            min_vector_.push_back(min);
            str_idx = i;
        }
    }
    return str_idx;
}

int SimplexMetodMin::FindMinSimplex() {
    double max = 10000000;
    int idx = 0;
    for (int i = 0; i < simplex_delta_.size(); i++) {
        if (simplex_delta_[i].first < 0) {
            max = simplex_delta_[i].first;
            idx = i;
            break;
        }
    }
    if (max == 10000000) {
        return -1;
    }
    return idx;
}

void SimplexMetodMin::CreateTableMin() {
    int tmp = count_coefficient_func_;
    for (int i = 0; i < count_limit_; i++) {
        for (int j = tmp; j < count_limit_ + count_coefficient_func_; j++) {
            if (type_ == 0) {
                solve_vector_[i][j] = -1;
                ++tmp;
                break;
            }
        }
    }
    int tmp_2 = count_coefficient_func_ + count_limit_;
    for (int i = 0; i < count_limit_; i++) {
        for (int j = tmp_2; j < count_coefficient_func_ * 2 + count_limit_ + 1; j++) {
            if (type_ == 0) {
                solve_vector_[i][j] = 1;
                ++tmp_2;
                break;
            }
        }
    }
    for (int i = 0; i < count_limit_; i++) {
        solve_vector_[i][size_ - 1] = cur_base_[i];
    }

    for (int i = 0; i < simplex_delta_.size(); i++) {
        for (int j = 0; j < count_limit_; j++) {
            simplex_delta_[i].first += solve_vector_[j][i];
        }
        simplex_delta_[i].second = -coefficient_func_[i];
        if (simplex_delta_[i].first == 1) {
            simplex_delta_[i].first = 0;
        }
        simplex_delta_[i].first *= -1;
    }
}

void SimplexMetodMin::PrintAnswer() {
    cur_base_.clear();
    for (int i = 0; i < size_; i++) {
        if (simplex_delta_[i].first == 0 && simplex_delta_[i].second == 0) {
            cur_base_.push_back(i);
        }
    }
    std::cout << std::endl << "Basic solution" << std::endl;

    for (int i = 0; i < count_limit_; i++) {
        std::cout << "x" << i + 1 << " = " << solve_vector_[i][size_ - 1] << std::endl;
    }
    if (count_coefficient_func_ > count_limit_) {
        for (int i = count_limit_; i < count_coefficient_func_; i++) {
            std::cout << "x" << i + 1 << " = " << 0  << std::endl;
        }

    }
    std::cout << "Target function value = " << trunc(simplex_delta_[size_ - 1].second) << std::endl;
}

void SimplexMetodMin::Print() {
    std::cout << "Current table:" << std::endl;
    for (int i = 0; i < count_limit_; i++) {
        for (int j = 0; j < size_; j++) {
            std::cout << solve_vector_[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "min ";
    for (int i = 0; !min_vector_.empty() && i < count_limit_; i++) {
        std::cout << min_vector_[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl << "Simplex delta:" << std::endl;
    for (int i = 0; i < size_; i++) {
        std::cout << simplex_delta_[i].first << "M " << simplex_delta_[i].second << "; ";
    }
    std::cout << std::endl << std::endl;
}

SimplexMetodMin::SimplexMetodMin(int n, int m, bool type, const std::vector<double> &coefficient_func,
                                 const std::vector<std::vector<double> > &solve_vector,
                                 const std::vector<double> &cur_base): AbstractSimplexMetod(
    n, m, type, coefficient_func, solve_vector, cur_base) {
}
