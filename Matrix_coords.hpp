#pragma once
/**
 * @file Matrix_coords.hpp
 * @brief classes for operator[] in Matrix.
 */
#include <string>

class Matrix_coords {
public:
    size_t r1, r2, c1, c2;
/**
 * \brief Constructor with 4 params
 * \param r1 first row
 * \param c1 first column
 * \param r2 last row
 * \param c2 last columns
 */
    Matrix_coords(size_t r1, size_t c1, size_t r2, size_t c2) : r1(r1), c1(c1), r2(r2), c2(c2) { }
/**
 * \brief Constructor with 2 params
 * \param r1 first row
 * \param c1 first column
 */
    Matrix_coords(size_t r1, size_t c1) : r2(r1), c2(c1) { }
};

class Matrix_row_coord {
public:
    size_t r, c1, c2;
/**
 * \brief Constructor with 3 params
 * \param c1 first column
 * \param c2 last column
 * \param r row
 */
    Matrix_row_coord(size_t r, size_t c1, size_t c2) : c1(c1), c2(c2), r(r) { }
};

class Matrix_col_coord {
public:
    size_t r1, r2, c;
/**
 * \brief Constructor with 3 params
 * \param r1 first row
 * \param r2 last row
 * \param c column
 */
    Matrix_col_coord(size_t c, size_t r1, size_t r2) : r1(r1), r2(r2), c(c) { }
};