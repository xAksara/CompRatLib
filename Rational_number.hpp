#pragma once

#include <type_traits>
#include <string>
#include <cctype> // для isdigit
#include <limits> // для numeric_limits
#include <sstream>
#include "Exceptions.hpp"
#include "Utils.hpp"



// // костыли для арифметики знаковых и беззнаковых
// template<typename T1, typename T2>
// long long mul_for_normal_people_not_for_cpp_mainers(T1 a, T2 b) {
//     bool isNegative = false;
//     if (a < 0) {
//         isNegative = true;
//         a = -a;
//     }
//     return isNegative ?  -(a * b) : a * b; 
// }

// template<typename T1, typename T2>
// long long sum_for_normal_people_not_for_cpp_mainers(T1 a, T2 b) {
//     bool isNegative = false;
//     if (a < 0) {
//         isNegative = true;
//         a = -a;
//     }
//     return isNegative ?  -(a + b) : a + b; 
// }
// #define sum sum_for_normal_people_not_for_cpp_mainers

// template<typename T1, typename T2>
// long long sub_for_normal_people_not_for_cpp_mainers(T1 a, T2 b) {
//     bool isNegative_a = false;
//     bool isNegative_b = false;
//     if (a < 0) {
//         isNegative_a = true;
//         a = -a;
//     }
//     if (b < 0) {
//         isNegative_b = true;
//         b = -b;
//     }
//     long long result;
//     if (isNegative_a && isNegative_b) {
//         result = (a > b ? -(a - b) : (a - b));
//     }
//     if (!isNegative_a && !isNegative_b) {
//         result = (a > b ? (a - b) : -(a - b));
//     }
//     if (!isNegative_a && isNegative_b) {
//         result = a + b;
//     }
//     if (isNegative_a && !isNegative_b) {
//         result = -(a + b);
//     }

//     return result;
// }
// #define sub sub_for_normal_people_not_for_cpp_mainers



template<typename num_type = long long>
class Rational_number {
    using denom_type = std::make_unsigned_t<num_type>; // unsigned num_type нельзя, поэтому делаем через make_unsigned_t (https://en.cppreference.com/w/cpp/types/make_unsigned)
    num_type num;
    denom_type denom;
private:
    num_type gcd(num_type a, denom_type b) const {
        a = a < 0 ? -a : a;
        // знак одинаковый
        if (a == b)
            return a;
        if (a < b)
        {
            denom_type t;
            t = a;
            a = b;
            b = t;
        }
        denom_type t = a % b;
        while (t)
        {
            a = b;
            b = t;
            t = a % b;
        }
        return b;
    }

public:
    Rational_number(num_type number) : num(number), denom(1) { }

    Rational_number(num_type numerator, denom_type denominator) {
        if (denominator == 0) {
            throw DivisionByZeroException(numerator, denominator);
        }
        num = numerator;
        denom = (num == 0) ? 1 : denominator;
    }

    Rational_number(std::string numStr) {
        size_t slash_idx = numStr.find('/');
        if (slash_idx == std::string::npos) {
            // значит целое число
            if (!isInteger(numStr)) {
                throw InvalidArgumentException("Строка должна быть вида \"123/123\" или  \"123\". Передано: ", numStr);
            }
            num = parseInteger<num_type>(numStr);
            denom = 1;
        } else {
            std::string numerator_str(numStr.begin(), numStr.begin() + slash_idx);
            std::string denominator_str(numStr.begin() + slash_idx + 1, numStr.end());
            if (!isInteger(numerator_str) || !isInteger(denominator_str)) {
                throw InvalidArgumentException("Строка должна быть вида \"123/123\" или  \"123\". Передано: ", numStr);
            }
            num = parseInteger<num_type>(numerator_str);
            denom = parseInteger<denom_type>(denominator_str);
            if (denom == 0) {
                throw DivisionByZeroException(numerator_str, denominator_str);
            }
        }
    }

    Rational_number() : num(0), denom(1) { }

    num_type getNumerator() const { return num; }

    denom_type getDenominator() const { return denom; }

    void setNumerator(num_type numerator) { num = numerator; }

    void setDenominator(denom_type denominator) { 
        if (denominator == 0) {
            throw DivisionByZeroException(num, denominator);
        }
        denom = denominator; 
    }

    Rational_number make_canonical() {
        if (num == 0) return Rational_number(0);
        num_type nod = gcd(num, denom); // num_type потому что НОД не больше числителя по модулю, и для корректности операций должен быть знаковым
        return Rational_number(num / nod, denom / nod);
    }

