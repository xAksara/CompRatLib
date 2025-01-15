#pragma once


#include <unordered_map>
#include <map>
#include <iostream>
#include <vector>
#include <cctype>
#include <cstdint>
#include <unordered_set>
#include "Rational_number.hpp"
#include "Complex_number.hpp"
#include "Utils.hpp"
#include "Matrix_proxy.hpp"

// Здесь пытался использовать наследование и специализацию шаблонов чтоб реализовать ограничение на типа контейнера без использования 
// enable_if и template <typename T = double, typename container_T = std::map<unsigned int, element_T>>
// Но уперся в непонимание как в базовом классе получать тип контейнера, известный только в специализациях дочерних шаблонов.
// Пытался через virtual auto& getContainer() = 0; и virtual typename Container_T& getContainer() = 0; Но не сработало....

// template<typename T>
// class MatrixBase {
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
// class Matrix {};

// template<typename T, typename key_type, typename value_type, typename compare, typename allocator>
// class Matrix<T, std::map<key_type, value_type, compare, allocator>> : public MatrixBase<T> { 
//     using Container_T = std::map<key_type, value_type, compare, allocator>;
//     Container_T container;
// public:
//     void mapMethod(T val) {
//         std::cout << "Map method for all specializations.\n" << val << std::endl;
//     }

//     Container_T getContainer() { return container; }
// };


// template<typename T, typename key_type, typename value_type, typename hash, typename key_equal, typename allocator>
// class Matrix<T, std::unordered_map<key_type, value_type, hash, key_equal, allocator>> : public MatrixBase<T> { 
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


template <typename T = double, typename container_T = std::map<std::pair<size_t, size_t>, T>>
class Matrix {
private:
    container_T data;
    size_t rows;
    size_t columns;
    double epsilon;
    static const T zero;
    std::vector<Matrix_proxy<T, container_T>*> proxies;
    std::unordered_set<Matrix_proxy<T, container_T>*> proxies_set;
public:
    void clean_pointer(Matrix_proxy<T, container_T>* p) {
        std::cout << "DELETE PROXY FROM SET " << p << std::endl;
        proxies_set.erase(p);
    }

