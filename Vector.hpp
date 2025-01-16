#pragma once

#include <unordered_map>
#include <map>
#include <iostream>
#include <vector>
#include <cstdint>
#include "Rational_number.hpp"
#include "Complex_number.hpp"
#include "Utils.hpp"
#include "Matrix.hpp"
#include "Matrix_proxy.hpp"

// Здесь пытался использовать наследование и специализацию шаблонов чтоб реализовать ограничение на типа контейнера без использования 
// enable_if и template <typename T = double, typename container_T = std::map<unsigned int, element_T>>
// Но уперся в непонимание как в базовом классе получать тип контейнера, известный только в специализациях дочерних шаблонов.
// Пытался через virtual auto& getContainer() = 0; и virtual typename Container_T& getContainer() = 0; Но не сработало....

// template<typename T>
// class VectorBase {
//     // Будем получать контейнер. 
//     virtual typename Container_T& getContainer() = 0;
// public:
//     void commonMethod() {
//         std::cout << "Common method for all specializations.\n";
//     }

//     void print() {
//         T a = 5;
//         typename Container_T cont = getContainer();
//         cont[0] = a;
//         std::cout << cont[0];
//     }
// };

// template<typename T = double, typename Container = std::map<size_t, T>>
// class Vector {};

// template<typename T, typename key_type, typename value_type, typename compare, typename allocator>
// class Vector<T, std::map<key_type, value_type, compare, allocator>> : public VectorBase<T> { 
//     using Container_T = std::map<key_type, value_type, compare, allocator>;
//     Container_T container;
// public:
//     void mapMethod(T val) {
//         std::cout << "Map method for all specializations.\n" << val << std::endl;
//     }

//     Container_T getContainer() { return container; }
// };


// template<typename T, typename key_type, typename value_type, typename hash, typename key_equal, typename allocator>
// class Vector<T, std::unordered_map<key_type, value_type, hash, key_equal, allocator>> : public VectorBase<T> { 
//     using Container_T = std::unordered_map<key_type, value_type, hash, key_equal, allocator>;
//     Container_T container;
// public:
//     void mapMethod(T val) {
//         std::cout << "Umap method for all specializations.\n" << val << std::endl;
//     }

//     Container_T getContainer() { return container; }
// };






// ЕСЛИ container_T не словарь и не хэш таблица то все навернется, но в условии задачи сказано что 
// контейнером может быть только словарь и хэш таблица. Все сломается если у контейнера не будет методов
// которые используются внутри функций. А если будут то по идее все ок. Так что в целом типы,
// определенные пользователем тоже можно скормить этому шаблону, и если звезды сойдутся то код скомпилируется.

/**
 * @class Класс представляющий разреженные векторы
 * 
 * @tparam T тип элемента
 * @tparam container_T тип контейнера(map или unordered_map параметризованные size_t и T)
 */
