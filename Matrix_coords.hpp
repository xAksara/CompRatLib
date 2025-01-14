#pragma once
#include <cstdlib>

class Matrix_coords {
public:
    size_t r1, r2, c1, c2;
    Matrix_coords(size_t r1, size_t c1, size_t r2, size_t c2) : r1(r1), c1(c1), r2(r2), c2(c2) { }
    Matrix_coords(size_t r1, size_t c1) : r2(r1), c2(c1) { }
};

class Matrix_row_coord {
public:
    size_t r, c1, c2;
    Matrix_row_coord(size_t r, size_t c1, size_t c2) : c1(c1), c2(c2), r(r) { }
};

class Matrix_col_coord {
public:
    size_t r1, r2, c;
    Matrix_col_coord(size_t c, size_t r1, size_t r2) : r1(r1), r2(r2), c(c) { }
};