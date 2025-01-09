#pragma once
#include "Exceptions.hpp"
#include <string>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <cstring>   
#include <sstream>

// #include "CompRationalArithmetic.hpp"
/**
 * @brief Represents a rational number with a numerator and a denominator.
 * 
 * @tparam numerator_T The type of the numerator, typically an integer type.
 */
template <typename numerator_T>
class Rational {
public:
    /** @brief The numerator of the rational number. */
    numerator_T _numerator;

    /** @brief The denominator of the rational number. */
    unsigned long long _denominator;
    
    Rational() : _numerator(0), _denominator(1) {}
    
    /**
     * @brief Constructs a rational number from a numerator and a denominator.
     * 
     * @param numerator The numerator of the rational number.
     * @param denominator The denominator of the rational number. Must not be zero.
     * @throws DivisionByZeroException If the denominator is zero.
     */
    Rational(numerator_T numerator, unsigned denominator) {
        if (denominator == 0)
            throw DivisionByZeroException(numerator);
        if (numerator == 0) {
            _numerator = numerator;
            _denominator = 1;
        } else {
            _numerator = numerator;
            _denominator = denominator;
        }
        
    }

    /**
     * @brief Constructs a rational number with the given numerator and a denominator of 1.
     * 
     * @param numerator The numerator of the rational number.
     */
    Rational(numerator_T numerator) {
        _numerator = numerator;
        _denominator = 1;
    }

    /**
     * @brief Checks if the given string represents an integer.
     * 
     * @param str The string to check.
     * @return true If the string represents an integer.
     * @return false Otherwise.
     */
    bool isInteger(const char* str) {
        if (!str || *str == '\0') {
            return false;
        }
        if (*str == '-' || *str == '+') {
            ++str;
        }
        while (*str) {
            if (!std::isdigit(*str)) {
                return false;
            }
            ++str;
        }
        return true;
    }
    
    /**
     * @brief Constructs a rational number from a string representation.
     * 
     * The string can be either a single integer (e.g., "5") or a fraction (e.g., "5/7").
     * 
     * @param str The string representation of the rational number.
     * @throws DivisionByZeroException If the denominator is zero.
     * @throws InvalidArgument If the string is not a valid integer or fraction.
     */
    explicit Rational(const char* str) {
        const char *slash = std::strchr(str, '/');
        if (slash) {
            std::string numerator(str, slash - str);
            if (!isInteger(numerator.c_str())) {
                throw InvalidArgument("Numerator must be an integer.");
            }
            _numerator = static_cast<numerator_T>(std::stoll(numerator));
            std::string denominator(slash + 1);
            if (denominator.size() == 0) {
                throw InvalidArgument("Input must be in the form of 'numerator/denominator' or an integer.");
            }
            if (!isInteger(denominator.c_str())) {
                throw InvalidArgument("Denominator must be an unsigned integer.");
            }
            if (denominator[0] == '-') {
                throw InvalidArgument("Denominator must be an unsigned integer.");
            }
            unsigned long long temp = static_cast<unsigned long long>(std::stoull(denominator));
            if (temp == 0) {
                throw DivisionByZeroException(_numerator);
            }
            if (_numerator == 0) {
                _denominator = 1;
            } else {
                _denominator = temp;
            }
        } else {
            std::string numerator(str);
            if (!isInteger(numerator.c_str())) {
                throw InvalidArgument("Input must be in the form of 'numerator/denominator' or an integer.");
            }
            _numerator = static_cast<numerator_T>(std::stoll(str));
            _denominator = 1;
        }
    }
 
    /**
     * @brief Computes the greatest common divisor (GCD) of two numbers.
     * 
     * This function calculates the GCD of two numbers using the Euclidean algorithm.
     * The GCD is always a positive number.
     * 
     * @tparam numerator_T The type of the first number, typically an integer type.
     * @param a The first number (can be negative or positive).
     * @param b The second number (must be non-negative).
     * @return numerator_T The greatest common divisor of the two numbers.
     */
    static numerator_T gcd(numerator_T a, unsigned long long b) {
    if (a == 0 && b == 0) {
        throw InvalidArgument("GCD of 0 and 0 is undefined.");
    }
    if (b == 0) {
        return a < 0 ? -a : a;  // Возвращаем модуль a
    }
    a = a < 0 ? -a : a;  // Берём модуль a
    b = b < 0 ? -b : b;  // Берём модуль b
    while (b != 0) {
        numerator_T t = a % b;
        a = b;
        b = t;
    }
    return a;
}
    
    /**
     * @brief Converts the rational number to its canonical form.
     * 
     * A rational number is in canonical form if its numerator and denominator are 
     * reduced to their smallest values while maintaining equivalence.
     * For example, "4/6" is reduced to "2/3".
     * 
     * @return Rational<numerator_T> A new Rational object in canonical form.
     */
    Rational make_canonical() const
    {
        if (_numerator == 0) return Rational<numerator_T>(0, 1);
        numerator_T nod = gcd(_numerator, _denominator);
        return Rational<numerator_T>(_numerator / nod, _denominator / nod);
    }
    