    operator int() const {
        num_type local_num = this->num;
        bool isNegative = false;
        if (local_num < 0) {
            isNegative = true;
            local_num = -local_num; // чтобы не сломалась арфиметика знаковых и беззнаковых
        }
        unsigned long long div = local_num / denom;
        if (!isNegative) {
            if (div > std::numeric_limits<int>::max())
            {
                throw OverflowException(div);
            }
            return div;
        }
        else {
            if (div > std::numeric_limits<int>::max())
            {
                throw OverflowException(div);
            }
            return -static_cast<int>(div);
        }
    }

    operator double() const {
        // Не справился с проверкой на переполнение. Double устроен куда сложнее целочисленных 
        return static_cast<double>(num) / denom;
        
    }

    num_type round() const {
        num_type local_num = this->num;
        bool isNegative = false;
        if (local_num < 0) {
            isNegative = true;
            local_num = -local_num;
        }
        num_type int_part = local_num / denom;
        num_type drob_part = local_num % denom;
        // num_type потому что оно точно положительное и сравнение корректно
        if (isNegative) {
            if (drob_part * 2 >= denom) return -int_part;
            return -int_part + 1;
        } else {
            if (drob_part * 2 >= denom) {
                return int_part + 1;
            }
            return int_part;
        }
    }

    num_type floor() const {
        num_type local_num = this->num;
        bool isNegative = false;
        if (local_num < 0) {
            isNegative = true;
            local_num = -local_num;
        }
        num_type int_part = local_num / denom;
        num_type drob_part = local_num % denom;
        if (isNegative) {
            return -int_part + (drob_part == 0 ? 0 : -1);
        } else {
            return int_part;
        }
    }

    // template<typename rhs_num>
    // Rational_number operator+(const Rational_number<rhs_num>& other) const {
    //     return Rational_number(num * static_cast<num_type>(other.getDenominator()) +
    //                              other.getNumerator() * static_cast<num_type>(denom),
    //                              denom*other.getDenominator()).make_canonical();
    // }

    // template<typename rhs_num>
    // Rational_number operator-(const Rational_number<rhs_num>& other) const {
    //     return Rational_number(num * static_cast<num_type>(other.getDenominator()) -
    //                              other.getNumerator() * static_cast<num_type>(denom),
    //                              denom*other.getDenominator()).make_canonical();
    // }

    Rational_number operator-() const {
        return Rational_number(-num, denom);
    }

    // template<typename rhs_num>
    // Rational_number operator*(const Rational_number<rhs_num>& other) const {
    //     return Rational_number(num*static_cast<num_type>(other.getNumerator()), denom*static_cast<denom_type>(other.getDenominator())).make_canonical();
    // }

    // template<typename rhs_num>
    // Rational_number operator/(const Rational_number<rhs_num>& other) const {
    //     rhs_num other_num = other.getNumerator();
    //     bool isNegative = false;
    //     if (other_num < 0) {
    //         isNegative = true;
    //         other_num = -other_num;
    //     }

    //     return Rational_number((isNegative ? -num : num) * static_cast<num_type>(other.getDenominator()),
    //                            denom * other_num).make_canonical();
    // }

    // bool operator==(const Rational_number& other) const {
    //     return this->num * static_cast<num_type>(other.denom) == other.num * static_cast<num_type>(denom);
    // }

    // bool operator!=(const Rational_number& other) const {
    //     return this->num * static_cast<num_type>(other.denom) != other.num * static_cast<num_type>(denom);
    // }

    // bool operator<(const Rational_number& other) const {
    //     return this->num * static_cast<num_type>(other.denom) < other.num * static_cast<num_type>(denom);
    // }

    // bool operator<=(const Rational_number& other) const {
    //     return this->num * static_cast<num_type>(other.denom) <= other.num * static_cast<num_type>(denom);
    // }

    // bool operator>(const Rational_number& other) const {
    //     return this->num * static_cast<num_type>(other.denom) > other.num * static_cast<num_type>(denom);
    // }

    // bool operator>=(const Rational_number& other) const {
    //     return this->num * static_cast<num_type>(other.denom) >= other.num * static_cast<num_type>(denom);
    // }

    Rational_number operator++(int) {
        Rational_number res = *this;
        this->num = this->num + static_cast<num_type>(this->denom);
        return res;
    }

    Rational_number& operator++() {
        this->num = this->num + static_cast<num_type>(this->denom);
        return *this;
    }

    Rational_number operator--(int) {
        Rational_number res = *this;
        this->num = this->num - static_cast<num_type>(this->denom);
        return res;
    }

    Rational_number& operator--() {
        this->num = this->num - static_cast<num_type>(this->denom);
        return *this;
    }
};

