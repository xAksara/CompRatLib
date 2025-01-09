#pragma once
#include "Matrix.hpp"
#include <memory>
#include <iostream>
#include <stdexcept>
#include <map>
#include "Exceptions.hpp"

template <typename element_T, typename container_T>
class Matrix;
/**
 * @class Matrix_proxy
 * @brief Provides a proxy object to manipulate a slice of a larger matrix.
 *
 * @tparam element_T The type of elements stored in the matrix (default: double).
 * @tparam container_T The type of container used to store matrix elements (default: std::map<std::pair<unsigned int, unsigned int>, element_T>).
 *
 * The `Matrix_proxy` class represents a submatrix (slice) of a larger matrix. 
 * It enables both read and write access to elements within the defined slice. 
 * Changes made to the proxy are reflected in the original matrix unless a local copy is made.
 */
template <typename element_T = double, typename container_T = std::map<std::pair<unsigned int, unsigned int>, element_T>>
class Matrix_proxy {
private:
    unsigned int r1, r2, c1, c2;
    /**
     * @brief Creates a local copy of the matrix slice.
     *
     * This method is called automatically when modifying the slice. 
     * It ensures that modifications are made to the local copy instead of the original matrix.
     */
    void copyMatrixSlice() {
        if (!has_local_copy) {
            std::cout << "Making copy!" << std::endl;
            Matrix<element_T, container_T>* M_new = new Matrix<element_T, container_T>(r2 - r1 + 1, c2 - c1 + 1, M->eps);
            for (unsigned int i = 0; i < r2 - r1; ++i) {
                for (unsigned int j = 0; j < c2 - c1; ++j) {
                    element_T elem = (*M)(r1 + i, c1 + j);
                    if (elem != 0) {
                        M_new->set(i, j, elem);
                    }
                }
            }
            has_local_copy = true;
            M = M_new;
        }
    }
public:
    bool has_local_copy = false;
    Matrix<element_T, container_T>* M;
/**
     * @brief Constructs a proxy object for a slice of a matrix.
     *
     * @param M The original matrix from which the slice is extracted.
     * @param r1 The starting row of the slice.
     * @param c1 The starting column of the slice.
     * @param r2 The ending row of the slice.
     * @param c2 The ending column of the slice.
     */
    Matrix_proxy(const Matrix<element_T, container_T>& M, unsigned int r1, unsigned int c1, unsigned int r2, unsigned int c2)  : 
                 M(const_cast<Matrix<element_T, container_T>*>(&M)), r1(r1), c1(c1), r2(r2), c2(c2) { }
      /**
     * @brief Destructor for the proxy object.
     *
     * If a local copy of the matrix has been created, it is properly deallocated to avoid memory leaks.
     */
    ~Matrix_proxy() {
        if (has_local_copy && M != nullptr) {
            delete M;
        }
    }
     /**
     * @brief Accesses an element within the proxy slice.
     *
     * @param row The row index of the element relative to the slice.
     * @param col The column index of the element relative to the slice.
     * @return A constant reference to the requested element.
     * @throws EmptyContainerError If the matrix is null.
     * @throws OutOfRangeException If the requested indices are out of bounds for the slice.
     */
    const element_T& operator()(unsigned int row, unsigned int col) const {
        if (M == nullptr) {
            throw EmptyContainerError();
        }
        unsigned int source_row = r1 + row;
        unsigned int source_col = c1 + col;
        if (source_row > r2 || source_col > c2) {
            throw OutOfRangeException(row, col, r2 - r1, c2 - c1);
        }
        if (has_local_copy) {
            return M->operator()(row, col); 
        } else {
            return M->operator()(source_row, source_col);
        }
    }
    /**
     * @brief Sets an element within the proxy slice.
     *
     * @param row The row index of the element relative to the slice.
     * @param col The column index of the element relative to the slice.
     * @param value The new value to assign to the element.
     * @throws EmptyContainerError If the matrix is null.
     * @throws OutOfRangeException If the requested indices are out of bounds for the slice.
     */
    void set(unsigned int row, unsigned int col, element_T value) {
        if (M == nullptr) {
            throw EmptyContainerError();
        }
        unsigned int source_row = r1 + row;
        unsigned int source_col = c1 + col;
        if (source_row > r2 || source_col > c2) {
            throw OutOfRangeException(row, col, r2 - r1, c2 - c1);
        }
        if (!has_local_copy)
            copyMatrixSlice(); 
        M->set(row, col, value);
    }

