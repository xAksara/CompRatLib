#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <regex>
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Rational.hpp"
#include "Complex.hpp"


class FileParser {
public:
    std::stringstream delete_comments(std::ifstream& file) {
        std::stringstream stream_without_comments;
        std::string line;
        while (std::getline(file, line)) {
            auto sharp = line.find('#');
            if (sharp != std::string::npos) {
                if (sharp != 0) {
                    stream_without_comments << line.substr(0, sharp) << std::endl;
                }
            } else {
                stream_without_comments << line << std::endl;
            }
        }
        return stream_without_comments;
    }

    std::unique_ptr<VectorBase> parse_vector(std::stringstream& ss) {
        std::string element_type;
        ss >> element_type;
        if (element_type == "complex") {
            std::string type_real, type_image;
            ss >> type_real >> type_image;
            if (type_real == "integer" && type_image == "integer") {
                unsigned int vec_size;
                ss >> vec_size;
                Vector<Complex<long long, long long>> v(vec_size);
                unsigned int coord;
                while (true) {
                    ss >> coord;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1)); // delete ()
                    try {
                        Complex<long long> element(value.c_str());
                        v.set(coord - 1, element);
                    } catch (InvalidArgument e) {
                        throw WrongFileFormat("aaaaa" + e.what);
                    }
                }
                return std::make_unique<Vector<Complex<long long>>>(v);
            } else if (type_real == "integer" && type_image == "float") {
                unsigned int vec_size;
                ss >> vec_size;
                Vector<Complex<long long, double>> v(vec_size);
                unsigned int coord;
                while (true) {
                    ss >> coord;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                    try {
                        Complex<long long, double> element(value.c_str());
                        v.set(coord - 1, element);
                    } catch (InvalidArgument) {
                        throw WrongFileFormat("bbbbb");
                    }
                }
                return std::make_unique<Vector<Complex<long long, double>>>(v);
            } else if (type_real == "float" && type_image == "integer") {
                unsigned int vec_size;
                ss >> vec_size;
                Vector<Complex<double, long long>> v(vec_size);
                unsigned int coord;
                while (true) {
                    ss >> coord;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                    try {
                        Complex<double, long long> element(value.c_str());
                        v.set(coord - 1, element);
                    } catch (InvalidArgument) {
                        throw WrongFileFormat("ccccc");
                    }
                }
                return std::make_unique<Vector<Complex<double, long long>>>(v);
            } else if (type_real == "float" && type_image == "float") {
                unsigned int vec_size;
                ss >> vec_size;
                Vector<Complex<double>> v(vec_size);
                unsigned int coord;
                while (true) {
                    ss >> coord;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                    try {
                        Complex<double> element(value.c_str());
                        v.set(coord - 1, element);
                    } catch (InvalidArgument) {
                        throw WrongFileFormat("dddd");
                    }
                }
                return std::make_unique<Vector<Complex<double>>>(v);
            } else {
                throw WrongFileFormat("eeeeee");
            }
        } else if (element_type == "rational") {
            unsigned int vec_size;
            ss >> vec_size;
            Vector<Rational<long long>> v(vec_size); 
            unsigned int coord;
            while (true) {
                ss >> coord;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                try {
                    Rational<long long> element(value.c_str());
                    v.set(coord - 1, element);
                } catch (InvalidArgument e) {
                    throw WrongFileFormat("ffff");
                } catch (DivisionByZeroException<long long> e) {
                    throw WrongFileFormat("ggggg");
                }
            }
            return std::make_unique<Vector<Rational<long long>>>(v);
        } else if (element_type == "bit") {
            
            unsigned int vec_size;
            ss >> vec_size;
            Vector<bool> v(vec_size); 
            unsigned int coord;
            while (true) {
                ss >> coord;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                try {
                    
                    v.set(coord - 1, 1);
                    
                } catch (InvalidArgument e) {
                    throw WrongFileFormat("ffff");
                } catch (DivisionByZeroException<long long> e) {
                    throw WrongFileFormat("ggggg");
                }
            }
            return std::make_unique<Vector<bool>>(v);
        } else {
            return 0;
        }
    }

    std::unique_ptr<MatrixBase> parse_matrix(std::stringstream& ss) {
        std::string element_type;
        ss >> element_type;
        if (element_type == "complex") {
            std::string type_real, type_image;
            ss >> type_real >> type_image;
            if (type_real == "integer" && type_image == "integer") {
                unsigned int rows, columns;
                ss >> rows >> columns;
                Matrix<Complex<long long, long long>> v(rows, columns);
                unsigned int row, col;
                while (true) {
                    ss >> row >> col;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1)); // delete ()
                    try {
                        Complex<long long> element(value.c_str());
                        v.set(row - 1, col - 1, element);
                    } catch (InvalidArgument e) {
                        throw WrongFileFormat("aaaaa" + e.what);
                    }
                }
                return std::make_unique<Matrix<Complex<long long>>>(v);
            } else if (type_real == "integer" && type_image == "float") {
                unsigned int rows, columns;
                ss >> rows >> columns;
                Matrix<Complex<long long, double>> v(rows, columns);
                unsigned int row, col;
                while (true) {
                    ss >> row >> col;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                    try {
                        Complex<long long, double> element(value.c_str());
                        v.set(row - 1, col - 1, element);
                    } catch (InvalidArgument) {
                        throw WrongFileFormat("bbbbb");
                    }
                }
                return std::make_unique<Matrix<Complex<long long, double>>>(v);
            } else if (type_real == "float" && type_image == "integer") {
                unsigned int rows, columns;
                ss >> rows >> columns;
                Matrix<Complex<double, long long>> v(rows, columns);
                unsigned int row, col;
                while (true) {
                    ss >> row >> col;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                    try {
                        Complex<double, long long> element(value.c_str());
                        v.set(row - 1, col - 1, element);
                    } catch (InvalidArgument) {
                        throw WrongFileFormat("ccccc");
                    }
                }
                return std::make_unique<Matrix<Complex<double, long long>>>(v);
            } else if (type_real == "float" && type_image == "float") {
                unsigned int rows, columns;
                ss >> rows >> columns;
                Matrix<Complex<double, double>> v(rows, columns);
                unsigned int row, col;
                while (true) {
                    ss >> row >> col;
                    if (ss.fail() || ss.eof()) {
                        break;
                    }
                    std::string value;
                    ss >> value;
                    value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                    try {
                        Complex<double> element(value.c_str());
                        v.set(row - 1, col - 1, element);
                    } catch (InvalidArgument) {
                        throw WrongFileFormat("dddd");
                    }
                }
                return std::make_unique<Matrix<Complex<double>>>(v);
            } else {
                throw WrongFileFormat("eeeeee");
            }
        } else if (element_type == "rational") {
            std::cout << "a" << std::endl;
            unsigned int rows, columns;
            ss >> rows >> columns;
            Matrix<Rational<long long>> v(rows, columns); 
            unsigned int row, column;
            while (true) {
                ss >> row >> column;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                try {
                    Rational<long long> element(value.c_str());
                    v.set(row - 1, column - 1, element);
                } catch (InvalidArgument e) {
                    throw WrongFileFormat("ffff");
                } catch (DivisionByZeroException<long long> e) {
                    throw WrongFileFormat("ggggg");
                }
            }
            return std::make_unique<Matrix<Rational<long long>>>(v);
        } else if (element_type == "bit") {
            
            unsigned int rows, columns;
            ss >> rows >> columns;
            Matrix<bool> v(rows, columns); 
            unsigned int row, column;
            while (true) {
                ss >> row >> column;
                if (ss.fail() || ss.eof()) {
                    break;
                }
                std::string value;
                ss >> value;
                value = std::string(value.begin() + 1, value.begin() + (value.size() - 1));
                try {
                    
                    v.set(row - 1, column - 1, 1);
                    
                } catch (InvalidArgument e) {
                    throw WrongFileFormat("ffff");
                } catch (DivisionByZeroException<long long> e) {
                    throw WrongFileFormat("ggggg");
                }
            }
            return std::make_unique<Matrix<bool>>(v);
        } else {
            return 0;
        }
    }

    std::unique_ptr<Base> parse_file(const std::string filename) {
        std::ifstream file(filename);
        std::stringstream stream_without_comments = delete_comments(file);
        std::string file_data_type;
        stream_without_comments >> file_data_type;
        if (file_data_type == "vector") {
            return parse_vector(stream_without_comments);
        } else if (file_data_type == "matrix") {
            return parse_matrix(stream_without_comments);
            
        }
        return 0;
    }
};

