//
// Created by Rostislav on 30.10.2024.
//

#ifndef ABSTRACTSIMPLEXMETOD_H
#define ABSTRACTSIMPLEXMETOD_H
#include <vector>

class AbstractSimplexMetod {
public:
    virtual ~AbstractSimplexMetod() = default;

    bool type_; // тип задачи 1-max 0-min
    int count_coefficient_func_; // количество коэффициентов целевой ф-и
    int count_limit_; // количество ограничений
    int size_; // размер матрицы
    std::vector<std::vector<double> > A_; // матрица A
    std::vector<int> b_; // матрица b
    std::vector<double> coefficient_func_; // коэффициенты целевой ф-и
    std::vector<std::vector<double> > solve_vector_; // сама симплекс таблица
    std::vector<std::pair<double, double> > simplex_delta_; // симплекс разности
    std::vector<double> min_vector_; // для посиска направляющей строки
    std::vector<int> cur_base_; // текущее базисное решение

    AbstractSimplexMetod(int n, int m, bool type) : size_(n + m + m + 1)
                                                    , type_(type)
                                                    , count_coefficient_func_(n)
                                                    , count_limit_(m)
                                                    , coefficient_func_(size_, 0)
                                                    , A_(m, std::vector<double>(n))
                                                    , b_(m, 0)
                                                    , solve_vector_(m, std::vector<double>(m + m + n + 1, 0))
                                                    , simplex_delta_(m + m + n + 1, std::pair<int, int>(0, 0))
                                                    , cur_base_(m) {
    }

    // матричное преобразование
    virtual void TransformationMatrix(int idx_column, int idx_str) = 0;

    // основной метод для поиска оптимального решения
    virtual void FindSolve() = 0;

    virtual void CreateTableMin() = 0;

    // подсчет симплекс разности
    virtual void CalculateSimplexDelta(int idx_str, int idx_column) = 0;

    // поиск минимальной симплекс разности для поиска направляющего столбца
    virtual int FindMinSimplex() = 0;

    // поиск направляющей строки
    virtual int CalculateMin(int idx) = 0;

    // вывод матрицы и симлекс разностей
    virtual void Print() = 0;

    // вывод оптимального базисного решения
    virtual void PrintAnswer() = 0;

    // округление числа
    virtual double Round(double x) = 0;

    // округления значений матрицы
    virtual std::vector<std::vector<double> > RoundArray(std::vector<std::vector<double> > &vector) = 0;
};
#endif //ABSTRACTSIMPLEXMETOD_H