template <typename T = double, typename container_T = std::map<size_t, T>>
class Vector {
private:
    container_T data;
    size_t size;
    double epsilon;
    static const T zero;
public:
    /**
     * @brief Конструктор с одним аргументом. Выставляет эпсилон = 0
     * @param size размер вектора 
     */
    Vector(size_t size) : size(size), epsilon(0) {}
    /**
     * @brief Конструктор с двумя аргументами
     * @param size размер вектора 
     * @param epsilon эпсилон
     */
    Vector(size_t size, double epsilon) : size(size), epsilon(epsilon) { 
        if (epsilon < 0) {
            throw NegativeEpsilonException(epsilon);
        }
    }
    /**
     * @brief Конструктор с тремя аргументами. Инициализирует вектор значениями init_val
     * @param size размер вектора 
     * @param size эпсилон
     * @param size начальное значение
     */
    Vector(size_t size, double epsilon, T init_val) : size(size), epsilon(epsilon) { 
        if (epsilon < 0) {
            throw NegativeEpsilonException(epsilon);
        }
        if (abs(init_val) >= epsilon) {
            for (size_t i = 0; i < size; ++i) {
                data[i] = init_val;
            }
        }
    }
    Vector(Vector& rhs) {
        data = rhs.data;
        size = rhs.size;
        epsilon = rhs.epsilon;
    }
    Vector(Vector&& rhs) {
        data = std::move(rhs.data);
        size = rhs.size;
        epsilon = rhs.epsilon;
    }
    /**
     * @brief Конструктор, строящий вектор по срезу строки
     * @tparam proxy_container_T тип контейнера прокси (матрицы, его создавшей)
     * @param rhs срез строки матрицы
     * @throw ProxyToVectorException если прокси неправильный
     */
    template<typename proxy_container_T>
    Vector(const Matrix_proxy<T, proxy_container_T>& rhs) { 
        if (!rhs.is_row) {
            throw ProxyToVectorException("Прокси обхект должен быть прокси-строкой.", rhs);
        }
        size = rhs.getSize();
        std::cout << "SIZE" << size << std::endl;
        epsilon = rhs.getEpsilon();
        for (size_t idx = 0; idx < size; ++idx) {
            if (rhs(idx) < epsilon || epsilon == 0) {
                data[idx] = rhs(idx);
            }
        }
    }
    /**
     * @brief Конструктор, создающий вектор по файлу
     * @param filename имя файла с вектором
     * @throw WrongFileException если файл имеет некорректный вид
     */
    Vector(std::string filename) {
        epsilon = 0;
        std::ifstream file(filename);
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        std::string file_content = ss.str();
        ss.str("");
        ss << delete_comments(file_content);
        std::string file_data_type;
        ss >> file_data_type;
        if (file_data_type != "vector") {
            
            throw WrongFileException("Файл должен хранить вектор. Имя файла: " + filename);
        }
        std::string element_type;
        ss >> element_type;
        if (element_type == "complex") {
            std::string type_real, type_image;
            ss >> type_real >> type_image;
            Complex_number<int, int> c_int_int;
            Complex_number<int, double> c_int_float;
            Complex_number<double, int> c_float_int;
            Complex_number<double, double> c_float_float;
            T for_comparison;
            if (type_real == "integer" && type_image == "integer") {
                if (typeid(for_comparison).name() != typeid(c_int_int).name()) {
                    
            throw WrongFileException("Файл должен содержать complex integer integer. Имя файла: " + filename);
                }
            } else if (type_real == "integer" && type_image == "float") {
                if (typeid(for_comparison).name() != typeid(c_int_float).name()) {
                    
            throw WrongFileException("Файл должен содержать complex integer float. Имя файла: " + filename);
                }
            } else if (type_real == "float" && type_image == "integer") {
                if (typeid(for_comparison).name() != typeid(c_float_int).name()) {
                    
            throw WrongFileException("Файл должен содержать complex float integer. Имя файла: " + filename);
                }
            } else if (type_real == "float" && type_image == "float") {
                if (typeid(for_comparison) != typeid(c_float_float)) {
                    
            throw WrongFileException("Файл должен содержать complex float float. Имя файла: " + filename);
                }
            } else {
                
            throw WrongFileException("Не валидные типы параметров комплексного числа. Имя файла: " + filename);
            }
            ss >> size;
            size_t coord;
            while (true) {
                ss >> coord;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1)); // delete ()
                T element(value.c_str());
                data[coord - 1] = element;
            }
            
        } else if (element_type == "rational") {
            ss >> size;
            size_t coord;
            while (true) {
                ss >> coord;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                T element(value.c_str());
                data[coord - 1] = element;
            }
            
        } else {
            
            throw WrongFileException("Не верный формат файла. Файл: " + filename);
        }
    }


    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            data = rhs.data;
            size = rhs.size;
            epsilon = rhs.epsilon;
        }
        return *this;
    }
    Vector& operator=(Vector&& rhs) noexcept { 
        if (this != &rhs) {
            data = std::move(rhs.data);
            size = rhs.size;
            epsilon = rhs.epsilon;
        }
        return *this;
    }
    /**
     * @brief Оператор присваивания, строящий вектор по срезу строки
     * @tparam proxy_container_T тип контейнера прокси (матрицы, его создавшей)
     * @param rhs срез строки матрицы
     * @throw ProxyToVectorException если прокси неправильный
     * @return построенный вектор
     */
    template<typename proxy_container_T>
    Vector& operator=(const Matrix_proxy<T, proxy_container_T>& rhs) { 
        if (!rhs.is_row) {
            throw ProxyToVectorException("Прокси обхект должен быть прокси-строкой.", rhs);
        }
        size = rhs.getSize();
        epsilon = rhs.getEpsilon();
        for (size_t idx = 0; idx < size; ++idx) {
            if (rhs(idx) < epsilon) {
                data[idx] = rhs(idx);
                std::cout << rhs(idx);
            }
        }
        return *this;
    }
    
    const T& operator()(size_t idx) const {
        if (idx >= size) {
            throw VectorIndexOutOfRangeException(idx, size);
        }
        const auto& it = data.find(idx);
        return (it != data.end()) ? it->second : zero;
    }
    T& operator()(size_t idx) {
        if (idx >= size) {
            throw VectorIndexOutOfRangeException(idx, size);
        }
        return data[idx];
    }
    void set(size_t idx, const T& val) {
        if (idx >= size) {
            throw VectorIndexOutOfRangeException(idx, size);
        }
        if (abs(val) >= epsilon)
            data[idx] = val;
    }
    container_T getData() const { return data; }
    size_t  getSize() const { return size; }
    double getEpsilon() const { return epsilon; }
    std::string to_string() const {
        std::stringstream ss;
        ss << "<Vector size: " << size << ", epsilon: " << epsilon << ", data:[";
        size_t counter = 0; // чтобы поймать  последний элемент. Хотя можно через итераторы, но так симпотичнее
        size_t data_size = data.size();
        for (const auto& [key, value] : data) {
            ss << "{" << key << ", " << value << (++counter < data_size ? "}, " : "}");
        }
        ss << "]>";
        return ss.str();
    }
};

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
bool operator==(const Vector<lhs_T, lhs_container_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getSize() != rhs.getSize() || lhs.getEpsilon() != rhs.getEpsilon()) return false;
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    // for (size_t idx = 0; idx < lhs.getSize(); ++idx) {
    //     if (lhs_data[idx] != rhs_data[idx]) return false;
    // }
    for (const auto& [key, value] : lhs_data) {
        if (value != rhs_data[key]) return false;
    }
    for (const auto& [key, value] : lhs_data) {
        if (value != rhs_data[key]) return false;
    }

    return true;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
