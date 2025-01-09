#pragma once

#include <map>
#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "Exceptions.hpp"
#include "Matrix.hpp"
#include "Base.hpp"
#include "TypeChecker.hpp"

    template<typename real, typename image>
    double abs(Complex<real, image> c) {
        return c.modulus();
    }
    template<typename T>
    Rational<T> abs(Rational<T> r) {
        return r._numerator < 0 ? r*(-1) : r;
    }

class VectorBase : public Base {};

template <typename element_T = double, typename container_T = std::map<unsigned int, element_T>>
class Vector : public VectorBase {
private:
    container_T elements;
    unsigned int dimension;
    double epsilon;
    static const element_T zero;

public:
    Vector(unsigned int dimension, double epsilon = 0.0) 
        : dimension(dimension), epsilon(epsilon) {}

    Vector(const Vector& other) 
        : elements(other.elements), dimension(other.dimension), epsilon(other.epsilon) {}

    Vector(Vector&& other) noexcept 
        : elements(std::move(other.elements)), dimension(other.dimension), epsilon(other.epsilon) {}

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            elements = other.elements;
            dimension = other.dimension;
            epsilon = other.epsilon;
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            elements = std::move(other.elements);
            dimension = other.dimension;
            epsilon = other.epsilon;
        }
        return *this;
    }

    auto myAbs(element_T value) {
        if constexpr (is_rational<std::decay_t<element_T>>::value || 
                      is_complex<std::decay_t<element_T>>::value) {
            return abs(value);
        } else {
            return std::abs(value);
        }
    }

    void set(unsigned int index, element_T value) {
        if (index >= dimension) {
            throw OutOfRangeException(index, dimension);
        }
        if (myAbs(value) >= epsilon) {
            elements[index] = value;
        } else {
            elements.erase(index);
        }
    }

    const element_T& get(unsigned int index) const {
        if (index >= dimension) {
            throw OutOfRangeException(index, dimension);
        }
        auto it = elements.find(index);
        return (it != elements.end()) ? it->second : zero;
    }

    const element_T& operator()(unsigned int index) const {
        if (index >= dimension) {
            throw OutOfRangeException(index, dimension);
        }
        auto it = elements.find(index);
        return (it != elements.end()) ? it->second : zero;
    }

    element_T& operator()(unsigned int index) {
        if (index >= dimension) {
            throw OutOfRangeException(index, dimension);
        }
        return elements[index];
    }

    bool operator==(const Vector& other) const {
        if (dimension != other.dimension) {
            return false;
        }
        for (const auto& [index, value] : elements) {
            if (other.elements.find(index) != other.elements.end()) {
                if (value != other.elements.at(index)) {
                    return false;
                }
            } else if (value != 0) {
                return false;
            }
        }
        for (const auto& [index, value] : other.elements) {
            if (elements.find(index) != elements.end()) {
                if (value != other.elements.at(index)) {
                    return false;
                }
            } else if (value != 0) {
                return false;
            }
        }
        return true;
    }

    Vector operator+(const Vector& other) const {
        if (dimension != other.dimension) {
            throw InvalidArgument("Vectors must have the same dimension for addition");
        }
        Vector result(dimension, epsilon);
        for (const auto& [index, value] : elements) {
            result.set(index, value + other.get(index));
        }
        for (const auto& [index, value] : other.elements) {
            if (elements.find(index) == elements.end()) {
                result.set(index, value);
            }
        }
        return result;
    }

    Vector operator-(const Vector& other) const {
        if (dimension != other.dimension) {
            throw InvalidArgument("Vectors must have the same dimension for subtraction");
        }
        Vector result(dimension, epsilon);
        for (const auto& [index, value] : elements) {
            result.set(index, value - other.get(index));
        }
        for (const auto& [index, value] : other.elements) {
            if (elements.find(index) == elements.end()) {
                result.set(index, -value);
            }
        }
        return result;
    }

    Vector operator-() const {
        Vector result(dimension, epsilon);
        for (const auto& [index, value] : elements) {
            result.set(index, -value);
        }
        return result;
    }

    Vector operator*(double scalar) const {
        Vector result(dimension, epsilon);
        for (const auto& [index, value] : elements) {
            result.set(index, value * scalar);
        }
        return result;
    }
    template<typename Matrix_elements_T, typename Matrix_container_T>
    Vector operator*(Matrix<Matrix_elements_T, Matrix_container_T> M) {
        if (M.rows != this->dimension) {
            throw InvalidArgument("Matrix rows must match the vector's dimension.");
        }

        Vector result(M.columns, this->epsilon);

        for (const auto& [key, value] : M.data) {
            auto [row, col] = key;
            result.set(col, result.get(col) + value * this->get(row));
        }

        return result;
    }

    Vector operator/(double scalar) const {
        if (scalar == 0.0) {
            throw DivisionByZeroException(*this);
        }
        Vector result(dimension, epsilon);
        for (const auto& [index, value] : elements) {
            result.set(index, value / scalar);
        }
        return result;
    }

    std::string to_string() const {
        std::ostringstream out;
        out << "Vector(dim=" << dimension << ", epsilon=" << epsilon << ")\n";
        for (const auto& [index, value] : elements) {
            out << "Index " << index << ": " << value << "\n";
        }
        return out.str();
    }
};

