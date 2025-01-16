#pragma once

#include <string>
#include <sstream>
#include <iostream>



class MyBaseException {
public:
    virtual std::string getMessage() = 0;
    virtual ~MyBaseException() = default;
};

// Потому что не работает макрос, список аргументов шаблона путает с аргументами макроса(вроде) 
// (в скобки оборачивать второй аргумент пробовал - не работает)
//    ASSERT_THROW(Rational_number<int>("5/0"), DivisionByZeroException<std::string, std::string>);
//      GoogleTests.hpp:22:96: error: macro "ASSERT_THROW" passed 3 arguments, but takes just 2
//      22 |     ASSERT_THROW(Rational_number<int>("5/0"), DivisionByZeroException<std::string, std::string>)

class ZeroDivision { };
class InvalidArgument { };
class Overflow { };
class NegativeEpsilon { };

template<typename T1, typename T2>
class DivisionByZeroException : public ZeroDivision, public MyBaseException {
    std::string message;
    T1 numerator;
    T2 denominator;
public:
    DivisionByZeroException(const T1& num, const T2& denom) : numerator(num), denominator(denom), message("Division by zero.") { }
    T1 getNumerator() { return numerator; }
    T1 getDenominator() { return denominator; }
    std::string getMessage() override { return message; }
};

template<typename T>
class InvalidArgumentException : public InvalidArgument, public MyBaseException  {
    T argument;
    std::string message;
public:
    InvalidArgumentException(std::string what, const T& arg) {
        std::ostringstream oss;
        oss << what << arg << std::endl;
        message = oss.str();
        argument = arg;
    }
    T getArgument() { return argument; }
    std::string getMessage() override { return message; }
};

template<typename from_T>
class OverflowException : public Overflow, public MyBaseException  {
    std::string message;
public:
    OverflowException(from_T from) {
        std::ostringstream oss;
        oss << "Переполнение. Значение " << from << " слишком велико." << std::endl;
        message = oss.str();
    }
    std::string getMessage() override { return message; }

};

class NegativeEpsilonException : public MyBaseException  {
    std::string message;
    double epsilon;
public:
    NegativeEpsilonException(double epsilon) : epsilon(epsilon) {
        std::ostringstream oss;
        oss << "Эпсилон должен быть больше или равен нулю. Передано: " << epsilon << std::endl;
        message = oss.str();
    }
    std::string getMessage() override { return message; }
};

class VectorIndexOutOfRangeException : public MyBaseException  {
    std::string message;
    size_t size1;
    size_t max_size1;
public:
    VectorIndexOutOfRangeException(size_t size, size_t max_size) : size1(size), max_size1(max_size) {
        std::ostringstream oss;
        oss << "Ошибка в индексе вектора. Передано: " << size1 << ". Максимальный индекс" << max_size1 << '.' << std::endl;
        message = oss.str();
    }
    size_t getVectorSize() { return max_size1; }
    size_t getVectorIndex() { return size1; }
    std::string getMessage() override { return message; }
};


class MatrixIndexOutOfRangeException : public MyBaseException  {
    std::string message;
    size_t size1;
    size_t max_size1;
    size_t size2;
    size_t max_size2;
public:
    MatrixIndexOutOfRangeException(size_t size1, size_t size2, size_t max_size1, size_t max_size2) : size1(size1), max_size1(max_size1), size2(size2), max_size2(max_size2) {
        std::ostringstream oss;
        oss << "Ошибка в индексах матрицы. Передано: (" << size1 << ", " << size2 << "). Максимальные индексы: (" << max_size1 << ", " << max_size2 << ")." << std::endl;
        message = oss.str();
    }
    std::pair<size_t, size_t> getMatrixSize() { return std::make_pair(max_size1, max_size2); }
    std::pair<size_t, size_t> getMatrixIndeces() { return std::make_pair(size1, size2); }
    std::string getMessage() override { return message; }
};

class VectorSizeMissmatch : public MyBaseException {
    std::string message;
    size_t size1;
    size_t size2;
public:
    VectorSizeMissmatch(size_t size1, size_t size2) {
        std::ostringstream oss;
        oss << "Неверные размеры векторов. Размер левого вектора: " << size1 << ". Правого: " << size2 << '.' << std::endl;
        message = oss.str();
    }

    size_t getLeftSize() { return size1; }
    size_t getRightSize() { return size2; }
    std::string getMessage() override { return message; }
};

class MatrixSizeMissmatch : public MyBaseException {
    std::string message;
    size_t rows1;
    size_t columns1;
    size_t rows2;
    size_t columns2;
public:
    MatrixSizeMissmatch(size_t rows1, size_t columns1, size_t rows2, size_t columns2) {
        std::ostringstream oss;
        oss << "Неверные размеры матрицы. Размеры левой матрицы: [" << rows1 << ", " << columns1 << "] . Размеры правой матрицы: [" << rows1 << ", " << columns1 << "] ." << std::endl;
        message = oss.str();
    }

    size_t getLeftRows() { return rows1; }
    size_t getLeftColumns() { return columns1; }
    size_t getRightRows() { return rows2; }
    size_t getRightColumns() { return columns2; }
    std::string getMessage() override { return message; }
};



class EmptyContainerException : public MyBaseException  {
    std::string message;
public:
    EmptyContainerException() : message("Указатель на матрицу null.") {}
    std::string getMessage() override { return message; }
};


template<typename T>
class ProxyToVectorException : public MyBaseException  {
    std::string message;
    T proxy;
public:
    ProxyToVectorException(std::string message, T proxy) : message(message), proxy(proxy) {}
    std::string getMessage() override { return message; }
    T getProxy() { return proxy; }
};

class WrongFileException : public MyBaseException {
    std::string message;
public:
    WrongFileException(std::string message) : message(message) {}
    std::string getMessage() override { return message; }
};