bool operator!=(const Vector<lhs_T, lhs_container_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getSize() != rhs.getSize() || lhs.getEpsilon() != rhs.getEpsilon()) return true;
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    for (size_t idx = 0; idx < lhs.getSize(); ++idx) {
        if (lhs_data[idx] != rhs_data[idx]) return true;
    }
    return false;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
Vector<lhs_T, lhs_container_T> operator+(const Vector<lhs_T, lhs_container_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getSize() != rhs.getSize()) {
        throw VectorSizeMissmatch(lhs.getSize(), rhs.getSize());
    }
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    Vector<lhs_T, lhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        lhs_T sum = lhs_data[idx] + rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
Vector<lhs_T, lhs_container_T> operator-(const Vector<lhs_T, lhs_container_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getSize() != rhs.getSize()) {
        throw VectorSizeMissmatch(lhs.getSize(), rhs.getSize());
    }
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    Vector<lhs_T, lhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        lhs_T sub = lhs_data[idx] - rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sub);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<lhs_T, lhs_container_T> operator+(const Vector<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<lhs_T, lhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        lhs_T sum = lhs_data[idx] + rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<lhs_T, lhs_container_T> operator-(const Vector<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<lhs_T, lhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        lhs_T sum = lhs_data[idx] - rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<lhs_T, lhs_container_T> operator*(const Vector<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<lhs_T, lhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        lhs_T sum = lhs_data[idx] * rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<lhs_T, lhs_container_T> operator/(const Vector<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<lhs_T, lhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        lhs_T sum = lhs_data[idx] / rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<rhs_T, rhs_container_T> operator+(const lhs_T& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<rhs_T, rhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        rhs_T sum = rhs_data[idx] + lhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<rhs_T, rhs_container_T> operator-(const lhs_T& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<rhs_T, rhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        rhs_T sum = static_cast<rhs_T>(lhs) - rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<rhs_T, rhs_container_T> operator*(const lhs_T& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<rhs_T, rhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        rhs_T sum = rhs_data[idx] * lhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<rhs_T, rhs_container_T> operator/(const lhs_T& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<rhs_T, rhs_container_T> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        rhs_T sum = static_cast<rhs_T>(lhs) / rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

// Комплексные и рациональные

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<Rational_number<rhs_T>> operator+(const Vector<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Rational_number<rhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<rhs_T> sum = lhs_data[idx] + rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;

}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<Rational_number<rhs_T>> operator-(const Vector<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Rational_number<rhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<rhs_T> sum = lhs_data[idx] - rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<Rational_number<rhs_T>> operator*(const Vector<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Rational_number<rhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<rhs_T> sum = lhs_data[idx] * rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Vector<Rational_number<rhs_T>> operator/(const Vector<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Rational_number<rhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<rhs_T> sum = lhs_data[idx] / rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<Rational_number<lhs_T>> operator+(const Rational_number<lhs_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Rational_number<lhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<lhs_T> sum = rhs_data[idx] + lhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<Rational_number<lhs_T>> operator-(const Rational_number<lhs_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Rational_number<lhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<lhs_T> sum = lhs - rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<Rational_number<lhs_T>> operator*(const Rational_number<lhs_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Rational_number<lhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<lhs_T> sum = rhs_data[idx] * lhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Vector<Rational_number<lhs_T>> operator/(const Rational_number<lhs_T>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Rational_number<lhs_T>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Rational_number<lhs_T> sum = lhs / rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<rhs_real, rhs_imag>> operator+(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<rhs_real, rhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<rhs_real, rhs_imag> sum = lhs_data[idx] + rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<rhs_real, rhs_imag>> operator-(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<rhs_real, rhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<rhs_real, rhs_imag> sum = lhs_data[idx] - rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<rhs_real, rhs_imag>> operator*(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<rhs_real, rhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<rhs_real, rhs_imag> sum = lhs_data[idx] * rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<rhs_real, rhs_imag>> operator/(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<rhs_real, rhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<rhs_real, rhs_imag> sum = lhs_data[idx] / rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}




template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Vector<Complex_number<lhs_real, lhs_imag>> operator+(const Complex_number<lhs_real, lhs_imag>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Complex_number<lhs_real, lhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<lhs_real, lhs_imag> sum = lhs + rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Vector<Complex_number<lhs_real, lhs_imag>> operator-(const Complex_number<lhs_real, lhs_imag>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Complex_number<lhs_real, lhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<lhs_real, lhs_imag> sum = lhs - rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Vector<Complex_number<lhs_real, lhs_imag>> operator*(const Complex_number<lhs_real, lhs_imag>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Complex_number<lhs_real, lhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<lhs_real, lhs_imag> sum = lhs * rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}


template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Vector<Complex_number<lhs_real, lhs_imag>> operator/(const Complex_number<lhs_real, lhs_imag>& lhs, const Vector<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t size = rhs.getSize();
    rhs_container_T rhs_data = rhs.getData();
    Vector<Complex_number<lhs_real, lhs_imag>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<lhs_real, lhs_imag> sum = lhs / rhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}




// сложение вектора комплексных и комплексного, параметризованного рациональным должно вернуть вектор комплексного, параметризованного рациональным
// Т.к. слишком много добавлять, решил оставить ограничение. Если типы комплексных у вектора и слагаемого разные, то ошибка компиляции. 

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator+(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = rhs + lhs_data[idx]; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator-(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>(lhs_data[idx]) - rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator*(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>(lhs_data[idx]) * rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator/(const Vector<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t size = lhs.getSize();
    lhs_container_T lhs_data = lhs.getData();
    Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(size, eps);
    for (size_t idx = 0; idx < size; ++idx) {
        Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>(lhs_data[idx]) / rhs; // работаем с копией, так что можем засорять нулями
        result.set(idx, sum);
    }
    return result;
}

// template<typename lhs_real, typename lhs_image, typename lhs_container_T, typename rhs_real, typename rhs_imag>
// Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator+(const Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
//     double eps = lhs.getEpsilon();
//     size_t size = lhs.getSize();
//     lhs_container_T lhs_data = lhs.getData();
//     Vector<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(size, eps);
//     for (size_t idx = 0; idx < size; ++idx) {
//         Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = rhs + lhs_data[idx]; // работаем с копией, так что можем засорять нулями
//         result.set(idx, sum);
//     }
//     return result;
// }



// надо бы наделать специализаций, чтобы не приводить к lhs_T в случае комплексных и рациональных, а возвращать наиболее "широкий" тип
template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
Vector<lhs_T, lhs_container_T> operator *(const Vector<lhs_T, lhs_container_T>& v, const Matrix<rhs_T, rhs_container_T>& m) {
    if (v.getSize() != m.getRows()) {
        throw MatrixSizeMissmatch(1, v.getSize(), m.getRows(), m.getColumns());
    }
    Vector<lhs_T, lhs_container_T> result(m.getColumns(), v.getEpsilon());
    for (const auto& [key, m_value] : m.getData()) {
        size_t row = key.first; // key это пара <строка, столбец> (ключ в мапе)
        size_t column = key.second; // Еще можно [row, column] = key
        result.set(column, result(column) + v(row) * m_value);
    }
    return result;
}





template<>
class Vector<bool> {
private:
    std::vector<uint64_t> data;
    size_t size;
    const static uint64_t one = 1; // выяснилось что размер UL может быть не обязательно 64, так что завел переменную чтобы гарантировано использовать 64итное число
public:
    Vector(size_t size) : size(size), data(std::vector<uint64_t>((size + 63) / 64, 0)) {} // граничные случаи правильно
    Vector(size_t size, bool val) : size(size) {
        data = std::vector<uint64_t>((size + 63) / 64, (val ? 0xffffffffffffffff : 0)); // ff - 1байт
    }
    Vector(std::string filename) {
        std::ifstream file(filename);
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        std::string file_content = ss.str();
        ss.str("");
        ss << delete_comments(file_content);
        std::string file_data_type;
        ss >> file_data_type;
        if (file_data_type != "vector") {
            
            throw WrongFileException("Файл должен хранить вектор. Имя файла: " + filename);
        }
        std::string element_type;
        ss >> element_type;
        if (element_type == "bit") {
            ss >> size;
            data = std::vector<uint64_t>((size + 63) / 64, 0);
            size_t coord;
            while (true) {
                ss >> coord;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                if (value != "0") {
                    set(coord - 1, 1);
                }
            }
            
        }
    }

    void set(size_t idx, bool value) {
        if (idx >= size) {
            throw VectorIndexOutOfRangeException(idx, size - 1);
        }
        size_t block = idx / 64;
        size_t bit_in_block = idx % 64;
        if (value) {
            data[block] |= ((one << 63) >> bit_in_block); // 1<<63 = 10000...0; >> bit_in_block - выставили  нужнй бит
        } else {
            data[block] &= ~((one << 63) >> bit_in_block); // получаем 111...0...1111 (нолик в нужном бите) и делаем &
        }
    }

    bool operator()(size_t idx) const {
        if (idx >= size) {
            throw VectorIndexOutOfRangeException(idx, size - 1);
        }
        size_t block = idx / 64;
        size_t bit_in_block = idx % 64;
        return (data[block] & ((one << 63) >> bit_in_block)) != 0;
    }

    Vector<bool> operator*(const Vector& rhs) const {
        if (size != rhs.size) {
            throw VectorSizeMissmatch(size, rhs.size);
        }
        Vector<bool> result(size);
        for (size_t idx = 0; idx < data.size(); ++idx) {
            result.data[idx] = data[idx] & rhs.data[idx];
        }
        return result;
    }

    Vector<bool> operator+(const Vector& rhs) const {
        if (size != rhs.size) {
            throw VectorSizeMissmatch(size, rhs.size);
        }
        Vector<bool> result(size);
        for (size_t idx = 0; idx < data.size(); ++idx) {
            result.data[idx] = data[idx] | rhs.data[idx];
        }
        return result;
    }

    bool operator==(const Vector& rhs) const {
        if (size != rhs.size) return false;
        for (size_t idx = 0; idx < size; ++idx) {
            if ((*this)(idx) != rhs(idx)) return false;
        }
        return true;
    }

    bool operator!=(const Vector& rhs) const {
        if (size != rhs.size) return false;
        for (size_t idx = 0; idx < size; ++idx) {
            if ((*this)(idx) != rhs(idx)) return true;
        }
        return false;
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "<Vector size: " << size << ", data:[";
        for (size_t idx = 0; idx < size; ++idx) {
            ss << (*this)(idx);
        }
        ss << "]>";
        return ss.str();
    }
};


template<typename T, typename container_T>
std::ostream& operator<<(std::ostream& os, const Vector<T, container_T>& v) {
    os << v.to_string();
    return os;
}

// конст статик нельзя просто сделать = 0 в классе
template <typename T, typename container_T>
const T Vector<T, container_T>::zero = 0;