    Matrix(size_t rows, size_t columns) : rows(rows), columns(columns), epsilon(0) {}
    // explicit потому что сильно копипастил вектор и констуркторы вида Matrix(5, 0.1, 5) приводят дабл к сайз_т
    explicit Matrix(size_t rows, size_t columns, double epsilon) : rows(rows), columns(columns), epsilon(epsilon) { 
        if (epsilon < 0) {
            throw NegativeEpsilonException(epsilon);
        }
    }
    Matrix(size_t rows, size_t columns, double epsilon, T init_val) : rows(rows), columns(columns), epsilon(epsilon) { 
        if (epsilon < 0) {
            throw NegativeEpsilonException(epsilon);
        }
        if (abs(init_val) >= epsilon) {
            for (size_t i = 0; i < rows; ++i) {
                for (size_t j = 0; j < columns; ++j) {
                    data[std::make_pair(i, j)] = init_val;
                }
            }
        }
    }
    Matrix(std::string filename) {
        epsilon = 0;
        std::ifstream file(filename);
        std::stringstream ss;
        ss << file.rdbuf(); // Пытаюсь отловить баг. http://it.kgsu.ru/C_STL/c_stl500.html
        file.close();
        std::string file_content = ss.str(); // переносим содержимое в строку
        ss.str(""); // чистим
        std::cout << "ZASTRYAL TYT" << std::endl;
        std::string temp_str = delete_comments(file_content);
        std::cout << "POCHEMY NE VIVODITSA" << std::endl;
        ss << temp_str;
        std::cout << "POCHEMY VIVODITSA" << std::endl;

        std::string file_data_type;
        ss >> file_data_type;
        std::cout << "POCHEMY VIVODITSA2222222" << file_data_type <<  std::endl;
        if (file_data_type != "matrix") {
            throw WrongFileException("Файл должен хранить матрицу. Имя файла: " + filename);
        }
        std::cout << "FILE DATA TYPE CLEAR" << std::endl;

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
            std::cout << "FILE DATA TYPE CLEAR" << std::endl;
        
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
            ss >> rows >> columns;
            size_t row, column;
            std::cout << "FILE DATA TYPE CLEAR" << std::endl;

            while (true) {
            std::cout << "FILE DATA TYPE CLEAR3" << std::endl;
                ss >> row >> column;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1)); // delete ()
                T element(value.c_str());
                data[{row - 1, column - 1}] = element;
            }
            std::cout << "FILE DATA TYPE CLEAR4" << std::endl;
        } else if (element_type == "rational") {
            ss >> rows >> columns;
            size_t row, column;
            while (true) {
                ss >> row >> column;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                T element(value.c_str());
                data[{row - 1, column - 1}] = element;
            }
        } else {
            throw WrongFileException("Не верный формат файла. Файл: " + filename);
        }
        std::cout << "READED" << std::endl;
    }

    Matrix(Matrix& rhs) {
        data = rhs.data;
        rows = rhs.rows;
        columns = rhs.columns;
        epsilon = rhs.epsilon;
    }

    Matrix(Matrix&& rhs) {
        data = std::move(rhs.data);
        rows = rhs.rows;
        columns = rhs.columns;
        epsilon = rhs.epsilon;
    }

    ~Matrix() {
        for (auto& proxy : proxies_set) {
            std::cout << "Destructor matrix" << this << std::endl;
            proxy->clean_pointer();
        }
    }

    Matrix& operator=(const Matrix& rhs) {
        if (this != &rhs) {
            data = rhs.data;
            rows = rhs.rows;
            columns = rhs.columns;
            epsilon = rhs.epsilon;
        }
        return *this;
    }
    Matrix& operator=(Matrix&& rhs) noexcept { 
        if (this != &rhs) {
            data = std::move(rhs.data);
            rows = rhs.rows;
            columns = rhs.columns;
            epsilon = rhs.epsilon;
        }
        return *this;
    }
    const T& operator()(size_t i, size_t j) const {
        if (i >= rows || j >= columns) {
            throw MatrixIndexOutOfRangeException(i, j, rows, columns);
        }
        const auto& it = data.find(std::make_pair(i, j));
        return (it != data.end()) ? it->second : zero;
    }
    T& operator()(size_t i, size_t j) {
        if (i >= rows || j >= columns) {
            throw MatrixIndexOutOfRangeException(i, j, rows, columns);
        }
        return data[std::make_pair(i, j)];
    }

    Matrix_proxy<T, container_T>* operator[](const Matrix_coords& coords) {
        Matrix_proxy<T, container_T>* proxy = new Matrix_proxy<T, container_T>(*this, coords);
        std::cout << "NEW PROXY" << proxy << std::endl;
        // proxies.push_back(proxy);
        proxies_set.insert(proxy);
        return proxy;
    }

    Matrix_proxy<T, container_T>* operator[](const Matrix_row_coord& coords) {
        Matrix_proxy<T, container_T>* proxy = new Matrix_proxy<T, container_T>(*this, coords);
        // proxies.push_back(proxy);
        std::cout << "NEW PROXY" << proxy << std::endl;
        proxies_set.insert(proxy);
        return proxy;
    }

    Matrix_proxy<T, container_T>* operator[](const Matrix_col_coord& coords) {
        Matrix_proxy<T, container_T>* proxy = new Matrix_proxy<T, container_T>(*this, coords);
        // proxies.push_back(proxy);
        std::cout << "NEW PROXY" << proxy << std::endl;
        proxies_set.insert(proxy);
        return proxy;
    }

    void set(size_t i, size_t j, const T& val) {
        if (i >= rows || j >= columns) {
            throw MatrixIndexOutOfRangeException(i, j, rows, columns);
        }
        if (abs(val) >= epsilon)
            data[std::make_pair(i, j)] = val;
    }
    container_T getData() const { return data; }
    size_t getRows() const { return rows; }
    size_t getColumns() const { return columns; }
    double getEpsilon() const { return epsilon; }
    std::string to_string() const {
        std::stringstream ss;
        ss << "<Matrix rows: " << rows << ", columns: " << columns << ", epsilon: " << epsilon << ", data:[";
        size_t counter = 0; // чтобы поймать  последний элемент. Хотя можно через итераторы, но так симпотичнее
        size_t data_size = data.size();
        for (const auto& [key, value] : data) {
            ss << "{(" << key.first << ", " << key.second << "), " << value << (++counter < data_size ? "}, " : "}");
        }
        ss << "]>";
        return ss.str();
    }
};

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
bool operator==(const Matrix<lhs_T, lhs_container_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getRows() != rhs.getRows() || lhs.getColumns() != rhs.getColumns() || lhs.getEpsilon() != rhs.getEpsilon()) return false;
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            if (lhs_data[std::make_pair(i, j)] != rhs_data[std::make_pair(i, j)]) return false;
        }
    }
    return true;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
