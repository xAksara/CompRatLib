#pragma once
#include <map>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <vector>
#include <memory>
#include "Exceptions.hpp"
// #include "Vector.hpp"
#include "Rational.hpp"
#include "Complex.hpp"
#include "Matrix_coords.hpp"
#include "Matrix_proxy.hpp"
#include "Base.hpp"
#include "TypeChecker.hpp"

// using namespace space;

class MatrixBase : public Base {};

template <typename element_T = double, typename container_T = std::map<std::pair<unsigned int, unsigned int>, element_T>>
class Matrix : public MatrixBase {
private:
    static const element_T zero;
    std::vector<Matrix_proxy<element_T, container_T>*> proxies;
    std::vector<Matrix_proxy_row<element_T, container_T>*> proxies_row;
    std::vector<Matrix_proxy_col<element_T, container_T>*> proxies_col;
public:
    container_T data; // public for fast VecMatMul
    unsigned int rows;
    unsigned int columns;
    double eps;
    Matrix(unsigned int rows, unsigned int columns, double eps = 0) : rows(rows), columns(columns), eps(eps) {}
    Matrix(const Matrix& other) : data(other.data), eps(other.eps), rows(other.rows), columns(other.columns) {}
    Matrix(Matrix&& other) : data(std::move(other.data)), eps(other.eps), rows(other.rows), columns(other.columns) {}
    ~Matrix() {
        for (const auto& proxy : proxies) {
            proxy->clearPointer();
        }
        for (const auto& proxy : proxies_row) {
            proxy->clearPointer();
        }
        for (const auto& proxy : proxies_col) {
            proxy->clearPointer();
        }
        for (const auto& proxy : proxies) {
            delete proxy;
        }
        for (const auto& proxy : proxies_row) {
            delete proxy;
        }
        for (const auto& proxy : proxies_col) {
            delete proxy;
        }
    }

    auto myAbs(element_T value) {
        if constexpr (is_rational<std::decay_t<element_T>>::value || 
                      is_complex<std::decay_t<element_T>>::value) {
            return abs(value);
        } else {
            return std::abs(value);
        }
    }

    void set(unsigned int row, unsigned int col, element_T value) {
        if (row >= rows || col >= columns) {
            throw OutOfRangeException(row, col, rows - 1, columns - 1);
        }
        if (myAbs(value) >= eps || eps == 0.0) {
            data[{row, col}] = value;
        } else {
            data.erase({row, col});
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            data = other.data;
            eps = other.eps;
            rows = other.rows;
            columns = other.columns;
        }
        std::cout << "copy" << std::endl;
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
            eps = other.eps;
            rows = other.rows;
            columns = other.columns;
        }
        std::cout << "move" << std::endl;
        return *this;
    }

    const element_T& operator()(unsigned int row, unsigned int col) const {
        if (row >= rows || col >= columns) {
            throw OutOfRangeException(row, col, rows - 1, columns - 1);
        }
        auto it = data.find({row, col});
        return it != data.end() ? it->second : zero;
    }

    element_T& operator()(unsigned int row, unsigned int col) {
        if (row >= rows || col >= columns) {
            throw OutOfRangeException(row, col, rows - 1, columns - 1);
        } 
        return data[{row, col}];
    }

    Matrix_proxy<element_T, container_T>& operator[](const Matrix_coords& coords) {
        Matrix_proxy<element_T, container_T>* proxy = new Matrix_proxy<element_T, container_T>(*this, coords.r1, coords.c1, coords.r2, coords.c2);
        proxies.push_back(proxy);
        return *proxy;
    }
    Matrix_proxy_row<element_T, container_T>& operator[](const Matrix_row_coord& coords) {
        Matrix_proxy_row<element_T, container_T>* proxy = new Matrix_proxy_row<element_T, container_T>(*this, coords.r, coords.c1, coords.c2);
        proxies_row.push_back(proxy);
        return *proxy;
    }
    Matrix_proxy_col<element_T, container_T>& operator[](const Matrix_col_coord& coords) {
        Matrix_proxy_col<element_T, container_T>* proxy = new Matrix_proxy_col<element_T, container_T>(*this, coords.c, coords.r1, coords.r2);
        proxies_col.push_back(proxy);
        return *proxy;
    }

    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || columns != other.columns) {
            throw InvalidArgument("Wrong matrix dimension.");
        }
        Matrix result(rows, columns, eps);
        for (const auto& [index, value] : data) {
            result.set(index.first, index.second, value + other(index.first, index.second));
        }
        for (const auto& [index, value] : other.data) {
            if (data.find(index) == data.end()) {
                result.set(index.first, index.second, value);
            }
        }
        return result;
    }

    bool operator==(const Matrix& other) const {
        if (rows != other.rows || columns != other.columns) {
            return false;
        }
        for (const auto& [index, value] : other.data) {
            if (data.find(index) != data.end()) {
                if (data.at(index) != value) {
                    return false;
                }             
            } else {
                return false;
            }
        }
        return true;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || columns != other.columns) {
            throw InvalidArgument("Wrong matrix dimension.");
        }
        Matrix result(rows, columns, eps);
        for (const auto& [index, value] : data) {
            result.set(index.first, index.second, value - other(index.first, index.second));
        }
        for (const auto& [index, value] : other.data) {
            if (data.find(index) == data.end()) {
                result.set(index.first, index.second, -value);
            }
        }
        return result;
    }

    Matrix operator-() const {
        Matrix result(rows, columns, eps);
        for (const auto& [index, value] : data) {
            result.set(index.first, index.second, -value);
        }
        return result;
    }

    template <typename Number_T>
    Matrix operator*(const Number_T& scalar) const {
        Matrix result(rows, columns, eps);
        for (const auto& [index, value] : data) {
            result.set(index.first, index.second, value * scalar);
        }
        return result;
    }

    Matrix operator*(const Matrix& other) const {
        if (columns != other.rows) {
            throw InvalidArgument("Wrong matrix dimension.");
        }
        Matrix result(rows, other.columns, eps);
        for (const auto& [index1, value1] : data) {
            const auto& [row1, col1] = index1;
            for (const auto& [index2, value2] : other.data) {
                const auto& [row2, col2] = index2;
                if (col1 == row2) {
                    result.set(row1, col2, result(row1, col2) + value1 * value2);
                }
            }
        }
        return result;
    }

    template <typename Number_T>
    Matrix operator/(const Number_T& scalar) const {
        if (scalar == static_cast<Number_T>(0)) {
            throw DivisionByZeroException(*this);
        } 
        Matrix result(rows, columns, eps);
        for (const auto& [index, value] : data) {
            result.set(index.first, index.second, value / scalar);
        }
        return result;
    }

    Matrix operator~() const {
        Matrix result(columns, rows, eps);  
        for (const auto& [index, value] : data) {
            result.set(index.second, index.first, value);
        }
        return result;
    }

    std::string to_string() const {
        std::stringstream ss; 
        ss << "Matrix(rows=" << rows << ", columns=" << columns << ", eps=" << eps << ")\n";
        for (const auto& [index, value] : data) {
            ss << "[" << index.first << ", " << index.second << "] -> " << value << "\n";
        }
        return ss.str();
    }
};