template <typename element_T, typename container_T>
const element_T Vector<element_T, container_T>::zero = element_T(0);

template <typename element_T, typename container_T>
std::ostream& operator<<(std::ostream& os, const Vector<element_T, container_T>& obj) {
    os << obj.to_string();
    return os;
}

template<>
class Vector<bool> : public VectorBase {
private:
    std::vector<uint64_t> data;
    unsigned int dimension;

    unsigned int getBlockIndex(unsigned int index) const {
        return index / 64;
    }

    unsigned int getBitIndex(unsigned int index) const {
        return index % 64;
    }
public:
    explicit Vector(unsigned int dimension)
        : dimension(dimension), data(std::vector<uint64_t>((dimension + 63) / 64, 0)) {}

    void set(unsigned int index, bool value) {
        if (index >= dimension) {
            throw OutOfRangeException(index, dimension - 1);
        }
        unsigned int block = getBlockIndex(index);
        unsigned int bit = getBitIndex(index);
        if (value) {
            data[block] |= (1ULL << bit); // Установка бита
        } else {
            data[block] &= ~(1ULL << bit); // Сброс бита
        }
    }

    bool get(unsigned int index) const {
        if (index >= dimension) {
            throw OutOfRangeException(index, dimension - 1);
        }
        unsigned int block = getBlockIndex(index);
        unsigned int bit = getBitIndex(index);
        return (data[block] & (1ULL << bit)) != 0;
    }

    Vector<bool> operator&(const Vector& other) const {
        if (dimension != other.dimension) {
            throw InvalidArgument("Size mismatch");
        }
        Vector<bool> result(dimension);
        for (unsigned int i = 0; i < data.size(); ++i) {
            result.data[i] = data[i] & other.data[i];
        }
        return result;
    }

    Vector<bool> operator*(const Vector& other) const {
        return (*this)&other;
    }

    Vector<bool> operator|(const Vector& other) const {
        if (dimension != other.dimension) {
            throw InvalidArgument("Size mismatch");
        }
        Vector<bool> result(dimension);
        for (unsigned int i = 0; i < data.size(); ++i) {
            result.data[i] = data[i] | other.data[i];
        }
        return result;
    }
    Vector<bool> operator+(const Vector& other) const {
        return (*this)|other;
    }

    std::string to_string() const {
        std::ostringstream os;
        os << "Vector(dim=" << dimension << ")\n";
        for (unsigned int i = 0; i < dimension; ++i) {
            auto temp = get(i);
            if (temp)
                os << "Index " << i << ": " << temp << "\n";
        }
        return os.str();
    }
};

std::ostream& operator<<(std::ostream& os, const Vector<bool>& obj) {
    os << obj.to_string();
    return os;
}