bool operator!=(const Matrix<lhs_T, lhs_container_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getRows() != rhs.getRows() || lhs.getColumns() != rhs.getColumns() || lhs.getEpsilon() != rhs.getEpsilon()) return true;
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            if (lhs_data[std::make_pair(i, j)] != rhs_data[std::make_pair(i, j)]) return true;
        }
    }
    return false;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
Matrix<lhs_T, lhs_container_T> operator+(const Matrix<lhs_T, lhs_container_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getRows() != rhs.getRows() || lhs.getColumns() != rhs.getColumns()) {
        throw MatrixSizeMissmatch(lhs.getRows(), lhs.getColumns(), rhs.getRows(), rhs.getColumns());
    }
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<lhs_T, lhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i < lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            lhs_T sum = lhs_data[std::make_pair(i, j)] + rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T, typename rhs_container_T>
Matrix<lhs_T, lhs_container_T> operator-(const Matrix<lhs_T, lhs_container_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    if (lhs.getRows() != rhs.getRows() || lhs.getColumns() != rhs.getColumns()) {
        throw MatrixSizeMissmatch(lhs.getRows(), lhs.getColumns(), rhs.getRows(), rhs.getColumns());
    }
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<lhs_T, lhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            lhs_T sub = lhs_data[std::make_pair(i, j)] - rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sub);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<lhs_T, lhs_container_T> operator+(const Matrix<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<lhs_T, lhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            lhs_T sum = lhs_data[std::make_pair(i, j)] + rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<lhs_T, lhs_container_T> operator-(const Matrix<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<lhs_T, lhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            lhs_T sum = lhs_data[std::make_pair(i, j)] - rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<lhs_T, lhs_container_T> operator*(const Matrix<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<lhs_T, lhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            lhs_T sum = lhs_data[std::make_pair(i, j)] * rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<lhs_T, lhs_container_T> operator/(const Matrix<lhs_T, lhs_container_T>& lhs, const rhs_T& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<lhs_T, lhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            lhs_T sum = lhs_data[std::make_pair(i, j)] / rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<rhs_T, rhs_container_T> operator+(const lhs_T& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<rhs_T, rhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            rhs_T sum = rhs_data[std::make_pair(i, j)] + lhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<rhs_T, rhs_container_T> operator-(const lhs_T& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<rhs_T, rhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            rhs_T sum = static_cast<rhs_T>(lhs) - rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<rhs_T, rhs_container_T> operator*(const lhs_T& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<rhs_T, rhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            rhs_T sum = rhs_data[std::make_pair(i, j)] * lhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<rhs_T, rhs_container_T> operator/(const lhs_T& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<rhs_T, rhs_container_T> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            rhs_T sum = static_cast<rhs_T>(lhs) / rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

// Комплексные и рациональные

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<Rational_number<rhs_T>> operator+(const Matrix<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Rational_number<rhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Rational_number<rhs_T> sum = lhs_data[std::make_pair(i, j)] + rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;

}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<Rational_number<rhs_T>> operator-(const Matrix<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Rational_number<rhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Rational_number<rhs_T> sum = lhs_data[std::make_pair(i, j)] - rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<Rational_number<rhs_T>> operator*(const Matrix<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Rational_number<rhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Rational_number<rhs_T> sum = lhs_data[std::make_pair(i, j)] * rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_T>
Matrix<Rational_number<rhs_T>> operator/(const Matrix<lhs_T, lhs_container_T>& lhs, const Rational_number<rhs_T>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Rational_number<rhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Rational_number<rhs_T> sum = lhs_data[std::make_pair(i, j)] / rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<Rational_number<lhs_T>> operator+(const Rational_number<lhs_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Rational_number<lhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Rational_number<lhs_T> sum = rhs_data[std::make_pair(i, j)] + lhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<Rational_number<lhs_T>> operator-(const Rational_number<lhs_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
    size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Rational_number<lhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Rational_number<lhs_T> sum = lhs - rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<Rational_number<lhs_T>> operator*(const Rational_number<lhs_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
    size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Rational_number<lhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Rational_number<lhs_T> sum = rhs_data[std::make_pair(i, j)] * lhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename rhs_T, typename rhs_container_T>
Matrix<Rational_number<lhs_T>> operator/(const Rational_number<lhs_T>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
    size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Rational_number<lhs_T>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Rational_number<lhs_T> sum = lhs / rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<rhs_real, rhs_imag>> operator+(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<rhs_real, rhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<rhs_real, rhs_imag> sum = lhs_data[std::make_pair(i, j)] + rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<rhs_real, rhs_imag>> operator-(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<rhs_real, rhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<rhs_real, rhs_imag> sum = lhs_data[std::make_pair(i, j)] - rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<rhs_real, rhs_imag>> operator*(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<rhs_real, rhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<rhs_real, rhs_imag> sum = lhs_data[std::make_pair(i, j)] * rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<rhs_real, rhs_imag>> operator/(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<rhs_real, rhs_imag>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<rhs_real, rhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<rhs_real, rhs_imag> sum = lhs_data[std::make_pair(i, j)] / rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}




template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Matrix<Complex_number<lhs_real, lhs_imag>> operator+(const Complex_number<lhs_real, lhs_imag>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
    size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Complex_number<lhs_real, lhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Complex_number<lhs_real, lhs_imag> sum = lhs + rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Matrix<Complex_number<lhs_real, lhs_imag>> operator-(const Complex_number<lhs_real, lhs_imag>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Complex_number<lhs_real, lhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Complex_number<lhs_real, lhs_imag> sum = lhs - rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Matrix<Complex_number<lhs_real, lhs_imag>> operator*(const Complex_number<lhs_real, lhs_imag>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
    size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Complex_number<lhs_real, lhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Complex_number<lhs_real, lhs_imag> sum = lhs * rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}


template<typename lhs_real, typename lhs_imag, typename rhs_T, typename rhs_container_T>
Matrix<Complex_number<lhs_real, lhs_imag>> operator/(const Complex_number<lhs_real, lhs_imag>& lhs, const Matrix<rhs_T, rhs_container_T>& rhs) {
    double eps = rhs.getEpsilon();
    size_t rows = rhs.getRows();
    size_t columns = rhs.getColumns();
    rhs_container_T rhs_data = rhs.getData();
    Matrix<Complex_number<lhs_real, lhs_imag>> result(rows, columns, eps);
    for (size_t i = 0; i <  rhs.getColumns(); ++i) {
        for (size_t j = 0; j < rhs.getColumns(); ++j) {
            Complex_number<lhs_real, lhs_imag> sum = lhs / rhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}




// сложение вектора комплексных и комплексного, параметризованного рациональным должно вернуть вектор комплексного, параметризованного рациональным
// Т.к. слишком много добавлять, решил оставить ограничение. Если типы комплексных у вектора и слагаемого разные, то ошибка компиляции. 

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator+(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = rhs + lhs_data[std::make_pair(i, j)]; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator-(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>(lhs_data[std::make_pair(i, j)]) - rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator*(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>(lhs_data[std::make_pair(i, j)]) * rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<typename lhs_T, typename lhs_container_T, typename rhs_real, typename rhs_imag>
Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> operator/(const Matrix<lhs_T, lhs_container_T>& lhs, const Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>& rhs) {
    double eps = lhs.getEpsilon();
    size_t rows = lhs.getRows();
    size_t columns = lhs.getColumns();
    lhs_container_T lhs_data = lhs.getData();
    Matrix<Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>> result(rows, columns, eps);
    for (size_t i = 0; i <  lhs.getRows(); ++i) {
        for (size_t j = 0; j < lhs.getColumns(); ++j) {
            Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>> sum = Complex_number<Rational_number<rhs_real>, Rational_number<rhs_imag>>(lhs_data[std::make_pair(i, j)]) / rhs; // работаем с копией, так что можем засорять нулями
            result.set(i, j, sum);
        }
    }
    return result;
}

template<>
class Matrix<bool> {
private:
    std::vector<uint64_t> data;
    size_t rows;
    size_t columns;
public:
    Matrix(size_t rows, size_t columns) : rows(rows), columns(columns), data(std::vector<uint64_t>((rows * columns + 63) / 64, 0)) {} // граничные случаи правильно
    Matrix(size_t rows, size_t columns, bool val) : rows(rows), columns(columns) {
        data = std::vector<uint64_t>((rows*columns + 63) / 64, (val ? 0xffffffffffffffff : 0)); // ff - 1байт
    }
    Matrix(std::string filename) {
        std::ifstream file(filename);
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();
        std::string file_content = ss.str();
        ss.str("");
        ss << delete_comments(file_content);
        std::string file_data_type;
        ss >> file_data_type;
        if (file_data_type != "matrix") {
            throw WrongFileException("Файл должен хранить матрицу. Имя файла: " + filename);
        }
        std::string element_type;
        ss >> element_type;
        if (element_type == "bit") {
            ss >> rows >> columns;
            data = std::vector<uint64_t>((rows * columns + 63) / 64, 0);
            size_t row, column;
            while (true) {
                ss >> row >> column;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                if (value != "0") {
                    set(row - 1, column -  1, 1);
                }
            }
            
        }
    }

    void set(size_t i, size_t j, bool value) {
        if (i >= rows || j >= columns) {
            throw MatrixIndexOutOfRangeException(i, j, rows, columns);
        }
        size_t idx = i * columns + j;
        size_t block = idx / 64;
        size_t bit_in_block = idx % 64;
        if (value) {
            data[block] |= ((1UL << 63) >> bit_in_block); // 1<<63 = 10000...0; >> bit_in_block - выставили  нужнй бит
        } else {
            data[block] &= ~((1UL << 63) >> bit_in_block); // получаем 111...0...1111 (нолик в нужном бите) и делаем &
        }
    }

    bool operator()(size_t i, size_t j) const {
        if (i >= rows || j >= columns) {
            throw MatrixIndexOutOfRangeException(i, j, rows, columns);
        }
        size_t idx = i * columns + j;
        size_t block = idx / 64;
        size_t bit_in_block = idx % 64;
        return (data[block] & ((1UL << 63) >> bit_in_block)) != 0;
    }

    Matrix<bool> operator*(const Matrix& rhs) const {
        if (rows != rhs.rows || columns != rhs.columns) {
            throw MatrixSizeMissmatch(rows, columns, rhs.rows, rhs.columns);
        }
        Matrix<bool> result(rows, columns);
        for (size_t i = 0; i < data.size(); ++i) {
            result.data[i] = data[i] & rhs.data[i];
        }
        return result;
    }

    Matrix<bool> operator+(const Matrix& rhs) const {
        if (rows != rhs.rows || columns != rhs.columns) {
            throw MatrixSizeMissmatch(rows, columns, rhs.rows, rhs.columns);
        }
        Matrix<bool> result(rows, columns);
        for (size_t i = 0; i < data.size(); ++i) {
            result.data[i] = data[i] | rhs.data[i];
        }
        return result;
    }

    bool operator==(const Matrix& rhs) const {
        if (rows != rhs.rows || columns != rhs.columns) return false;
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] != rhs.data[i]) return false;
        }
        return true;
    }

    bool operator!=(const Matrix& rhs) const {
        if (rows != rhs.rows || columns != rhs.columns) return true;
        for (size_t i = 0; i < data.size(); ++i) {
            if (data[i] != rhs.data[i]) return true;
        }
        return false;
    }

    std::string to_string() const {
        std::stringstream ss;
        ss << "<Matrix rows: " << rows << ", columns: " << columns << ", data:[";
        for (size_t i = 0; i < rows; ++i) {
            ss << '[';
            for (size_t j = 0; j < columns; ++j) {
                ss << (*this)(i, j);
            }
            ss << "], ";
        }
        ss << "]>";
        return ss.str();
    }
};


template<typename T, typename container_T>
std::ostream& operator<<(std::ostream& os, const Matrix<T, container_T>& v) {
    os << v.to_string();
    return os;
}

template <typename T, typename container_T>
const T Matrix<T, container_T>::zero = 0;