template <typename T>
bool try_cast_and_print(Base* base_p, const std::string& type_name) {
    auto derived1 = dynamic_cast<Vector<T>*>(base_p);
    if (derived1 && type_name[0] == 'V') {
        std::cout << "Парсинг успешен: объект - " << type_name << "." << std::endl;
        std::cout << derived1->to_string();
        return true;
    }
    auto derived2 = dynamic_cast<Matrix<T>*>(base_p);
    if (derived2  && type_name[0] == 'M') {
        std::cout << "Парсинг успешен: объект - " << type_name << "." << std::endl;
        std::cout << derived2->to_string();
        return true;
    }
    return false;
}

int main() {
    FileParser fp;
    for (int j = 0; j < 2; ++j) {
        std::string temp = (j == 0 ? "vector_" : "matrix_");
        for (int i = 1; i <= 6; ++i) {
            std::string filename = "test_" + temp + std::to_string(i) + ".txt";
            std::cout << "Обработка файла: " << filename << std::endl;

            try {
                auto base_p = fp.parse_file(filename);
                if (!base_p) {
                    std::cerr << "Парсинг не удался или файл пуст." << std::endl;
                    continue;
                }
                std::cout << "Тип объекта в base_p: " << typeid(*base_p).name() << std::endl;

                
                if (try_cast_and_print<Rational<long long>>(base_p.get(), "Vector<Rational<int>>") ||
                    try_cast_and_print<bool>(base_p.get(), "Vector<bool>") ||
                    try_cast_and_print<Complex<long long>>(base_p.get(), "Vector<Complex<int, int>>") ||
                    try_cast_and_print<Complex<long long, double>>(base_p.get(), "Vector<Complex<int, double>>") ||
                    try_cast_and_print<Complex<double, long long>>(base_p.get(), "Vector<Complex<double, int>>") ||
                    try_cast_and_print<Complex<double>>(base_p.get(), "Vector<Complex<double, double>>") ||
                    try_cast_and_print<Rational<long long>>(base_p.get(), "Matrix<Rational<int>>") ||
                    try_cast_and_print<bool>(base_p.get(), "Matrix<bool>") ||
                    try_cast_and_print<Complex<long long>>(base_p.get(), "Matrix<Complex<int, int>>") ||
                    try_cast_and_print<Complex<long long, double>>(base_p.get(), "Matrix<Complex<int, double>>") ||
                    try_cast_and_print<Complex<double, long long>>(base_p.get(), "Matrix<Complex<double, int>>") ||
                    try_cast_and_print<Complex<double>>(base_p.get(), "Matrix<Complex<double, double>>")) {
                    std::cout << "----------------------------" << std::endl;
                    continue;
                }

                std::cerr << "Не удалось определить тип объекта." << std::endl;

            } catch (const WrongFileFormat& e) {
                std::cerr << "Неверный формат файла: " << e.what << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Ошибка: " << e.what() << std::endl;
            }

        }
    }

    return 0;
}


















































