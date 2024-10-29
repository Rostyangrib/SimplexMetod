#include <iostream>
#include <vector>

class AbstractSimplexMetod {
public:
    virtual void CreateTableMin()  = 0;
    virtual void CalculateSimplexDelta(int idx_str, int idx_column) = 0;
    virtual int FindMinSimplex() = 0;
    virtual int CalculateMin(int idx) = 0;
};

class SimplexMetodMin : public AbstractSimplexMetod {
public:
    bool type_;
    int count_coefficient_func_;
    int count_limit_;
    int size_;
    std::vector<std::vector<int> > A_;
    std::vector<int> b_;
    std::vector<int> coefficient_func_;
    std::vector<std::vector<double> > solve_vector_;
    std::vector<std::pair<double, double> > simplex_delta_;
    std::vector<double> min_vector_;
    std::vector<int> cur_base_;

    SimplexMetodMin(int n, int m, bool type);

    void CreateTableMin() override;

    void CalculateSimplexDelta(int idx_str, int idx_column) override;

    int FindMinSimplex() override;

    int CalculateMin(int idx) override;

    void Print();

    void PrintAnswer();

    void TransformationMatrix(int idx_column, int idx_str);

    void FindSolve();

};

class SimplexMetodMax : public SimplexMetodMin {
    public:SimplexMetodMax(int n, int m, bool type) : SimplexMetodMin(n, m, type) {

    }
};

/*

2 3
0
12 16
2 6 24
5 4 31
2 3 18

3 2
0
170 370 400
3 2 4 40
2 6 5 30


2 3
0
1 2
1 0 20
0 1 15
1 2 40

3 3
0
40 2 1
1 0 3 20
4 5 7 15
1 2 0 40

*/

int main() {
    int n, m;
    bool type;
    std::cin >> n >> m >> type;
    if (type == 0) {
        SimplexMetodMin simplex(n, m, type);
        simplex.FindSolve();
    }
    return 0;
}

void SimplexMetodMin::FindSolve() {
    CreateTableMin();
    Print();
    for (int i = 0; i < 3; i++) {
        min_vector_.clear();
        int idx_column = FindMinSimplex();
        int idx_str = CalculateMin(idx_column);
        TransformationMatrix(idx_column, idx_str);
        CalculateSimplexDelta(idx_str, idx_column);
        Print();
    }
    PrintAnswer();
}


void SimplexMetodMin::TransformationMatrix(int idx_column, int idx_str) {
    double coefficient = solve_vector_[idx_str][idx_column];
    for (int i = 0; i < size_; i++) {
        solve_vector_[idx_str][i] /= coefficient;
    }
    for (int i = 0; i < count_limit_; i++) {
        double coefficient_1 = solve_vector_[i][idx_column];
        for (int j = 0; j < size_; j++) {
            if (i == idx_str) {
                break;
            }
            solve_vector_[i][j] -= solve_vector_[idx_str][j] * coefficient_1;
        }
    }
}


void SimplexMetodMin::CalculateSimplexDelta(int idx_str, int idx_column) {
    double coefficient_1 = simplex_delta_[idx_column].first;
    double coefficient_2 = simplex_delta_[idx_column].second;
    for (int i = 0; i < size_; i++) {
        simplex_delta_[i].first -= solve_vector_[idx_str][i] * coefficient_1;
        simplex_delta_[i].second -= solve_vector_[idx_str][i] * coefficient_2;
    }
}

int SimplexMetodMin::CalculateMin(int idx) {
    double min = 10000000;
    int str_idx = 0;
    for (int i = 0; i < cur_base_.size(); i++) {
        if (solve_vector_[i][idx] > 0 && solve_vector_[i][size_ - 1] / solve_vector_[i][idx] < min) {
            min = solve_vector_[i][size_ - 1] / solve_vector_[i][idx];
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


SimplexMetodMin::SimplexMetodMin(int n, int m, bool type)
    : size_(n + m + m + 1)
      , type_(type)
      , count_coefficient_func_(n)
      , count_limit_(m)
      , coefficient_func_(size_, 0)
      , A_(m, std::vector<int>(n))
      , b_(m, 0)
      , solve_vector_(m, std::vector<double>(m + m + n + 1, 0))
      , simplex_delta_(m + m + n + 1, std::pair<int, int>(0, 0))
      , cur_base_(m) {
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

void SimplexMetodMin::PrintAnswer() {
    cur_base_.clear();
    for (int i = 0; i < size_; i++) {
        if (simplex_delta_[i].first == 0 && simplex_delta_[i].second == 0) {
            cur_base_.push_back(i);
        }
    }
    std::cout << std::endl << "Basic solution" << std::endl;
    for (int i = 0; i < cur_base_.size(); i++) {
        for (int j = 0; j < count_limit_; j++) {
            if (solve_vector_[j][cur_base_[i]] == 1) {
                std::cout << "x" << j + 1 << " = " << solve_vector_[j][size_ - 1] << std::endl;
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