    /**
     * @brief Clears the internal matrix pointer and deletes the local copy if it exists.
     *
     * After calling this method, the proxy object becomes invalid and cannot be used.
     */
    void clearPointer() {
        if (has_local_copy && M != nullptr) {
            delete M;
        }
        this->M = nullptr;
        has_local_copy = false;
    }
};

template <typename element_T = double, typename container_T = std::map<std::pair<unsigned int, unsigned int>, element_T>>
class Matrix_proxy_row {
private:
    unsigned int r, c1, c2;
    void copyMatrixSlice() {
        if (!has_local_copy) {
            std::cout << "Making copy!" << std::endl;
            Matrix<element_T, container_T>* M_new = new Matrix<element_T, container_T>(1, c2 - c1 + 1, M->eps);
            for (unsigned int j = 0; j < c2 - c1; ++j) {
                element_T elem = (*M)(r, c1 + j);
                if (elem != 0) {
                    M_new->set(0, j, elem);
                }
            }
            has_local_copy = true;
            M = M_new;
        }
    }
public:
    bool has_local_copy = false;
    Matrix<element_T, container_T>* M;
    
    Matrix_proxy_row(const Matrix<element_T, container_T>& M, unsigned int r, unsigned int c1, unsigned int c2) : 
                    M(const_cast<Matrix<element_T, container_T>*>(&M)), r(r), c1(c1), c2(c2) { }

    const element_T& operator()(unsigned int col) const {
        if (M == nullptr) {
            throw EmptyContainerError();
        }
        unsigned int source_row = r;
        unsigned int source_col = c1 + col;
        if (source_col > c2) {
            throw OutOfRangeException(col, c2 - c1);
        }
        if (has_local_copy) {
            return M->operator()(0, col); 
        } else {
            return M->operator()(source_row, source_col);
        }
    }
    void set(unsigned int col, element_T value) {
        if (M == nullptr) {
            throw EmptyContainerError();
        }
        unsigned int source_row = r;
        unsigned int source_col = c1 + col;
        if (source_col > c2) {
            throw OutOfRangeException(col, c2 - c1);
        }
        if (!has_local_copy)
            copyMatrixSlice(); 
        M->set(0, col, value);
    }
    void clearPointer() {
        if (has_local_copy && M != nullptr) {
            delete M;
        }
        M = nullptr;
        has_local_copy = false;
    }
};

template <typename element_T = double, typename container_T = std::map<std::pair<unsigned int, unsigned int>, element_T>>
class Matrix_proxy_col {
private:
    unsigned int c, r1, r2;
    void copyMatrixSlice() {
        if (!has_local_copy) {
            std::cout << "Making copy!" << std::endl;
            Matrix<element_T, container_T>* M_new = new Matrix<element_T, container_T>(r2 - r1 + 1,1, M->eps);
            for (unsigned int j = 0; j < r2 - r1; ++j) {
                element_T elem = (*M)(r1 + j, c);
                if (elem != 0) {
                    M_new->set(j, 0, elem);
                }
            }
            has_local_copy = true;
            M = M_new;
        }
    }
public:
    bool has_local_copy = false;
    Matrix<element_T, container_T>* M;
    
    Matrix_proxy_col(const Matrix<element_T, container_T>& M, unsigned int c, unsigned int r1, unsigned int r2) : 
                    M(const_cast<Matrix<element_T, container_T>*>(&M)), c(c), r1(r1), r2(r2) { }

    const element_T& operator()(unsigned int row) const {
        if (M == nullptr) {
            throw EmptyContainerError();
        }
        unsigned int source_row = r1 + row;
        unsigned int source_col = c;
        if (source_row > r2) {
            throw OutOfRangeException(row, r2 - r1);
        }
        if (has_local_copy) {
            return M->operator()(row, 0); 
        } else {
            return M->operator()(source_row, source_col);
        }
    }

    void set(unsigned int row, element_T value) {
        if (M == nullptr) {
            throw EmptyContainerError();
        }
        unsigned int source_row = r1 + row;
        unsigned int source_col = c;
        if (source_row > r2) {
            throw OutOfRangeException(row, r2 - r1);
        }
        if (!has_local_copy)
            copyMatrixSlice(); 
        M->set(row, 0, value);
    }
    void clearPointer() {
        if (has_local_copy && M != nullptr) {
            delete M;
        }
        M = nullptr;
        has_local_copy = false;
    }
};