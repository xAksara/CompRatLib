#pragma once
#include <string>
#include <cctype>
#include <limits>
#include <cmath>
#include <sstream>

bool isInteger(std::string str) {
    std::stringstream ss;
    for (size_t idx = 0; idx < str.size(); ++idx) {
        if (str[idx] != ' ' && str[idx] != '\t') ss << str[idx];
    }
    str = ss.str();
    if (str.size() == 0) return false;
    size_t idx = 0;
    if (str[0] == '-') ++idx;
    for (;idx < str.size(); ++idx) {
        if (!std::isdigit(str[idx])) return false;
    }
    return true;
}

bool isNumber(std::string str) {
    std::stringstream ss;
    for (size_t idx = 0; idx < str.size(); ++idx) {
        if (str[idx] != ' ' && str[idx] != '\t') ss << str[idx];
    }
    str = ss.str();
    if (str.size() == 0) return false;
    size_t idx = 0;
    bool isComma = false;
    if (str[0] == '-') ++idx;
    for (;idx < str.size(); ++idx) {
        if (!std::isdigit(str[idx])) {
            if (str[idx] == '.' && isComma) {
                return false;
            } else if (str[idx] == '.') {
                isComma = true;
            }
        }
    }
    return true;
}

template<typename T>
T parseInteger(std::string str) {
    std::stringstream ss;
    for (size_t idx = 0; idx < str.size(); ++idx) {
        if (str[idx] != ' ' && str[idx] != '\t') ss << str[idx];
    }
    str = ss.str();
    unsigned long long result = 0; // потому что не знаем куда влезет
    bool is_negative = false;
    size_t idx = 0;
    if (str[0] == '-') {
        is_negative = true;
        ++idx;
    }
    for (;idx < str.size(); ++idx) {
        int digit = str[idx] - '0';
        result *= 10;
        result += digit;
    }
    if (!is_negative) {
        if (result > std::numeric_limits<T>::max()) { // проверка что влезаем в тип T https://en.cppreference.com/w/cpp/types/numeric_limits
            throw OverflowException(result);
        }
        return result;
    }
    else {
        // тут раньше было -result < numeric_limits<T>min() но вроде бред
        if (result > std::numeric_limits<T>::max()) {
            throw OverflowException(result);
        }
        return -result;
    }
}

double parseDouble(std::string str) {
    std::stringstream ss;
    for (size_t idx = 0; idx < str.size(); ++idx) {
        if (str[idx] != ' ' && str[idx] != '\t') ss << str[idx];
    }
    str = ss.str();
    double result = 0;
    double drob_part = 0;
    bool is_negative = false;
    bool dotFlag = false;
    int afterDotCounter = 0;
    size_t idx = 0;
    if (str[0] == '-') {
        is_negative = true;
        ++idx;
    }
    for (;idx < str.size(); ++idx) {
        if (str[idx] == '.') {
            dotFlag = true;
            continue;
        }
        if (!dotFlag) {
            int digit = str[idx] - '0';
            result *= 10;
            result += digit;
        } else {
            ++afterDotCounter;
            int digit = str[idx] - '0';
            drob_part += digit / std::pow(10, afterDotCounter);
        }
    }
    return (result + drob_part) * (is_negative ? -1 : 1);
}