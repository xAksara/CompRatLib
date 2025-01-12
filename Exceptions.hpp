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

// class Exception {
// protected:
//     std::string message; 
// public:
//     template<typename... Args>
//     MyException(const std::string& error_message, Args... args) {
//         std::ostringstream oss;
//         oss << error_message;
//         appendParams(oss, args...);
//         message = oss.str();
//     }
    
//     virtual std::string getMessage() const {
//         return message;
//     }
// private:
//     void appendParams(std::ostringstream&) {}
    
//     template<typename T, typename... Args>
//     void appendParams(std::ostringstream& oss, T param, Args... args) {
//         oss << " " << param;
//         appendParams(oss, args...);
//     }
// };