template <typename element_T, typename container_T>
const element_T Matrix<element_T, container_T>::zero = element_T(0);

template <typename element_T, typename container_T>
std::ostream& operator<<(std::ostream& os, const Matrix<element_T, container_T>& obj) {
    os << obj.to_string();
    return os;
}

template<>
class Matrix<bool> : public MatrixBase {
private:
    unsigned int rows, columns;
    std::vector<uint64_t> data; 
    static constexpr size_t BITS_PER_BLOCK = 64;

    size_t get_block_index(unsigned int row, unsigned int col) const {
        return (row * columns + col) / BITS_PER_BLOCK;
    }

    size_t get_bit_offset(unsigned int row, unsigned int col) const {
        return (row * columns + col) % BITS_PER_BLOCK;
    }

public:
    Matrix<bool>(unsigned int rows, unsigned int columns)
        : rows(rows), columns(columns), data((rows * columns + BITS_PER_BLOCK - 1) / BITS_PER_BLOCK, 0) {}

    void set(unsigned int row, unsigned int col, bool value) {
        if (row >= rows || col >= columns) {
            throw OutOfRangeException(row, col, rows - 1, columns - 1);
        }
        size_t block = get_block_index(row, col);
        size_t offset = get_bit_offset(row, col);
        if (value) {
            data[block] |= (1ULL << offset);
        } else {
            data[block] &= ~(1ULL << offset);
        }
    }

    bool get(unsigned int row, unsigned int col) const {
        if (row >= rows || col >= columns) {
            throw OutOfRangeException(row, col, rows - 1, columns - 1);
        }
        size_t block = get_block_index(row, col);
        size_t offset = get_bit_offset(row, col);
        return (data[block] & (1ULL << offset)) != 0;
    }

    Matrix<bool> operator*(const Matrix<bool>& other) const {
        if (rows != other.rows || columns != other.columns) {
            throw InvalidArgument("Matrix dimensions mismatch");
        }
        Matrix<bool> result(rows, columns);
        for (size_t i = 0; i < data.size(); ++i) {
            result.data[i] = data[i] & other.data[i];
        }
        return result;
    }

    Matrix<bool> operator+(const Matrix<bool>& other) const {
        if (rows != other.rows || columns != other.columns) {
            throw InvalidArgument("Matrix dimensions mismatch");
        }
        Matrix<bool> result(rows, columns);
        for (size_t i = 0; i < data.size(); ++i) {
            result.data[i] = data[i] | other.data[i];
        }
        return result;
    }

    std::string to_string() const {
        std::ostringstream out;
        out << "Matrix(rows = "<<rows << ", columns = " << columns << ")\n";
        for (unsigned int row = 0; row < rows; ++row) {
            for (unsigned int col = 0; col < columns; ++col) {
                auto val = get(row, col);
                if (val)
                    out << "[" << row << ", " << col << "] -> " << val << std::endl;
            }
        }
        return out.str();
    }
};