// Операторы для работы с базовыми типами. Double приводится к int в 
// момент передачи в конструктор либо при умножении.

// Тоже может быть переполнение, но везде проверять - тоже странно, да и 
// будет замедление работы. Если пользователь жалеет байты - пусть ловит 
// UB из-за переполнения беззнаковых

template<typename num_type, typename T>
Rational_number<num_type> operator+(Rational_number<num_type> lhs, T rhs) { 
    return (lhs + Rational_number(rhs)).make_canonical();
}

template<typename num_type, typename T>
Rational_number<num_type> operator-(Rational_number<num_type> lhs, T rhs) { 
    return (lhs - Rational_number(rhs)).make_canonical();
}

template<typename num_type, typename T>
Rational_number<num_type> operator*(Rational_number<num_type> lhs, T rhs) { 
    return (lhs * Rational_number(rhs)).make_canonical();
}

template<typename num_type, typename T>
Rational_number<num_type> operator/(Rational_number<num_type> lhs, T rhs) { 
    return (lhs / Rational_number(rhs)).make_canonical();
}

template<typename num_type, typename T>
Rational_number<num_type> operator+(T lhs, Rational_number<num_type> rhs) { 
    return (Rational_number(lhs) + rhs).make_canonical();
}

template<typename num_type, typename T>
Rational_number<num_type> operator-(T lhs, Rational_number<num_type> rhs) { 
    return (Rational_number(lhs) - rhs).make_canonical();
}

template<typename num_type, typename T>
Rational_number<num_type> operator*(T lhs, Rational_number<num_type> rhs) { 
    return (Rational_number(lhs) * rhs).make_canonical();
}

template<typename num_type, typename T>
Rational_number<num_type> operator/(T lhs, Rational_number<num_type> rhs) { 
    return (Rational_number(lhs) / rhs).make_canonical();
}

// template<typename num_type, typename T>
// Rational_number<num_type> operator-(Rational_number<num_type> lhs, T rhs) { 
//     return Rational_number<num_type>(lhs.getNumerator() - rhs * static_cast<T>(lhs.getDenominator()), lhs.getDenominator()).make_canonical();
// }
// template<typename num_type, typename T>
// Rational_number<num_type> operator-(Rational_number<num_type> lhs, T rhs) { 
//     Rational_number<num_type> r(lhs.getNumerator() - rhs * static_cast<T>(lhs.getDenominator()), lhs.getDenominator()).make_canonical();
//     return r;
// }


// Логические операторы для работы с базовыми типами

template<typename num_type, typename T>
bool operator==(Rational_number<num_type> lhs, T rhs) { 
    return lhs == Rational_number(rhs);
}

template<typename num_type, typename T>
bool operator==(T lhs, Rational_number<num_type> rhs) { 
    return Rational_number(lhs) == rhs;
}

template<typename num_type, typename T>
bool operator!=(Rational_number<num_type> lhs, T rhs) { 
    return lhs != Rational_number(rhs);
}

template<typename num_type, typename T>
bool operator!=(T lhs, Rational_number<num_type> rhs) { 
    return Rational_number(lhs) != rhs;
}

template<typename num_type, typename T>
bool operator<(Rational_number<num_type> lhs, T rhs) { 
    return lhs < Rational_number(rhs);
}

template<typename num_type, typename T>
bool operator<(T lhs, Rational_number<num_type> rhs) { 
    return Rational_number(lhs) < rhs;
}

template<typename num_type, typename T>
bool operator<=(Rational_number<num_type> lhs, T rhs) { 
    return lhs <= Rational_number(rhs);
}

template<typename num_type, typename T>
bool operator<=(T lhs, Rational_number<num_type> rhs) { 
    return Rational_number(lhs) <= rhs;
}

template<typename num_type, typename T>
bool operator>(Rational_number<num_type> lhs, T rhs) { 
    return lhs > Rational_number(rhs);
}

template<typename num_type, typename T>
bool operator>(T lhs, Rational_number<num_type> rhs) { 
    return Rational_number(lhs) > rhs;
}

template<typename num_type, typename T>
bool operator>=(Rational_number<num_type> lhs, T rhs) { 
    return lhs >= Rational_number(rhs);
}

template<typename num_type, typename T>
bool operator>=(T lhs, Rational_number<num_type> rhs) { 
    return Rational_number(lhs) >= rhs;
}


// Работа с другими рациональными числами

