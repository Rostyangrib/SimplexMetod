//
// Created by Rostislav on 30.10.2024.
//

#include "iostream"
#include <cmath>
#include "SimplexMetodMax.h"


void SimplexMetodMax::FindSolve() {
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
        TransformationMatrix(idx_column, idx_str);
        solve_vector_ = RoundArray(solve_vector_);
        CalculateSimplexDelta(idx_str, idx_column);
        Print();
    }
    PrintAnswer();
}

double SimplexMetodMax::Round(double x) {
    double delta = 1e-3;
    x = round(x * pow(10, 2)) / pow(10, 2);

    if (x > 0 && x <= delta || x < 0 && x >= -delta) {
        x = 0;
    }
    return x;
}

std::vector<std::vector<double>> SimplexMetodMax::RoundArray(std::vector<std::vector<double>> &vector) {
    // Round matrix
    for (auto& i : vector) {
        for (auto & j : i) {
            j = Round(j);
        }
    }
    return vector;
}

SimplexMetodMax::SimplexMetodMax(int n, int m, bool type): AbstractSimplexMetod(n, m, type) {
    size_ = n + m + 1;
    for (int i = 0; i < solve_vector_.size(); i++) {
        solve_vector_[i].resize(n + m + 1, 0);
    }

    for (int i = 0; i < n; i++) {
        std::cin >> coefficient_func_[i];
        coefficient_func_[i];
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            double in;
            std::cin >> in;
            solve_vector_[i][j] = A_[i][j] = in;
        }
        std::cin >> b_[i];
        cur_base_[i] = b_[i];
    }
}

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
        solve_vector_[idx_str][i] = Round(solve_vector_[idx_str][i] / coefficient);
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

void SimplexMetodMax::CalculateSimplexDelta(int idx_str, int idx_column) {
    double coefficient_2 = simplex_delta_[idx_column].second;
    for (int i = 0; i < size_; i++) {
        simplex_delta_[i].second = Round(simplex_delta_[i].second - solve_vector_[idx_str][i] * coefficient_2);
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
                    std::cout << "x" << cur_base_[i] + 1 << " = " << 0 << std::endl;
                    break;
                }
                std::cout << "x" << cur_base_[i] + 1<< " = " << solve_vector_[i][size_ - 1] << std::endl;
            }
        }
    }
    std::cout << "Target function value = " << simplex_delta_[size_ - 1].second;
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
        if (solve_vector_[i][idx] > 0 && Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx]) < min) {
            min = Round(solve_vector_[i][size_ - 1] / solve_vector_[i][idx]);
            min_vector_.push_back(min);
            str_idx = i;
        }
    }
    cur_base_[str_idx] = idx;
    return str_idx;
}
