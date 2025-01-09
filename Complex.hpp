#pragma once
#include <cmath>
#include "Exceptions.hpp"
#include "Rational.hpp"

template <typename real_T = double, typename image_T = real_T>
class Complex {
public:
    real_T real;
    image_T image;
    /**
     * @brief Constructor to initialize a complex number with specified real and imaginary parts.
     * @param r The real part of the complex number.
     * @param i The imaginary part of the complex number.
     */
    Complex(real_T r, image_T i) : real(r), image(i) {}
    /**
     * @brief Constructor to initialize a complex number with a specified real part and a default imaginary part of 0.
     * @param r The real part of the complex number.
     */
    Complex(real_T r) : real(r), image(0) {}
    /**
     * @brief Default constructor to initialize a complex number with both real and imaginary parts set to 0.
     */
    Complex() : real(0), image(0) {}
    
    /**
     * @brief Checks if a given string represents a valid numeric value.
     * @param str A C-style string to validate as a number.
     * @return True if the string represents a valid number; otherwise, false.
     * 
     * @details The function supports both integer and floating-point representations. 
     * It handles optional '+' or '-' signs and ensures there are digits before and/or after the decimal point.
     */
    bool isNumber(const char* str) {
        if (!str || *str == '\0') {
            return false; 
        }
        const char* p = str;
        if (*p == '+' || *p == '-') {
            ++p;
        }
        bool has_digits = false;
        while (std::isdigit(*p)) {
            has_digits = true;
            ++p;
        }
        if (*p == '.') {
            ++p;
            
            while (std::isdigit(*p)) {
                has_digits = true;
                ++p;
            }
        }
        return has_digits && *p == '\0';
    }
    
    /**
     * @brief Constructs a complex number from a string representation.
     * @param str A C-style string representing the complex number.
     * 
     * @throws InvalidArgument If the string does not represent a valid complex number.
     * 
     * @details The input can be in the following formats:
     * - `real+imaginary*i` or `real-imaginary*i`
     * - `real,imaginary` (comma-separated format)
     * - A single real number without an imaginary part.
     */
    explicit Complex(const char* str) {
        const char* comma = std::strchr(str, ',');
        if (comma) {
            
            std::string real_part(str, comma - str);
            if (real_part.empty() || !isNumber(real_part.c_str())) {
                throw InvalidArgument("Real part must be a valid number.");
            }
            real = static_cast<real_T>(std::stod(real_part));

            std::string imaginary_part(comma + 1);
            if (imaginary_part.empty() || !isNumber(imaginary_part.c_str())) {
                throw InvalidArgument("Imaginary part must be a valid number.");
            }
            image = static_cast<image_T>(std::stod(imaginary_part));
            return;
        }
        const char* plus = std::strchr(str, '+');
        const char* minus = std::strchr(str + 1, '-');
        const char* imaginary_marker = std::strchr(str, 'i');

        if (!plus && !minus && !imaginary_marker) {
            
            if (!isNumber(str)) {
                throw InvalidArgument("Input must be a valid number or in the form of 'real+imaginary*i', 'real-imaginary*i', or 'real,imaginary'.");
            }
            real = static_cast<real_T>(std::stod(str));
            image = static_cast<image_T>(0);
            return;
        }

        if (!imaginary_marker || (plus && minus)) {
            throw InvalidArgument("Input must be in the form of 'real+imaginary*i', 'real-imaginary*i', or 'real,imaginary'.");
        }

        const char* separator = plus ? plus : minus;
        if (!separator || separator > imaginary_marker) {
            throw InvalidArgument("Input must be in the form of 'real+imaginary*i', 'real-imaginary*i', or 'real,imaginary'.");
        }

        std::string real_part(str, separator - str);
        if (!isNumber(real_part.c_str())) {
            throw InvalidArgument("Real part must be a valid number.");
        }
        real = static_cast<real_T>(std::stod(real_part));

        std::string imaginary_part(separator, imaginary_marker - separator);
        if (imaginary_part.empty() || !isNumber(imaginary_part.c_str())) {
            throw InvalidArgument("Imaginary part must be a valid number.");
        }
        image = static_cast<image_T>(std::stod(imaginary_part));
    }
    
    /**
     * @brief Computes the modulus (absolute value) of the complex number.
     * @return The modulus of the complex number as a `double`.
     * 
     * @details The modulus is calculated as √(real² + imaginary²).
     */ 
    double modulus() const {
        return std::sqrt(real * real + image * image);
    }
    