template <typename lhs_type, typename rhs_type>
Rational_number<lhs_type> operator+(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return Rational_number<lhs_type>(lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) +
                                 rhs.getNumerator() * static_cast<lhs_type>(lhs.getDenominator()),
                                 lhs.getDenominator()*rhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
Rational_number<lhs_type> operator-(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return Rational_number<lhs_type>(lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) -
                                 rhs.getNumerator() * static_cast<lhs_type>(lhs.getDenominator()),
                                 lhs.getDenominator()*rhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
Rational_number<lhs_type> operator*(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return Rational_number<lhs_type>(lhs.getNumerator()*rhs.getNumerator(), lhs.getDenominator()*rhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
Rational_number<lhs_type> operator/(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    rhs_type rhs_num = rhs.getNumerator();
    rhs_type lhs_num = lhs.getNumerator();
    bool isRhsNegative = false;
    bool isLhsNegative = false;
    if (rhs_num < 0) {
        isRhsNegative = true;
        rhs_num = -rhs_num;
    }
    if (lhs_num < 0) {
        isLhsNegative = true;
        lhs_num = -lhs_num;
    }
    if (isLhsNegative && isRhsNegative || !isLhsNegative && !isRhsNegative) {
        return Rational_number<lhs_type> (lhs_num * rhs.getDenominator(), lhs.getDenominator() * rhs_num);
    } else {
        return Rational_number<lhs_type> (-lhs_num * rhs.getDenominator(), lhs.getDenominator() * rhs_num);
    }
}

template <typename lhs_type, typename rhs_type>
bool operator==(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) == rhs.getNumerator() * static_cast<rhs_type>(lhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
bool operator!=(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) != rhs.getNumerator() * static_cast<rhs_type>(lhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
bool operator<(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) < rhs.getNumerator() * static_cast<rhs_type>(lhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
bool operator<=(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) <= rhs.getNumerator() * static_cast<rhs_type>(lhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
bool operator>(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) > rhs.getNumerator() * static_cast<rhs_type>(lhs.getDenominator());
}

template <typename lhs_type, typename rhs_type>
bool operator>=(const Rational_number<lhs_type>& lhs, const Rational_number<rhs_type>& rhs) {
    return lhs.getNumerator() * static_cast<lhs_type>(rhs.getDenominator()) >= rhs.getNumerator() * static_cast<rhs_type>(lhs.getDenominator());
}


// bool operator!=(const Rational_number& other) const {
//     return this->num * static_cast<num_type>(other.denom) != other.num * static_cast<num_type>(denom);
// }

// bool operator<(const Rational_number& other) const {
//     return this->num * static_cast<num_type>(other.denom) < other.num * static_cast<num_type>(denom);
// }

// bool operator<=(const Rational_number& other) const {
//     return this->num * static_cast<num_type>(other.denom) <= other.num * static_cast<num_type>(denom);
// }

// bool operator>(const Rational_number& other) const {
//     return this->num * static_cast<num_type>(other.denom) > other.num * static_cast<num_type>(denom);
// }

// bool operator>=(const Rational_number& other) const {
//     return this->num * static_cast<num_type>(other.denom) >= other.num * static_cast<num_type>(denom);
// }




// +=, -= и т.д.

template<typename lhs_type, typename rhs_type>
Rational_number<lhs_type>& operator+=(Rational_number<lhs_type>& lhs, const rhs_type& rhs) {
    lhs.setNumerator(lhs.getNumerator() + rhs * static_cast<lhs_type>(lhs.getDenominator()));
    return lhs;
}

template<typename lhs_type, typename rhs_type>
Rational_number<lhs_type>& operator-=(Rational_number<lhs_type>& lhs, const rhs_type& rhs) {
    lhs.setNumerator(lhs.getNumerator() - rhs * static_cast<lhs_type>(lhs.getDenominator()));
    return lhs;
}

template<typename lhs_type, typename rhs_type>
Rational_number<lhs_type>& operator*=(Rational_number<lhs_type>& lhs, const rhs_type& rhs) {
    lhs.setNumerator(lhs.getNumerator() * rhs);
    return lhs;
}

template<typename lhs_type, typename rhs_type>
Rational_number<lhs_type>& operator/=(Rational_number<lhs_type>& lhs, const rhs_type& rhs) {
    rhs_type local_rhs = (rhs < 0 ? -rhs : rhs);
    lhs.setDenominator(lhs.getDenominator() * local_rhs);
    if (rhs < 0) {
        lhs.setNumerator(-lhs.getNumerator());
    }
    return lhs;
}

template<typename num_type>
std::ostream& operator<<(std::ostream& os, const Rational_number<num_type>& r) {
    os << r.getNumerator() << '/' << r.getDenominator();
    return os;
}