#pragma once
#include <sstream>
#include <string>
#include "Matrix.hpp"
#include "Matrix_coords.hpp"


template <typename T, typename Container>
class Matrix;

/**
 * @class Прокси обхект позволяющий обращаться к срезу матрицы
 * 
 * @tparam T тип элементов матрицы
 * @tparam container_T контейнер матрицы
 */
template<typename T, typename container_T>
class Matrix_proxy {
    friend class Matrix<T, container_T>; // для клин поинтер
    size_t r1, r2, c1, c2;
    size_t proxy_rows, proxy_columns;
    Matrix<T, container_T>* matrix_p;
    Matrix<T, container_T>* initial_matrix_p;
    bool is_copy = false;
    

/**
 * @brief создает локальную копию матрицы исключая элементы, не вошедшие в срез
 * 
 */
    void make_local_copy() {
        is_copy = true;
        // matrix_p->clean_pointer(this);
        Matrix<T, container_T>* result_p = new Matrix<T, container_T>(matrix_p->getRows(), matrix_p->getColumns(), matrix_p->getEpsilon());
        std::cout << "NEW " << result_p << std::endl;
        for (const auto& [key, value] : (*matrix_p).getData()) { // key - пара (строка, столбец)
            if (key.first >= r1 && key.first <= r2 && key.second >= c1 && key.second <= c2) {
                result_p->set(key.first, key.second, value);
            }
        }
        matrix_p = result_p;
    }
/**
 * @brief обнуляет указатель на матрицу, после чего в случае обращения к элеиментам среза бросает исключение
 * 
 */
    void clean_pointer() {
        initial_matrix_p = nullptr;
        if (is_copy) {
            if (matrix_p != nullptr) {
                std::cout << "DELETE " << matrix_p << std::endl;
                delete matrix_p;
            }
            std::cout << "Pointer: " << matrix_p << std::endl;
            matrix_p = nullptr;
            is_copy = false;
        } else {
            matrix_p = nullptr;
        }
    }


public:
    bool is_row = false;
    bool is_column = false;
/**
 * @brief Строит прокси обхект по матрице и переданным координатам
 * 
 * @param matrix исходная матрица
 * @param coords координаты среза
 */
    Matrix_proxy(Matrix<T, container_T>& matrix, const Matrix_coords& coords)
        : matrix_p(&matrix), initial_matrix_p(&matrix)
    { 
        r1 = (coords.r1 == -1 ? 0 : coords.r1);
        r2 = (coords.r2 == -1 ? matrix.getRows() - 1 : coords.r2);
        c1 = (coords.c1 == -1 ? 0 : coords.c1);
        c2 = (coords.c2 == -1 ? matrix.getColumns() - 1 : coords.c2);
        proxy_rows = r2 - r1 + 1;
        proxy_columns = c2 - c1 + 1;
    }
/**
 * @brief Строит прокси обхект по матрице и переданным координатам
 * 
 * @param matrix исходная матрица
 * @param coords координаты среза
 */
    Matrix_proxy(Matrix<T, container_T>& matrix, const Matrix_row_coord& coords)
        : matrix_p(&matrix), is_row(true), initial_matrix_p(&matrix)
    { 
        r1 = coords.r;
        r2 = 0;
        c1 = (coords.c1 == -1 ? 0 : coords.c1);
        c2 = (coords.c2 == -1 ? matrix.getColumns() - 1 : coords.c2);
        proxy_rows = 1;
        proxy_columns = c2 - c1 + 1;
    }
/**
 * @brief Строит прокси обхект по матрице и переданным координатам
 * 
 * @param matrix исходная матрица
 * @param coords координаты среза
 */
    Matrix_proxy(Matrix<T, container_T>& matrix, const Matrix_col_coord& coords)
        : matrix_p(&matrix), is_column(true), initial_matrix_p(&matrix)
    { 
        r1 = (coords.r1 == -1 ? 0 : coords.r1);
        r2 = (coords.r2 == -1 ? matrix.getRows() - 1 : coords.r2);
        c1 = coords.c;
        c2 = 0;
        proxy_rows = r2 - r1 + 1;
        proxy_columns = 1;
    }


    ~Matrix_proxy() {
        std::cout << "DESTRUCTOR PROXY" << std::endl;
        if (initial_matrix_p != nullptr) {
            initial_matrix_p->clean_pointer(this);
        }
        if (is_copy && matrix_p != nullptr) {
            std::cout << "DELETE " << matrix_p << std::endl;
            delete matrix_p;
        }
    }

    double getEpsilon() const {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        return matrix_p->getEpsilon();
    }