    std::string to_string() const {
        std::ostringstream oss;
        if (image == 0 && real == 0) {
            return "0";
        }
        if (image == 0) {
            oss << real;
            return oss.str();
        }
        if (real == 0) {
            oss << image << "i";
            return oss.str();
        }
        oss << real << (image < 0 ? " - " : " + ") << (image < 0 ? -image : image) << "i";
        return oss.str();
    }
    
    template<typename other_real, typename other_image>
    bool operator<(const Complex<other_real, other_image> c) const {
        return this->modulus() < c.modulus();
    }

    template<typename other_real, typename other_image>
    bool operator<=(const Complex<other_real, other_image>& c) const {
        return this->modulus() <= c.modulus();
    }

    template<typename other_real, typename other_image>
    bool operator>(const Complex<other_real, other_image>& c) const {
        return this->modulus() > c.modulus();
    }

    template<typename other_real, typename other_image>
    bool operator>=(const Complex<other_real, other_image>& c) const {
        return this->modulus() >= c.modulus();
    }
 
    template<typename other_real, typename other_image>
    bool operator==(const Complex<other_real, other_image>& c) const {
        return real == c.real && image == c.image;
    }
 
    template<typename other_real, typename other_image>
    bool operator!=(const Complex<other_real, other_image>& c) const {
        return (real != c.real) || (image != c.image);
    }
 
    // template<typename other_real, typename other_image>
    // Complex operator+(const Complex<other_real, other_image>& c) const {
    //     return Complex(real + c.real, image + c.image);
    // }

    // template<typename other_real, typename other_image>
    // Complex operator-(const Complex<other_real, other_image>& c) const {
    //     return Complex(real - c.real, image - c.image);
    // }
   
    // template<typename other_real, typename other_image>
    // Complex operator*(const Complex<other_real, other_image>& c) const {
    //     return Complex(real * c.real - image * c.image, real * c.image + image * c.real);
    // }
 
    // template<typename other_real, typename other_image>
    // Complex operator/(const Complex<other_real, other_image>& c) const {
    //     double denom = c.real * c.real + c.image * c.image;
    //     if (denom == 0) throw DivisionByZeroException(this->to_string());
    //     return Complex<>((real * c.real + image * c.image) / denom,
    //                    (image * c.real - real * c.image) / denom);
    // }

    // template<typename number_T>
    // Complex<std::common_type_t<real_T, number_T>, std::common_type_t<image_T, number_T>> 
    // operator+(number_T value) const {
    //     using common_real = std::common_type_t<real_T, number_T>;
    //     using common_image = std::common_type_t<image_T, number_T>;
    //     return Complex<common_real, common_image>(static_cast<common_real>(real + value), static_cast<common_image>(image));
    // }

    // template<typename number_T>
    // Complex<std::common_type_t<real_T, number_T>, std::common_type_t<image_T, number_T>> 
    // operator-(number_T value) const {
    //     using common_real = std::common_type_t<real_T, number_T>;
    //     using common_image = std::common_type_t<image_T, number_T>;
    //     return Complex<common_real, common_image>(static_cast<common_real>(real - value), static_cast<common_image>(image));
    // }

    // template<typename number_T>
    // Complex<std::common_type_t<real_T, number_T>, std::common_type_t<image_T, number_T>> 
    // operator*(number_T value) const {
    //     using common_real = std::common_type_t<real_T, number_T>;
    //     using common_image = std::common_type_t<image_T, number_T>;
    //     return Complex<common_real, common_image>(static_cast<common_real>(real * value), static_cast<common_image>(image * value));
    // }

    // template<typename number_T>
    // Complex<std::common_type_t<real_T, number_T>, std::common_type_t<image_T, number_T>> 
    // operator/(number_T value) const {
    //     using common_real = std::common_type_t<real_T, number_T>;
    //     using common_image = std::common_type_t<image_T, number_T>;
    //     return Complex<common_real, common_image>(static_cast<common_real>(real / value), static_cast<common_image>(image / value));
    // }

    friend double abs(const Complex& r) {
        return r.modulus();
    }
};


// COMPLEX AND COMPLEX


template <typename real_T, typename image_T, typename other_real, typename other_image>
Complex<real_T, image_T> operator+(const Complex<real_T, image_T>& lhs, const Complex<other_real, other_image>& rhs) {
    return Complex<real_T, image_T>(lhs.real + rhs.real, lhs.image + rhs.image);
}

