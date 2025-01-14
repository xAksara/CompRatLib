#pragma once

#include <string>
#include <sstream>
#include <iostream>

// Потому что не работает макрос, список аргументов шаблона путает с аргументами макроса(вроде) 
// (в скобки оборачивать второй аргумент пробовал - не работает)
//    ASSERT_THROW(Rational_number<int>("5/0"), DivisionByZeroException<std::string, std::string>);
//      GoogleTests.hpp:22:96: error: macro "ASSERT_THROW" passed 3 arguments, but takes just 2
//      22 |     ASSERT_THROW(Rational_number<int>("5/0"), DivisionByZeroException<std::string, std::string>);
class ZeroDivision { };
class InvalidArgument { };
class Overflow { };
class NegativeEpsilon { };

template<typename T1, typename T2>
class DivisionByZeroException : public ZeroDivision {
    T1 numerator;
    T2 denominator;
public:
    DivisionByZeroException(const T1& num, const T2& denom) : numerator(num), denominator(denom) { }
    T1 getNumerator() { return numerator; }
    T1 getDenominator() { return denominator; }
};

template<typename T>
class InvalidArgumentException : public InvalidArgument {
    T* argument;
    std::string message;
public:
    InvalidArgumentException(std::string what, T& arg) {
        std::ostringstream oss;
        oss << what << arg << std::endl;
        message = oss.str();
        argument = &arg;
    }
    T& getArgument() { return argument; }
    std::string getMessage() { return message; }
};

template<typename from_T>
class OverflowException : public Overflow {
    std::string message;
public:
    OverflowException(from_T from) {
        std::ostringstream oss;
        oss << "Overflow exception. Value " << from << " too large (or small)" << std::endl;
        message = oss.str();
    }
    std::string getMessage() { return message; }

};

class NegativeEpsilonException {
    std::string message;
    double epsilon;
public:
    NegativeEpsilonException(double epsilon) : epsilon(epsilon) {
        std::ostringstream oss;
        oss << "Epsilon must be positive. Provided: " << epsilon << std::endl;
        message = oss.str();
    }
};

class VectorIndexOutOfRangeException {
    std::string message;
    size_t size1;
    size_t max_size1;
public:
    VectorIndexOutOfRangeException(size_t size, size_t max_size) : size1(size), max_size1(max_size) {
        std::ostringstream oss;
        oss << "Wrong vector index: " << size1 << ". Max index is" << max_size1 << '.' << std::endl;
        message = oss.str();
    }
    size_t getVectorSize() { return max_size1; }
    size_t getVectorIndex() { return size1; }
};


class MatrixIndexOutOfRangeException {
    std::string message;
    size_t size1;
    size_t max_size1;
    size_t size2;
    size_t max_size2;
public:
    MatrixIndexOutOfRangeException(size_t size1, size_t size2, size_t max_size1, size_t max_size2) : size1(size1), max_size1(max_size1), size2(size2), max_size2(max_size2) {
        std::ostringstream oss;
        oss << "Wrong matrix indeces: (" << size1 << ", " << size2 << "). Max indeces are (" << max_size1 << ", " << max_size2 << ")." << std::endl;
        message = oss.str();
    }
    std::pair<size_t, size_t> getMatrixSize() { return std::make_pair(max_size1, max_size2); }
    std::pair<size_t, size_t> getMatrixIndeces() { return std::make_pair(size1, size2); }
};

class VectorSizeMissmatch {
    std::string message;
    size_t size1;
    size_t size2;
public:
    VectorSizeMissmatch(size_t size1, size_t size2) {
        std::ostringstream oss;
        oss << "Vector size missmatch. Left vector size: " << size1 << ". Right vector size: " << size2 << '.' << std::endl;
        message = oss.str();
    }

    size_t getLeftSize() { return size1; }
    size_t getRightSize() { return size2; }
};