    container_T getData() const {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        return matrix_p->getData();
    }

    size_t getSize() const {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        if (!is_row && !is_column) {
            throw InvalidArgumentException("Для прокси матрицы используйте getRows() или getColumns(). Флаги строки и столбца равны: ", 0);
        }
        return (is_row ? proxy_columns : proxy_rows);
    }

    size_t getRows() const {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        if (is_row || is_column) {
            throw InvalidArgumentException("Для строки или столбца используйте getSize(). is_row == ", is_row);
        }
        return (is_row ? c1 : r1);
    }

    const T& operator()(size_t i, size_t j) const {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        if (is_row || is_column) {
            throw InvalidArgumentException("Прокси-строка или столбец должны принимать номер строки или столбца. Передан лишний аргумент: ", j);
        }
        if (i >= proxy_rows || j >= proxy_columns) {
            throw MatrixIndexOutOfRangeException(i, j, proxy_rows, proxy_columns);
        }
        size_t global_row = r1 + i;
        size_t global_column = c1 + j;
        return (*matrix_p)(global_row, global_column);
    }

    const T& operator()(size_t idx) const { // для прокси строки или столбца
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        if (is_row) {
            if (idx >= proxy_columns) {
                throw VectorIndexOutOfRangeException(idx, proxy_columns);
            }
            size_t global_row = r1;
            size_t global_column = c1 + idx;
            return (*matrix_p)(global_row, global_column);
        } else if (is_column) { // если столбец
            if (idx >= proxy_rows) {
                throw VectorIndexOutOfRangeException(idx, proxy_rows);
            }
            size_t global_row = r1 + idx;
            size_t global_column = c1;
            return (*matrix_p)(global_row, global_column);
        } 
        throw InvalidArgumentException("Прокси-матрица должна принимать номер строки и столбца. Передано: ", idx);
    }

    void set(size_t i, size_t j, T value) {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        if (is_row || is_column) {
            throw InvalidArgumentException("Прокси-строка или столбец должны принимать номер строки либо столбца и значение. Передан лишний аргумент: ", j);
        }
        if (value < matrix_p->getEpsilon()) {
            return;
        }
        if (!is_copy) {
            make_local_copy();
        }
        size_t global_row = r1 + i;
        size_t global_column = c1 + j;
        (*matrix_p).set(global_row, global_column, value);
    }

    void set(size_t idx, T value) {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        if (!is_row && !is_column) {
            throw InvalidArgumentException("Прокси-матрица помимо значения должна принимать номер строки и столбца. Передано: ", idx);
        }
        if (value < matrix_p->getEpsilon()) return;
        if (!is_copy) {
            make_local_copy();
        }
        if (is_row) {
            size_t global_row = r1;
            size_t global_column = c1 + idx;
            (*matrix_p).set(global_row, global_column, value);
        } else {
            size_t global_row = r1;
            size_t global_column = c1 + idx;
            (*matrix_p).set(global_row, global_column, value);
        }
    }

    std::string to_string() const {
        if (matrix_p == nullptr) {
            throw EmptyContainerException();
        }
        std::stringstream ss;
        ss << "<Proxy matrix rows: " << proxy_rows << ", columns: " << proxy_columns << ", epsilon: " << matrix_p->getEpsilon() << ", data:[";
        size_t counter = 0; // чтобы поймать  последний элемент. Хотя можно через итераторы, но так симпотичнее
        size_t data_size = matrix_p->getData().size();
        if (is_row) {
            for (const auto& [key, value] : matrix_p->getData()) {
                if (key.second >= c1 && key.second <= c2) {
                    ss << "{(" << key.first - r1 << ", " << key.second - c1 << "), " << value << (++counter < data_size ? "}, " : "}");
                }
            }
        } else if (is_column) {
            for (const auto& [key, value] : matrix_p->getData()) {
                if (key.second >= r1 && key.second <= r2) {
                    ss << "{(" << key.first - r1 << ", " << key.second - c1 << "), " << value << (++counter < data_size ? "}, " : "}");
                }
            }
        } else {
            for (const auto& [key, value] : matrix_p->getData()) {
                if (key.first >= r1 && key.first <= r2 && key.second >= c1 && key.second <= c2) {
                    ss << "{(" << key.first - r1 << ", " << key.second - c1 << "), " << value << (++counter < data_size ? "}, " : "}");
                }
            }
        }
        ss << "]>";
        return ss.str();
    }
};

template<typename T, typename container_T>
std::ostream& operator<<(std::ostream& os, const Matrix_proxy<T, container_T>& m) {
    os << m.to_string();
    return os;
}