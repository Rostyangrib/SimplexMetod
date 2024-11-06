//
// Created by Rostislav on 30.10.2024.
//

#include <iostream>
#include "SimplexMetodMin.h"

#include <cmath>
// find min with M metod
double SimplexMetodMin::Round(double x) {
    double delta = 1e-3;
    round(x * pow(10, 2)) / pow(10, 2);

    if (x > 0 && x <= delta || x < 0 && x >= -delta) {
        x = 0;
    }
    return x;
}

void SimplexMetodMin::FindSolve() {
    CreateTableMin();
    Print();
    for (int i = 0; ; i++) {
        min_vector_.clear();
        int idx_column = FindMinSimplex();
        if (idx_column == -1) {
            break;
        }
        int idx_str = CalculateMin(idx_column);
        TransformationMatrix(idx_column, idx_str);
        CalculateSimplexDelta(idx_str, idx_column);
        Print();
    }
    PrintAnswer();
}

std::vector<std::vector<double>> SimplexMetodMin::RoundArray(std::vector<std::vector<double>> &vector) {
    // Round matrix
    for (const auto& i : vector) {
        for (auto j : i) {
            Round(j);
        }
    }
    return vector;
}

void SimplexMetodMin::TransformationMatrix(int idx_column, int idx_str) {
    double coefficient = solve_vector_[idx_str][idx_column];
    for (int i = 0; i < size_; i++) {
        solve_vector_[idx_str][i] =  Round(solve_vector_[idx_str][i] / coefficient);
    }
    for (int i = 0; i < count_limit_; i++) {
        double coefficient_1 = solve_vector_[i][idx_column];
        for (int j = 0; j < size_; j++) {
            if (i == idx_str) {
                break;
            }
            solve_vector_[i][j] = Round(solve_vector_[i][j] - solve_vector_[idx_str][j] * coefficient_1);
        }
    }
}



void SimplexMetodMin::CalculateSimplexDelta(int idx_str, int idx_column) {
    double coefficient_1 = simplex_delta_[idx_column].first;
    double coefficient_2 = simplex_delta_[idx_column].second;
    for (int i = 0; i < size_; i++) {
        simplex_delta_[i].first = Round(simplex_delta_[i].first - solve_vector_[idx_str][i] * coefficient_1);
        simplex_delta_[i].second = Round(simplex_delta_[i].second - solve_vector_[idx_str][i] * coefficient_2);
    }
}

int SimplexMetodMin::CalculateMin(int idx) {
    double min = 10000000;
    int str_idx = 0;
    for (int i = 0; i < cur_base_.size(); i++) {
        if (solve_vector_[i][idx] > 0 && Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx]) < min) {
            min = Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx]);
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
        for (int j = 0; j < size_; j++) {
            if (solve_vector_[i][j] == 1) {
                if (j + 1 > count_limit_) {
                    std::cout << "x" << cur_base_[i] - 1 << " = " << 0 << std::endl;
                    break;
                }
                std::cout << "x" << cur_base_[i] - 1 << " = " << solve_vector_[i][size_ - 1] << std::endl;
            }
        }
    }
    std::cout << "Target function value = " << simplex_delta_[size_ - 1].second;
}

void SimplexMetodMin::Print() {
    std::cout << "Current table:" << std::endl;
    for (int i = 0; i < count_limit_; i++) {
        for (int j = 0; j < size_; j++) {
            std::cout << solve_vector_[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl << "Simplex delta:" << std::endl;
    for (int i = 0; i < size_; i++) {
        std::cout << simplex_delta_[i].first << "M " << simplex_delta_[i].second << "; ";
    }
    std::cout << std::endl << std::endl;
}

SimplexMetodMin::SimplexMetodMin(int n, int m, bool type): AbstractSimplexMetod(n, m, type) {

    int tmp = 0;
    for (int i = 0; i < n; i++) {
        std::cin >> coefficient_func_[i];
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> A_[i][j];
            solve_vector_[i][j] = A_[i][j];
        }
        std::cin >> b_[i];
        cur_base_[i] = b_[i];
    }
}