    /**
     * @brief Converts the rational number to an integer.
     * 
     * The conversion truncates any fractional part, effectively performing floor division
     * for positive numbers and truncation for negative numbers.
     * 
     * @return int The integer representation of the rational number.
     */
    operator int() const {
        return static_cast<int>(_numerator / static_cast<numerator_T>(_denominator));
    }

    /**
     * @brief Converts the rational number to a double.
     * 
     * The conversion divides the numerator by the denominator as floating-point numbers.
     * 
     * @return double The double representation of the rational number.
     */
    operator double() const {
        return static_cast<double>(_numerator) / static_cast<double>(_denominator);
    }

    /**
     * @brief Rounds the rational number down to the nearest integer.
     * 
     * This method uses integer division to calculate the largest integer less than
     * or equal to the rational number.
     * 
     * @return numerator_T The largest integer less than or equal to the rational number.
     */
    numerator_T floor() const {
    if (_numerator < 0 && _numerator % static_cast<numerator_T>(_denominator) != 0) {
        return _numerator / static_cast<numerator_T>(_denominator) - 1;
    }
    return _numerator / static_cast<numerator_T>(_denominator);
}

    /**
     * @brief Rounds the rational number to the nearest integer.
     * 
     * This method adds 0.5 to the rational number for positive numbers, subtracts
     * 0.5 for negative numbers, and then truncates the result.
     * 
     * @return numerator_T The nearest integer to the rational number.
     */
    numerator_T round() const {
        if (_numerator >= 0) {
            return static_cast<numerator_T>(_numerator + _denominator / 2) / static_cast<numerator_T>(_denominator);
        } else {
            return static_cast<numerator_T>(_numerator - _denominator / 2) / static_cast<numerator_T>(_denominator);
        }
    }
    
    /**
     * @brief Converts the rational number to its string representation.
     * 
     * The string representation is in the form "numerator/denominator".
     * For example, a rational number with numerator 3 and denominator 4
     * will be represented as "3/4".
     * 
     * @return std::string The string representation of the rational number.
     */
    std::string to_string() const {
        std::stringstream ss;
        ss << _numerator << '/' << _denominator;
        return ss.str();
    }

    Rational<numerator_T> operator-() const {
        return Rational(-_numerator, _denominator);
    }

    template <typename other_T>
    bool operator==(const Rational<other_T>& other) const {
        Rational<other_T> other_canon = other.make_canonical();
        Rational<numerator_T> this_canon = this->make_canonical();
        return (this_canon._numerator == other_canon._numerator) && (this_canon._denominator == other_canon._denominator);
    }

    template <typename other_T>
    bool operator!=(const Rational<other_T>& other) const {
        return !(*this == other);
    }

    template <typename other_T>
    bool operator<(const Rational<other_T>& other) const {
        using common_type = std::common_type_t<numerator_T, other_T>;
        common_type left = static_cast<common_type>(_numerator) * static_cast<common_type>(other._denominator);
        common_type right = static_cast<common_type>(_denominator) * static_cast<common_type>(other._numerator);
        return left < right;
    }

    template <typename other_T>
    bool operator<=(const Rational<other_T>& other) const {
        using common_type = std::common_type_t<numerator_T, other_T>;
        common_type left = static_cast<common_type>(_numerator) * static_cast<common_type>(other._denominator);
        common_type right = static_cast<common_type>(_denominator) * static_cast<common_type>(other._numerator);
        return left <= right;
    }

    template <typename other_T>
    bool operator>(const Rational<other_T>& other) const {
        using common_type = std::common_type_t<numerator_T, other_T>;
        common_type left = static_cast<common_type>(_numerator) * static_cast<common_type>(other._denominator);
        common_type right = static_cast<common_type>(_denominator) * static_cast<common_type>(other._numerator);
        return left > right;
    }

    template <typename other_T>
    bool operator>=(const Rational<other_T>& other) const {
        using common_type = std::common_type_t<numerator_T, other_T>;
        common_type left = static_cast<common_type>(_numerator) * static_cast<common_type>(other._denominator);
        common_type right = static_cast<common_type>(_denominator) * static_cast<common_type>(other._numerator);
        return left >= right;
    }

    template <typename other_T>
    Rational operator+(const Rational<other_T>& other) const {
        numerator_T new_numerator = _numerator * other._denominator + 
                                    other._numerator * _denominator;
        unsigned long long new_denominator = _denominator * other._denominator;
        return Rational<numerator_T>(new_numerator, new_denominator).make_canonical();
    }

    template <typename other_T>
    Rational<numerator_T> operator-(const Rational<other_T>& other) const {
        numerator_T new_numerator = (_numerator) * other._denominator - 
                                    (other._numerator) * _denominator;
        unsigned long long new_denominator = _denominator * other._denominator;
        return Rational<numerator_T>(new_numerator, new_denominator).make_canonical();
    }

