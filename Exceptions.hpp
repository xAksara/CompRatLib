#pragma once

#include <string>
#include <sstream>
#include <iostream>


class MyException {
protected:
    std::string message; 
public:
    template<typename... Args>
    MyException(const std::string& error_message, Args... args) {
        std::ostringstream oss;
        oss << error_message;
        appendParams(oss, args...);
        message = oss.str();
    }
    
    virtual std::string getMessage() const {
        return message;
    }
private:
    void appendParams(std::ostringstream&) {}
    
    template<typename T, typename... Args>
    void appendParams(std::ostringstream& oss, T param, Args... args) {
        oss << " " << param;
        appendParams(oss, args...);
    }
};

template<typename numerator_t>
class DivisionByZeroException : public MyException {
private:
    numerator_t numerator; 
public:
    DivisionByZeroException(numerator_t num) 
        : MyException("Division by zero error. Numerator:", num), numerator(num) {}

    numerator_t getNumerator() const {
        return numerator;
    }
};

class OutOfRangeException : public MyException {
private:
    size_t index1; 
    size_t index2; 
    bool isMatrix; 
public:
    OutOfRangeException(size_t idx, size_t max)
        : MyException("Out of range error. Index:", idx, "is out of bounds. Max index:", max),
          index1(idx), index2(-1), isMatrix(false) {}

    OutOfRangeException(size_t row, size_t col, size_t maxRow, size_t maxCol)
        : MyException("Out of range error. Indices (row, col):", "(" + std::to_string(row) + "," + std::to_string(col) + ")",
                      "are out of bounds. Max indices (row, col):", "(" + std::to_string(maxRow) + "," + std::to_string(maxCol) + ")"),
          index1(row), index2(col), isMatrix(true) {}

    size_t getVectorIndex() const {
        return index1;
    }

    size_t getRow() const {
        return index1;
    }

    size_t getColumn() const {
        return index2;
    }
};

class InvalidArgument : public MyException {
private:
public:
    std::string what;
    InvalidArgument(std::string what) : MyException("Invalid argument.", what), what(what) {}
};


class OverflowError : public MyException {
private:
public:
template<typename FromT, typename ToT>
    OverflowError(FromT from, ToT to) : MyException("Overflow error. Value", from, "too large for", to) {}
};

class EmptyContainerError : public MyException {
private:
public:
    EmptyContainerError() : MyException("This container is empty.") {}
};


class WrongFileFormat : public MyException {
private:
public:
    std::string what;
    WrongFileFormat() : MyException("Wrong file format.") {}
    WrongFileFormat(std::string what) : MyException("Wrong file format.", what), what(what) {}
};