template <typename real_T, typename image_T, typename other_real, typename other_image>
Complex<real_T, image_T> operator-(const Complex<real_T, image_T>& lhs, const Complex<other_real, other_image>& rhs) {
    return Complex<real_T, image_T>(lhs.real - rhs.real, lhs.image - rhs.image);
}

template <typename real_T, typename image_T, typename other_real, typename other_image>
Complex<real_T, image_T> operator*(const Complex<real_T, image_T>& lhs, const Complex<other_real, other_image>& rhs) {
    return Complex<real_T, image_T>(
        lhs.real * rhs.real - lhs.image * rhs.image,
        lhs.real * rhs.image + lhs.image * rhs.real
    );
}

template <typename real_T, typename image_T, typename other_real, typename other_image>
Complex<real_T, image_T> operator/(const Complex<real_T, image_T>& lhs, const Complex<other_real, other_image>& rhs) {
    auto denom = static_cast<double>(rhs.real * rhs.real + rhs.image * rhs.image);
    if (denom == 0) throw DivisionByZeroException(lhs.to_string());
    return Complex<real_T, image_T>(
        static_cast<real_T>((lhs.real * rhs.real + lhs.image * rhs.image) / denom),
        static_cast<image_T>((lhs.image * rhs.real - lhs.real * rhs.image) / denom)
    );
}

// COMPLEX AND RATIONAL


template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, type_imag> operator+(const Complex<type_real, type_imag>& c, const Rational<int_t>& a) {
    return Complex(a + Rational<int_t>(c.real), c.image);
}

template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, type_imag> operator-(const Complex<type_real, type_imag>& c, const Rational<int_t>& a) {
    return Complex(Rational<int_t>(c.real) - a, c.image);
}

template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, Rational<int_t>> operator*(const Complex<type_real, type_imag>& c, const Rational<int_t>& a) {
    return Complex(a * Rational<int_t>(c.real), a * Rational<int_t>(c.image));
}

template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, Rational<int_t>> operator/(const Complex<type_real, type_imag>& c, const Rational<int_t>& a) {
    if (a == 0) {
        throw DivisionByZeroException(c);
    }
    return Complex(Rational<int_t>(c.real) / a, Rational<int_t>(c.image) / a);
}

template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, type_imag> operator+(const Rational<int_t>& a, const Complex<type_real, type_imag>& c) {
    return Complex(a + Rational<int_t>(c.real), c.image);
}

template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, type_imag> operator-(const Rational<int_t>& a, const Complex<type_real, type_imag>& c) {
    return Complex(a - Rational<int_t>(c.real), c.image);
}

template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, Rational<int_t>> operator*(const Rational<int_t>& a, const Complex<type_real, type_imag>& c) {
    return Complex(a * Rational<int_t>(c.real), a * Rational<int_t>(c.image));
}

template <typename type_real, typename type_imag, typename int_t>
Complex<Rational<int_t>, Rational<int_t>> operator/(const Rational<int_t>& a, const Complex<type_real, type_imag>& c) {
    return Complex<Rational<int_t>>(a, 0) / Complex<Rational<int_t>>(c.real, c.image);
}

// COMPLEX AND OTHER

template <typename type_real, typename type_imag, typename operand_t>
Complex<type_real, type_imag> operator+(Complex<type_real, type_imag> c, const operand_t& a) {
    return Complex(c.real + a, c.image);
}

template <typename type_real, typename type_imag, typename operand_t>
Complex<type_real, type_imag> operator-(Complex<type_real, type_imag> c, const operand_t& a)  {
    return Complex(c.real - a, c.image);
}

template <typename type_real, typename type_imag, typename operand_t>
Complex<type_real, type_imag> operator*(Complex<type_real, type_imag> c, const operand_t& a) {
    return Complex(c.real * a, c.image * a);
}

template <typename type_real, typename type_imag, typename operand_t>
Complex<type_real, type_imag> operator/(Complex<type_real, type_imag> c, const operand_t& a) {
    if (a == 0) {
        throw DivisionByZeroException("Cannot divide by zero.");
    }
    return Complex(c.real / a, c.image / a);
}

template <typename real_T, typename image_T>
std::ostream& operator<<(std::ostream& os, const Complex<real_T, image_T>& obj) {
    os << obj.to_string();
    return os;
}