    template <typename other_T>
    Rational<numerator_T> operator*(const Rational<other_T>& other) const {
        numerator_T new_numerator = (_numerator) * (other._numerator);
        unsigned long long new_denominator = _denominator * other._denominator;
        return Rational<numerator_T>(new_numerator, new_denominator).make_canonical();
    }

    template <typename other_T>
    Rational<numerator_T> operator/(const Rational<other_T>& other) const {
        if (other._numerator == 0) {
            throw DivisionByZeroException(_numerator);
        }
        numerator_T new_numerator = (_numerator) * (other._denominator);
        numerator_T new_denominator = (_denominator) * (other._numerator);
        if (new_denominator < 0) {
            new_numerator = -new_numerator;
            new_denominator = -new_denominator;
        }
        return Rational<numerator_T>(new_numerator, new_denominator).make_canonical();
    }

    friend Rational abs(const Rational& r) {
        return Rational(std::abs(r._numerator), r._denominator);
    }
};

template <typename Numeric, typename numerator_T>
std::enable_if_t<std::is_arithmetic<Numeric>::value, Rational<std::common_type_t<numerator_T, Numeric>>>
operator+(const Numeric& value, const Rational<numerator_T>& rational) {
    using common_type = std::common_type_t<numerator_T, Numeric>;
    return Rational<common_type>(static_cast<common_type>(value), 1) + rational;
}

template <typename Numeric, typename numerator_T>
std::enable_if_t<std::is_arithmetic<Numeric>::value, Rational<std::common_type_t<numerator_T, Numeric>>>
operator-(const Numeric& value, const Rational<numerator_T>& rational) {
    using common_type = std::common_type_t<numerator_T, Numeric>;
    return Rational<common_type>(static_cast<common_type>(value), 1) - rational;
}

template <typename Numeric, typename numerator_T>
std::enable_if_t<std::is_arithmetic<Numeric>::value, Rational<std::common_type_t<numerator_T, Numeric>>>
operator*(const Numeric& value, const Rational<numerator_T>& rational) {
    using common_type = std::common_type_t<numerator_T, Numeric>;
    return Rational<common_type>(static_cast<common_type>(value), 1) * rational;
}

template <typename Numeric, typename numerator_T>
std::enable_if_t<std::is_arithmetic<Numeric>::value, Rational<std::common_type_t<numerator_T, Numeric>>>
operator/(const Numeric& value, const Rational<numerator_T>& rational) {
    using common_type = std::common_type_t<numerator_T, Numeric>;
    return Rational<common_type>(static_cast<common_type>(value), 1) / rational;
}

template <typename numerator_T>
std::ostream& operator<<(std::ostream& os, const Rational<numerator_T>& obj) {
    os << obj.to_string();
    return os;
}

// RATIONAL AND BASIC TYPES

template <typename int_t, typename type_int>
bool operator==(const Rational<int_t> &r, const type_int &a)
{
    return r == Rational(a);
}

template <typename int_t, typename type_int>
bool operator!=(const Rational<int_t> &r, const type_int &a)
{
    return r != Rational(a);
}

template <typename int_t, typename type_int>
bool operator<(const Rational<int_t> &r, const type_int &a)
{
    return r <= Rational(a);
}

template <typename int_t, typename type_int>
bool operator<=(const Rational<int_t> &r, const type_int &a)
{
    return r <= Rational(a);
}

template <typename int_t, typename type_int>
bool operator>(const Rational<int_t> &r, const type_int &a)
{
    return r > Rational(a);
}

template <typename int_t, typename type_int>
bool operator>=(const Rational<int_t> &r, const type_int &a)
{
    return r >= Rational(a);
}


// Rational and numeric
template <typename Numeric, typename numerator_T>
std::enable_if_t<std::is_arithmetic<Numeric>::value, Rational<std::common_type_t<numerator_T, Numeric>>>
operator+(const Rational<numerator_T>& rational, const Numeric& value) {
    using common_type = std::common_type_t<numerator_T, Numeric>;
    return Rational<common_type>(static_cast<common_type>(value), 1) + rational;
}

template <typename Numeric, typename numerator_T>
std::enable_if_t<std::is_arithmetic<Numeric>::value, Rational<std::common_type_t<numerator_T, Numeric>>>
operator-(const Rational<numerator_T>& rational, const Numeric& value) {
    using common_type = std::common_type_t<numerator_T, Numeric>;
    return rational - Rational<common_type>(static_cast<common_type>(value), 1);
}

template <typename Numeric, typename numerator_T>
std::enable_if_t<std::is_arithmetic<Numeric>::value, Rational<std::common_type_t<numerator_T, Numeric>>>
operator*(const Rational<numerator_T>& rational, const Numeric& value) {
    using common_type = std::common_type_t<numerator_T, Numeric>;
    return Rational<common_type>(static_cast<common_type>(value), 1) * rational;
}

template <typename Numeric, typename numerator_T>
Rational<numerator_T> operator/(const Rational<numerator_T>& rational, const Numeric& value) {
    return rational / Rational<numerator_T>(value);
}