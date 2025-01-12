#pragma once
#include <string>
#include "Utils.hpp"
#include "Exceptions.hpp"
#include "Rational_number.hpp"

template<typename type_real = double, typename type_imag = type_real>
class Complex_number {
    type_real real;
    type_imag image;
public:
    Complex_number() : real(0), image(0) {}
    Complex_number(type_real real) : real(real), image(0) {}
    Complex_number(type_real real, type_imag image) : real(real), image(image) {}
    Complex_number(std::string numStr) {
        size_t comma_idx = numStr.find(',');
        if (comma_idx == std::string::npos) {
            // значит только вещественная часть
            if (!isNumber(numStr)) {
                throw InvalidArgumentException("Строка должна быть вида \"123,123\", \"12.3,123\", \"12.3,12.3\", \"123,12.3\", \"12.3\" или  \"123\". Передано: ", numStr);
            }
            real = parseDouble(numStr);
            image = 0;
        } else {
            std::string real_str(numStr.begin(), numStr.begin() + comma_idx);
            std::string image_str(numStr.begin() + comma_idx + 1, numStr.end());
            if (!isNumber(real_str) || !isNumber(image_str)) {
                throw InvalidArgumentException("Строка должна быть вида \"123,123\", \"12.3,123\", \"12.3,12.3\", \"123,12.3\", \"12.3\" или  \"123\". Передано: ", numStr);
            }
            real = parseDouble(real_str);
            image = parseDouble(image_str);
        }
    }

    type_real getReal() const { return real; }
    type_imag getImage() const { return image; }

    void setReal(type_real val) { real = val; }
    void setImage(type_imag val) { image = val; }

    double modulus() {
        return std::sqrt(real * real, image * image);
    }

    template<typename rhs_real, typename rhs_imag>
    Complex_number operator+(const Complex_number<rhs_real, rhs_imag>& rhs) const {
        return Complex_number(real + rhs.getReal(), image + rhs.getImage());
    }

    template<typename rhs_real, typename rhs_imag>
    Complex_number operator-(const Complex_number<rhs_real, rhs_imag>& rhs) const {
        return Complex_number(real - rhs.getReal(), image - rhs.getImage());
    }

    template<typename rhs_real, typename rhs_imag>
    Complex_number operator*(const Complex_number<rhs_real, rhs_imag>& rhs) const {
        return Complex_number(real * rhs.getReal() - image * rhs.getImage(), real * rhs.getImage() + image * rhs.getReal());
    }

    template<typename rhs_real, typename rhs_imag>
    Complex_number operator/(const Complex_number<rhs_real, rhs_imag>& rhs) const {
        type_real denominator = rhs.getReal() * rhs.getReal() + rhs.getImage() * rhs.getImage();
        if (denominator == 0) {
            throw DivisionByZeroException(*this, rhs);
        }
        return Complex_number(
            (real * rhs.getReal() + image * rhs.getImage()) / denominator,
            (image * rhs.getReal() - real * rhs.getImage()) / denominator
        );
    }

    template<typename rhs_real, typename rhs_imag>
    bool operator==(const Complex_number<rhs_real, rhs_imag>& rhs) const {
        return real == rhs.getReal() && image == rhs.getImage();
    }
};


template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator+(const Complex_number<lhs_real, lhs_imag>& lhs, const rhs_type& rhs) {
    return lhs + Complex_number<lhs_real, lhs_imag>(rhs);
}

template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator-(const Complex_number<lhs_real, lhs_imag>& lhs, const rhs_type& rhs) {
    return lhs - Complex_number<lhs_real, lhs_imag>(rhs);
}

template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator*(const Complex_number<lhs_real, lhs_imag>& lhs, const rhs_type& rhs) {
    return lhs * Complex_number<lhs_real, lhs_imag>(rhs);
}

template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator/(const Complex_number<lhs_real, lhs_imag>& lhs, const rhs_type& rhs) {
    return lhs / Complex_number<lhs_real, lhs_imag>(rhs);
}


// Арифметика комплексных и рациональных. 


// Нужно, потому что при попытке, например, сложения 
// GoogleTests.hpp:162:17: error: ambiguous overload for ‘operator+’ (operand types are ‘Complex_number<int, int>’ and ‘Rational_number<int>’)
//   162 |     ASSERT_EQ(c + r, sum1);
//       |               ~ ^ ~
//       |               |   |
//       |               |   Rational_number<int>
//       |               Complex_number<int, int>
// In file included from main.cpp:2:
// Rational_number.hpp:348:27: note: candidate: ‘Rational_number<num_type> operator+(T, Rational_number<num_type>) [with num_type = int; T = Complex_number<int, int>]’
//   348 | Rational_number<num_type> operator+(T lhs, Rational_number<num_type> rhs) {
//       |                           ^~~~~~~~
// In file included from GoogleTests.hpp:6:
// Complex_number.hpp:75:36: note: candidate: ‘Complex_number<lhs_real, lhs_imag> operator+(const Complex_number<lhs_real, lhs_imag>&, const rhs_type&) [with lhs_real = int; lhs_imag = int; rhs_type = Rational_number<int>]’
//    75 | Complex_number<lhs_real, lhs_imag> operator+(const Complex_number<lhs_real, lhs_imag>& lhs, const rhs_type& rhs) {

template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator+(const Complex_number<lhs_real, lhs_imag>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs + Complex_number<lhs_real, lhs_imag>(rhs);
}

template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator-(const Complex_number<lhs_real, lhs_imag>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs - Complex_number<lhs_real, lhs_imag>(rhs);
}

template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator*(const Complex_number<lhs_real, lhs_imag>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs * Complex_number<lhs_real, lhs_imag>(rhs);
}

template<typename lhs_real, typename lhs_imag, typename rhs_type>
Complex_number<lhs_real, lhs_imag> operator/(const Complex_number<lhs_real, lhs_imag>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs / Complex_number<lhs_real, lhs_imag>(rhs);
}




template<typename rhs_real, typename rhs_imag, typename lhs_type>
Complex_number<rhs_real, rhs_imag> operator+(const Rational_number<lhs_type>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    return Complex_number<rhs_real, rhs_imag>(lhs) + rhs;
}

template<typename rhs_real, typename rhs_imag, typename lhs_type>
Complex_number<rhs_real, rhs_imag> operator-(const Rational_number<lhs_type>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    return Complex_number<rhs_real, rhs_imag>(lhs) - rhs;
}

template<typename rhs_real, typename rhs_imag, typename lhs_type>
Complex_number<rhs_real, rhs_imag> operator*(const Rational_number<lhs_type>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    return Complex_number<rhs_real, rhs_imag>(lhs) * rhs;
}

template<typename rhs_real, typename rhs_imag, typename lhs_type>
Complex_number<rhs_real, rhs_imag> operator/(const Rational_number<lhs_type>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    return Complex_number<rhs_real, rhs_imag>(lhs) / rhs;
}


template<typename type_real, typename type_imag>
std::ostream& operator<<(std::ostream& os, const Complex_number<type_real, type_imag>& c) {
    type_imag image = c.getImage();
    os << c.getReal() << (image < 0 ? " - " : " + ") << (image < 0 ? -image : image) << "i";
    return os;
}