#pragma once
#include <gtest/gtest.h>
#include <cmath>
#include <cstdint>
#include <stdexcept>
#include "Exceptions.hpp"
#include "Rational_number.hpp"
#include "Utils.hpp"
#include "Complex_number.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Matrix_coords.hpp"
#include "Matrix_proxy.hpp"

TEST(Utils, Utils) {
    EXPECT_TRUE(isInteger("5"));
    EXPECT_TRUE(isInteger("-5"));
    EXPECT_TRUE(isInteger("- 5"));
    EXPECT_TRUE(isInteger("  -\t5 \t "));
    EXPECT_FALSE(isInteger("5."));
    EXPECT_TRUE(isNumber("5"));
    EXPECT_TRUE(isNumber("-5"));
    EXPECT_TRUE(isNumber("5."));
    EXPECT_TRUE(isNumber("-5."));
    EXPECT_TRUE(isNumber("-5.67"));
    EXPECT_EQ(parseInteger<int>("5"), 5);
    EXPECT_EQ(parseInteger<int>("-5"), -5);
    EXPECT_DOUBLE_EQ(parseDouble("5"), 5);
    EXPECT_DOUBLE_EQ(parseDouble("-5"), -5);
    EXPECT_DOUBLE_EQ(parseDouble("-5."), -5);
    EXPECT_DOUBLE_EQ(parseDouble("-5.56"), -5.56);
    EXPECT_DOUBLE_EQ(parseDouble("5."), 5);

}

TEST(Rational_numbers, Constructors) {
    Rational_number r1(5);
    EXPECT_TRUE(r1.getNumerator() == 5 && r1.getDenominator() == 1);
    Rational_number r2(5, 2);
    EXPECT_TRUE(r2.getNumerator() == 5 && r2.getDenominator() == 2);
    Rational_number<int> r3("5");
    EXPECT_TRUE(r3.getNumerator() == 5 && r3.getDenominator() == 1);
    Rational_number<int> r4("5/2");
    EXPECT_TRUE(r4.getNumerator() == 5 && r4.getDenominator() == 2);
    Rational_number<int> r5;
    EXPECT_TRUE(r5.getNumerator() == 0 && r5.getDenominator() == 1);
    EXPECT_THROW(Rational_number<int>("5/0"), ZeroDivision);
    EXPECT_THROW(Rational_number(1, 0), ZeroDivision);
    EXPECT_THROW(Rational_number<int>(std::to_string(std::string::npos)), Overflow); // переполняем int


    Rational_number<int> r6("-5/2");
    EXPECT_TRUE(r6.getNumerator() == -5 && r6.getDenominator() == 2);

}

TEST(Rational_numbers, NumberChanging) {
    Rational_number<int> r1;
    EXPECT_THROW(r1.setDenominator(0), ZeroDivision);
    Rational_number r2(10, 5);
    Rational_number r3 = r2.make_canonical();
    EXPECT_TRUE(r3.getNumerator() == 2 && r3.getDenominator() == 1);
    Rational_number r4(3, 2);
    EXPECT_EQ((int)r4, 1);
    EXPECT_DOUBLE_EQ((double)r4, 1.5);
    EXPECT_EQ(r4.round(), 2);
    EXPECT_EQ(r4.floor(), 1);
}

TEST(Rational_numbers, Arythmetic) {
    Rational_number r1(10, 5);
    Rational_number r2(2, 3);
    EXPECT_EQ(r1 + r2, Rational_number(10*3 + 2*5, 5*3));
    EXPECT_EQ(r1 + Rational_number<unsigned>(2, 3), Rational_number(10*3 + 2*5, 5*3)); // проверка, что можно скложить числа с разным ипараметрами шаблона
    EXPECT_EQ(r1 - r2, Rational_number(10*3 - 2*5, 5*3));
    EXPECT_EQ(r1 - Rational_number<unsigned>(2, 3), Rational_number(10*3 - 2*5, 5*3));
    EXPECT_EQ(r1 * r2, Rational_number(10*2, 5*3));
    EXPECT_EQ(r1 / r2, Rational_number(10*3, 5*2));

    EXPECT_EQ(r1 + 1, Rational_number(15, 5));
    EXPECT_EQ(r1 - 1, Rational_number(5, 5));
    EXPECT_EQ(r1 += 1, Rational_number(15, 5));
    EXPECT_EQ(r1 -= 1, Rational_number(10, 5));

    EXPECT_EQ(r1 * 2, Rational_number(20, 5));
    EXPECT_EQ(r1 / 2, Rational_number(5, 5));
    EXPECT_EQ(r1 *= 2, Rational_number(20, 5));
    EXPECT_EQ(r1 /= 2, Rational_number(10, 5));

    EXPECT_EQ(1 + r1, Rational_number(15, 5));
    EXPECT_EQ(1 - r1, Rational_number(-5, 5));
    EXPECT_EQ(2 * r1, Rational_number(20, 5));
    EXPECT_EQ(2 / r1, Rational_number(1));

    EXPECT_EQ(-r1, Rational_number(-10, 5));
    EXPECT_DOUBLE_EQ(Rational_number(3, 2), 1.5);
}

TEST(Rational_numbers, LogicOperators) {
    Rational_number r1(10, 5);
    Rational_number r2(2, 3);
    EXPECT_TRUE(r1 > r2);
    EXPECT_TRUE(r1 >= r2);
    EXPECT_TRUE(r2 < r1);
    EXPECT_TRUE(r2 <= r1);
    EXPECT_TRUE(r2 != r1);
    EXPECT_FALSE(r1 == r2);
    EXPECT_EQ(r1, Rational_number(2));

    EXPECT_TRUE (r1 < 2.7);
    EXPECT_TRUE (r1 <= 2.7);
    EXPECT_TRUE (r1 > 1.7);
    EXPECT_TRUE (r1 >= 1.7);
    EXPECT_TRUE(r1 != 2.1);
    EXPECT_TRUE(r1 == 2.);
}

TEST(Rational_numbers, OtherOperators) {
    Rational_number r1(3, 2);
    Rational_number r2 = r1++;
    EXPECT_EQ(r1, Rational_number(5, 2));
    EXPECT_EQ(r2, Rational_number(3, 2));
    r2 = r1--;
    EXPECT_EQ(r1, Rational_number(3, 2));
    EXPECT_EQ(r2, Rational_number(5, 2));
}

TEST(Complex_numbers, Constructors) {
    Complex_number c1;
    EXPECT_TRUE(c1.getReal() == 0 && c1.getImage() == 0);
    Complex_number c2(1);
    EXPECT_TRUE(c2.getReal() == 1 && c2.getImage() == 0);
    Complex_number c3(1, 2);
    EXPECT_TRUE(c3.getReal() == 1 && c3.getImage() == 2);
    Complex_number<> c4("5, 5");
    EXPECT_TRUE(c4.getReal() == 5 && c4.getImage() == 5);
    Complex_number<> c5("5");
    EXPECT_TRUE(c5.getReal() == 5 && c5.getImage() == 0);
    Complex_number<> c6("5.5");
    EXPECT_TRUE(c6.getReal() == 5.5 && c6.getImage() == 0);
    Complex_number<> c7("5.5,6.6");
    EXPECT_TRUE(c7.getReal() == 5.5 && c7.getImage() == 6.6);
    Complex_number<> c8("5 .\t5\t, 6 . 6 ");
    EXPECT_TRUE(c8.getReal() == 5.5 && c8.getImage() == 6.6);
}

TEST(Complex_numbers, Arythmetic) {
    Complex_number c1(5, 5);
    Complex_number<> c2(2, 2);
    EXPECT_EQ(c1 + c2, Complex_number(7, 7));
    EXPECT_EQ(c1 - c2, Complex_number(3, 3));
    EXPECT_EQ(c1 * c2, Complex_number(0, 20));
    EXPECT_EQ(c1 / c2, Complex_number(2, 0));
    c2.setReal(2.5);
    c2.setImage(2.5);
    // приведение к левостороннему значению
    EXPECT_EQ(c1 + c2, Complex_number(7, 7));
    EXPECT_EQ(c2 + c1, Complex_number(7.5, 7.5));

    EXPECT_EQ(c1 + 1, Complex_number(6, 5));
    EXPECT_EQ(c1 - 1, Complex_number(4, 5));
    EXPECT_EQ(c1 * 2, Complex_number(10, 10));
    EXPECT_EQ(c1 / 2, Complex_number(2, 2));
    EXPECT_EQ(c2 / 2, Complex_number(1.25, 1.25));

    EXPECT_EQ(c1 + 1., Complex_number(6, 5));
    EXPECT_EQ(c1 - 1., Complex_number(4, 5));
    EXPECT_EQ(c1 * 2., Complex_number(10, 10));
    EXPECT_EQ(c1 / 2, Complex_number(2, 2));
    EXPECT_EQ(c2 / 2., Complex_number(1.25, 1.25));

    EXPECT_EQ(c1 + 1.5, Complex_number(6, 5));
    EXPECT_EQ(c1 - 1.5, Complex_number(4, 5));
    EXPECT_EQ(c1 * 2.5, Complex_number(10, 10));
    EXPECT_EQ(c2 / 7.5, Complex_number(1./3, 1./3));

    EXPECT_EQ(c1 + 1, Complex_number(6, 5));
    EXPECT_EQ(c1 - 1, Complex_number(4, 5));
    EXPECT_EQ(c1 * 2, Complex_number(10, 10));
    EXPECT_EQ(c1 / 2, Complex_number(2, 2));
    EXPECT_EQ(c2 / 2, Complex_number(1.25, 1.25));

    EXPECT_EQ(c1 + 1., Complex_number(6, 5));
    EXPECT_EQ(c1 - 1., Complex_number(4, 5));
    EXPECT_EQ(c1 * 2., Complex_number(10, 10));
    EXPECT_EQ(c1 / 2, Complex_number(2, 2));
    EXPECT_EQ(c2 / 2., Complex_number(1.25, 1.25));

    EXPECT_EQ(c1 + 1.5, Complex_number(6, 5));
    EXPECT_EQ(c1 - 1.5, Complex_number(4, 5));
    EXPECT_EQ(c1 * 2.5, Complex_number(10, 10));
    EXPECT_EQ(c2 / 7.5, Complex_number(1./3, 1./3));


// Операторы приводят к типу левого операнда, поэтому для "математической корректности"
// параметризуем комплексное и рациональное числа конкретными типами. Если рациональное будет
// параметризовано чем-то другим, то из-за отсутствия оператора приведения 
// (пр. Rat<int> ----> Rat<long>) код не скомпилируется:

// Complex_number.hpp:116:18: error: no matching function for call to ‘Complex_number<Rational_number<> >::Complex_number(const Rational_number<int>&)’
//   116 |     return lhs + Complex_number<lhs_real, lhs_imag>(rhs);

    Complex_number<Rational_number<>> c(1, 1); 
    Rational_number<> r(5, 3);

    Complex_number sum1(Rational_number(8, 3), 1);
    EXPECT_EQ(c + r, sum1);
    Complex_number sub1(Rational_number(-2, 3), 1);
    EXPECT_EQ(c - r, sub1);
    Complex_number mul1(Rational_number(5, 3), Rational_number(5, 3));
    EXPECT_EQ(c * r, mul1);
    Complex_number div1(Rational_number(3, 5), Rational_number(3, 5));
    EXPECT_EQ(c / r, div1);

    Complex_number sum2(Rational_number(8, 3), 1);
    EXPECT_EQ(r + c, sum2);
    Complex_number sub2(Rational_number(2, 3), -1);
    EXPECT_EQ(r - c, sub2);
    Complex_number mul2(Rational_number(5, 3), Rational_number(5, 3));
    EXPECT_EQ(r * c, mul2);
    Complex_number div2(Rational_number(5, 6), Rational_number(-5, 6));
    EXPECT_EQ(r / c, div2);

    Complex_number sum22(1.0, 1.0);
    EXPECT_EQ(sum22 + Rational_number(1, 2), Complex_number(1.5, 1));
}

TEST(Complex_numbers, LogicalOperators) {
    Complex_number c1(1, 1);
    Complex_number c2(-1, 1.5);
    Complex_number c3(3);
    EXPECT_EQ(c1, Complex_number(1, 1));
    EXPECT_NE(c1, c2);
    EXPECT_TRUE(c1 < c2);
    EXPECT_TRUE(c1 <= c2);
    EXPECT_TRUE(c2 > c1);
    EXPECT_TRUE(c2 >= c1);

    EXPECT_NE(c1, std::sqrt(2));
    EXPECT_TRUE(c1 < 1.5);
    EXPECT_TRUE(c1 <= 1.5);
    EXPECT_TRUE(c1 > 1);
    EXPECT_TRUE(c1 >= 1);

    Rational_number r(6, 2);
    EXPECT_EQ(c3, r);
    EXPECT_TRUE(c3 != r + 1);
    EXPECT_FALSE(c3 < r);
    EXPECT_TRUE(c3 <= r);
    EXPECT_FALSE(c3 > r);
    EXPECT_TRUE(c3 >= r);

    EXPECT_EQ(r, c3);
    EXPECT_TRUE(r + 1 != c3);
    EXPECT_FALSE(r > c3);
    EXPECT_TRUE(r >= c3);
    EXPECT_FALSE(r < c3);
    EXPECT_TRUE(r <= c3);
}

TEST(Vectors, Constructors) {
    Vector v1(10, 0.1, 2);
    EXPECT_EQ(v1(4), 2);
    Vector v2(v1);
    Vector v3(std::move(Vector(5, 1, 5)));
    Vector v4(5);
    EXPECT_EQ(v4.getSize(), 5);
    EXPECT_EQ(v4.getEpsilon(), 0);
    Vector v5(5, 0.1);
    EXPECT_EQ(v5.getSize(), 5);
    EXPECT_DOUBLE_EQ(v5.getEpsilon(), 0.1);
}

TEST(Vectors, Operators) {
    Vector v1(10, 0.1, 1);
    Vector v2(10, 0.2, (long long)2);
    Vector sum12(10, 0.2, 3);
    EXPECT_TRUE(v1 != v2);
    EXPECT_FALSE(v1 == v2);
    EXPECT_EQ(v2+v1, sum12);
    EXPECT_EQ(sum12-v1, v2);

    Vector sum_scalar1(10, 0.1, 2);
    Vector sub_scalar1(10, 0.2, -1);
    Vector mul_scalar1(10, 0.2, 10);
    Vector div_scalar1(10, 0.2, 1);

    EXPECT_EQ(v1 + 1, sum_scalar1);
    EXPECT_EQ(1 + v1, sum_scalar1);
    
    EXPECT_EQ(v2 - 3, sub_scalar1);
    EXPECT_EQ(1 - v2, sub_scalar1);
    
    EXPECT_EQ(v2 * 5, mul_scalar1);
    EXPECT_EQ(5 * v2, mul_scalar1);
    
    EXPECT_EQ(v2 / 2, div_scalar1);
    EXPECT_EQ(2 / v2, div_scalar1);

    
    Vector v3(10, 0.2, 16.);
    Vector sum_scalar2(10, 0.2, 17.);
    Vector sub_scalar2(10, 0.2, -8.);
    Vector mul_scalar2(10, 0.2, 32.);
    Vector div_scalar2(10, 0.2, 2.);

    EXPECT_EQ(v3 + 1, sum_scalar2);
    EXPECT_EQ(1 + v3, sum_scalar2);
    
    EXPECT_EQ(v3 - 24, sub_scalar2);
    EXPECT_EQ(8 - v3, sub_scalar2);
    
    EXPECT_EQ(v3 * 2, mul_scalar2);
    EXPECT_EQ(2 * v3, mul_scalar2);
    
    EXPECT_EQ(v3 / 8, div_scalar2);
    EXPECT_EQ(32 / v3, div_scalar2);

    Vector v4(10, 0.2, Rational_number(5, 4));
    Vector sum_scalar3(10, 0.2, Rational_number(6, 4));
    Vector sub_scalar3(10, 0.2, Rational_number(3, 4));
    Vector mul_scalar3(10, 0.2, Rational_number(10, 4));
    Vector div_scalar3(10, 0.2, Rational_number(1, 4));

    EXPECT_EQ(v4 + Rational_number(1, 4), sum_scalar3);
    EXPECT_EQ(Rational_number(1, 4) + v4, sum_scalar3);
    
    EXPECT_EQ(v4 - Rational_number(2, 4), sub_scalar3);
    EXPECT_EQ(Rational_number(8, 4) - v4, sub_scalar3);
    
    EXPECT_EQ(v4 * 2, mul_scalar3);
    EXPECT_EQ(2 * v4, mul_scalar3);
    
    EXPECT_EQ(v4 / 5, div_scalar3);
    EXPECT_EQ(Rational_number(5, 16) / v4, div_scalar3);


    Vector v5(10, 0.2, 4);
    Vector sum_scalar4(10, 0.2, Rational_number(17, 4));
    Vector sub_scalar4(10, 0.2, Rational_number(7, 2));
    Vector mul_scalar4(10, 0.2, Rational_number(8));
    Vector div_scalar4(10, 0.2, Rational_number(1));

    EXPECT_EQ(v5 + Rational_number(1, 4), sum_scalar4);
    EXPECT_EQ(Rational_number(1, 4) + v5, sum_scalar4);

    EXPECT_EQ(v5 - Rational_number(2, 4), sub_scalar4);
    EXPECT_EQ(Rational_number(15, 2) - v5, sub_scalar4);
    
    EXPECT_EQ(v5 * Rational_number(2), mul_scalar4);
    EXPECT_EQ(Rational_number(2) * v5, mul_scalar4);
    
    EXPECT_EQ(v5 / 4, div_scalar4);
    EXPECT_EQ(Rational_number(4) / v5, div_scalar4);
}

TEST(Vectors, Complex) {
    // Работает только если типы комплексных чсел одинаковы, потому что иначе нужно было бы делать еще кучу 
    // специализаций шаблонных операторов (по крайней мере эта реализация того требует). И операции со скаляром
    // слева(для случаев когда вектор<комп<рац, ...>>, вектор<комп<..., рац>>, вектор<комп<рац, рац>>, а скаляры 
    // - комплексные) отсутствуют по той же причине 
    Vector v1(10, 0.1, Complex_number(Rational_number(6, 2), Rational_number(2)));
    Vector sum_scalar1(10, 0.1, Complex_number(Rational_number(11, 2), Rational_number(3)));
    Vector sub_scalar1(10, 0.1, Complex_number(Rational_number(1, 2), Rational_number(1)));
    Vector mul_scalar1(10, 0.1, Complex_number(Rational_number(11, 2), Rational_number(8)));
    Vector div_scalar1(10, 0.1, Complex_number(Rational_number(152, 116), Rational_number(32,116)));

    Complex_number c(Rational_number(5, 2), Rational_number(1));
    // std::cout << v1 + Complex_number(Rational_number(5, 2), Rational_number(5, 2)) << std::endl;
    EXPECT_EQ(v1 + c, sum_scalar1);
    EXPECT_EQ(v1 - c, sub_scalar1);
    EXPECT_EQ(v1 * c, mul_scalar1);
    EXPECT_EQ(v1 / c, div_scalar1);


    Vector v4(10, 0.2, Complex_number(5, 4));
    Vector sum_scalar3(10, 0.2, Complex_number(6, 8));
    Vector sub_scalar3(10, 0.2, Complex_number(3, 0));
    Vector mul_scalar3(10, 0.2, Complex_number(10, 8));
    Vector div_scalar3(10, 0.2, Complex_number(1));

    EXPECT_EQ(v4 + Complex_number(1, 4), sum_scalar3);
    EXPECT_EQ(Complex_number(1, 4) + v4, sum_scalar3);
    
    EXPECT_EQ(v4 - Complex_number(2, 4), sub_scalar3);
    EXPECT_EQ(Complex_number(8, 4) - v4, sub_scalar3);
    
    EXPECT_EQ(v4 * Complex_number(2), mul_scalar3);
    EXPECT_EQ(Complex_number(2) * v4, mul_scalar3);
    
    EXPECT_EQ(v4 / 5, div_scalar3);
    EXPECT_EQ(Complex_number(5, 4) / v4, div_scalar3);
}

TEST(Vectors, BoolVector) {
    Vector<bool> v1(5);
    Vector<bool> v2(5, 0);
    Vector<bool> v3(5, 1);

    EXPECT_EQ(v1(3), 0);
    EXPECT_EQ(v2(3), 0);
    EXPECT_EQ(v3(3), 1);
    v3.set(3, 0);
    EXPECT_EQ(v3(3), 0);

    EXPECT_THROW(v1(5), VectorIndexOutOfRangeException);

    EXPECT_EQ(v2 * v3, v2);
    EXPECT_EQ(v2 + v3, v3);

    Vector<bool> v4(4);
    EXPECT_THROW(v4 + v3, VectorSizeMissmatch);
    EXPECT_THROW(v4 * v3, VectorSizeMissmatch);


}

TEST(Matrixs, Constructors) {
    Matrix m1(10, 10, 0.1, 2);
    EXPECT_EQ(m1(4, 4), 2);
    Matrix m2(m1);
    Matrix m3(std::move(Matrix(5, 1, 5)));
    Matrix m4(5, 5);
    EXPECT_EQ(m4.getRows(), 5);
    EXPECT_EQ(m4.getColumns(), 5);
    EXPECT_EQ(m4.getEpsilon(), 0);
    Matrix m5(5, 6, 0.1);
    EXPECT_EQ(m5.getRows(), 5);
    EXPECT_EQ(m5.getColumns(), 6);
    EXPECT_DOUBLE_EQ(m5.getEpsilon(), 0.1);
}

TEST(Matrixs, Operators) {
    Matrix simple1(10, 3, 0.1);
    Matrix simple2(10, 3, 0.2);
    simple1.set(0, 0, 1);
    simple1.set(1, 1, 2);
    simple1.set(2, 2, 3);
    simple2.set(1, 0, 0.15);
    simple2.set(2, 1, 20);
    simple2.set(3, 2, 30);
    Matrix simple_sum(10, 3, 0.1);
    simple_sum.set(0, 0, 1);
    simple_sum.set(1, 1, 2);
    simple_sum.set(2, 2, 3);
    simple_sum.set(2, 1, 20);
    simple_sum.set(3, 2, 30);
    EXPECT_EQ(simple1 + simple2, simple_sum);


    Matrix m1(10, 10, 0.1, 1);
    Matrix m2(10, 10, 0.2, 2LL);
    Matrix sum12(10, 10, 0.2, 3);
    EXPECT_TRUE(m1 != m2);
    EXPECT_FALSE(m1 == m2);
    EXPECT_EQ(m2+m1, sum12);
    EXPECT_EQ(sum12-m1, m2);

    Matrix sum_scalar1(10, 10, 0.1, 2);
    Matrix sub_scalar1(10, 10, 0.2, -1);
    Matrix mul_scalar1(10, 10, 0.2, 10);
    Matrix div_scalar1(10, 10, 0.2, 1);

    EXPECT_EQ(m1 + 1, sum_scalar1);
    EXPECT_EQ(1 + m1, sum_scalar1);
    
    EXPECT_EQ(m2 - 3, sub_scalar1);
    EXPECT_EQ(1 - m2, sub_scalar1);
    
    EXPECT_EQ(m2 * 5, mul_scalar1);
    EXPECT_EQ(5 * m2, mul_scalar1);
    
    EXPECT_EQ(m2 / 2, div_scalar1);
    EXPECT_EQ(2 / m2, div_scalar1);

    
    Matrix m3(10, 10, 0.2, 16.);
    Matrix sum_scalar2(10, 10, 0.2, 17.);
    Matrix sub_scalar2(10, 10, 0.2, -8.);
    Matrix mul_scalar2(10, 10, 0.2, 32.);
    Matrix div_scalar2(10, 10, 0.2, 2.);

    EXPECT_EQ(m3 + 1, sum_scalar2);
    EXPECT_EQ(1 + m3, sum_scalar2);
    
    EXPECT_EQ(m3 - 24, sub_scalar2);
    EXPECT_EQ(8 - m3, sub_scalar2);
    
    EXPECT_EQ(m3 * 2, mul_scalar2);
    EXPECT_EQ(2 * m3, mul_scalar2);
    
    EXPECT_EQ(m3 / 8, div_scalar2);
    EXPECT_EQ(32 / m3, div_scalar2);

    Matrix m4(10, 10, 0.2, Rational_number(5, 4));
    Matrix sum_scalar3(10, 10, 0.2, Rational_number(6, 4));
    Matrix sub_scalar3(10, 10, 0.2, Rational_number(3, 4));
    Matrix mul_scalar3(10, 10, 0.2, Rational_number(10, 4));
    Matrix div_scalar3(10, 10, 0.2, Rational_number(1, 4));

    EXPECT_EQ(m4 + Rational_number(1, 4), sum_scalar3);
    EXPECT_EQ(Rational_number(1, 4) + m4, sum_scalar3);
    
    EXPECT_EQ(m4 - Rational_number(2, 4), sub_scalar3);
    EXPECT_EQ(Rational_number(8, 4) - m4, sub_scalar3);
    
    EXPECT_EQ(m4 * 2, mul_scalar3);
    EXPECT_EQ(2 * m4, mul_scalar3);
    
    EXPECT_EQ(m4 / 5, div_scalar3);
    EXPECT_EQ(Rational_number(5, 16) / m4, div_scalar3);


    Matrix m5(10, 10, 0.2, 4);
    Matrix sum_scalar4(10, 10, 0.2, Rational_number(17, 4));
    Matrix sub_scalar4(10, 10, 0.2, Rational_number(7, 2));
    Matrix mul_scalar4(10, 10, 0.2, Rational_number(8));
    Matrix div_scalar4(10, 10, 0.2, Rational_number(1));

    EXPECT_EQ(m5 + Rational_number(1, 4), sum_scalar4);
    EXPECT_EQ(Rational_number(1, 4) + m5, sum_scalar4);

    EXPECT_EQ(m5 - Rational_number(2, 4), sub_scalar4);
    EXPECT_EQ(Rational_number(15, 2) - m5, sub_scalar4);
    
    EXPECT_EQ(m5 * Rational_number(2), mul_scalar4);
    EXPECT_EQ(Rational_number(2) * m5, mul_scalar4);
    
    EXPECT_EQ(m5 / 4, div_scalar4);
    EXPECT_EQ(Rational_number(4) / m5, div_scalar4);
}

TEST(Matrixs, Complex) {
    // Работает только если типы комплексных чсел одинаковы, потому что иначе нужно было бы делать еще кучу 
    // специализаций шаблонных операторов (по крайней мере эта реализация того требует). И операции со скаляром
    // слева(для случаев когда вектор<комп<рац, ...>>, вектор<комп<..., рац>>, вектор<комп<рац, рац>>, а скаляры 
    // - комплексные) отсутствуют по той же причине 
    Matrix m1(10, 10, 0.1, Complex_number(Rational_number(6, 2), Rational_number(2)));
    Matrix sum_scalar1(10, 10, 0.1, Complex_number(Rational_number(11, 2), Rational_number(3)));
    Matrix sub_scalar1(10, 10, 0.1, Complex_number(Rational_number(1, 2), Rational_number(1)));
    Matrix mul_scalar1(10, 10, 0.1, Complex_number(Rational_number(11, 2), Rational_number(8)));
    Matrix div_scalar1(10, 10, 0.1, Complex_number(Rational_number(152, 116), Rational_number(32,116)));

    Complex_number c(Rational_number(5, 2), Rational_number(1));
    // std::cout << m1 + Complex_number(Rational_number(5, 2), Rational_number(5, 2)) << std::endl;
    EXPECT_EQ(m1 + c, sum_scalar1);
    EXPECT_EQ(m1 - c, sub_scalar1);
    EXPECT_EQ(m1 * c, mul_scalar1);
    EXPECT_EQ(m1 / c, div_scalar1);


    Matrix m4(10, 10, 0.2, Complex_number(5, 4));
    Matrix sum_scalar3(10, 10, 0.2, Complex_number(6, 8));
    Matrix sub_scalar3(10, 10, 0.2, Complex_number(3, 0));
    Matrix mul_scalar3(10, 10, 0.2, Complex_number(10, 8));
    Matrix div_scalar3(10, 10, 0.2, Complex_number(1));

    EXPECT_EQ(m4 + Complex_number(1, 4), sum_scalar3);
    EXPECT_EQ(Complex_number(1, 4) + m4, sum_scalar3);
    
    EXPECT_EQ(m4 - Complex_number(2, 4), sub_scalar3);
    EXPECT_EQ(Complex_number(8, 4) - m4, sub_scalar3);
    
    EXPECT_EQ(m4 * Complex_number(2), mul_scalar3);
    EXPECT_EQ(Complex_number(2) * m4, mul_scalar3);
    
    EXPECT_EQ(m4 / 5, div_scalar3);
    EXPECT_EQ(Complex_number(5, 4) / m4, div_scalar3);
}

TEST(Matrixs, BoolMatrix) {
    Matrix<bool> m1(5, 5);
    Matrix<bool> m2(5, 5, 0);
    Matrix<bool> m3(5, 5, 1);

    EXPECT_EQ(m1(3, 2), 0);
    EXPECT_EQ(m2(3, 2), 0);
    EXPECT_EQ(m3(3,2 ), 1);
    m3.set(3, 2, 0);
    EXPECT_EQ(m3(3,2 ), 0);

    EXPECT_THROW(m1(5, 4), MatrixIndexOutOfRangeException);

    EXPECT_EQ(m2 * m3, m2);
    EXPECT_EQ(m2 + m3, m3);

    Matrix<bool> m4(5, 4);
    EXPECT_THROW(m4 + m3, MatrixSizeMissmatch);
    EXPECT_THROW(m4 * m3, MatrixSizeMissmatch);
}

TEST(Vector, VecMatMul) {
    Vector v(3);
    v(0) = 10;
    v(1) = 20;
    v(2) = 30;
    Matrix m(3, 4);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;
    m(0, 3) = 4;

    m(1, 0) = 5;
    m(1, 1) = 6;
    m(1, 2) = 7;
    m(1, 3) = 8;

    m(2, 0) = 9;
    m(2, 1) = 10;
    m(2, 2) = 11;
    m(2, 3) = 12;


    Vector res_mul(4, 0);
    res_mul(0) = 380;
    res_mul(1) = 440;
    res_mul(2) = 500;
    res_mul(3) = 560;
    EXPECT_EQ(v * m, res_mul);
}


// Простые ситуации с прокси. прокси убивается раньше, чем матрица
TEST(Slices, SimpleMatrixSlices) {
    Matrix m(3, 4);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;
    m(0, 3) = 4;

    m(1, 0) = 5;
    m(1, 1) = 6;
    m(1, 2) = 7;
    m(1, 3) = 8;

    m(2, 0) = 9;
    m(2, 1) = 10;
    m(2, 2) = 11;
    m(2, 3) = 12;


    Matrix_coords coord(1, 1, 2, 3);
    auto proxy_p1 = m[coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
    
    EXPECT_EQ((*proxy_p1)(0, 0), m(1, 1));
    (*proxy_p1).set(0, 0, 123);
    EXPECT_NE((*proxy_p1)(0, 0), m(1, 1));
    EXPECT_THROW((*proxy_p1).set(0, 123), InvalidArgument);
    EXPECT_THROW((*proxy_p1)(0, 123), MatrixIndexOutOfRangeException);
    EXPECT_THROW((*proxy_p1)(0), InvalidArgument);

    Matrix_row_coord row_coord(1, 1, 2);
    auto proxy_row = m[row_coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
    std::cout << "m " << m(1, 1) << std::endl;
    EXPECT_EQ((*proxy_row)(0), m(1, 1));
    (*proxy_row).set(0, 123);
    EXPECT_NE((*proxy_row)(0), m(1, 1));
    EXPECT_THROW((*proxy_row).set(0, 0, 123), InvalidArgument);
    EXPECT_THROW((*proxy_row)(123), VectorIndexOutOfRangeException);
    EXPECT_THROW((*proxy_row)(0, 0), InvalidArgument);


    Matrix_col_coord col_coord(1, 1, 2);
    auto proxy_col = m[row_coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
    std::cout << "m " << m(1, 1) << std::endl;
    EXPECT_EQ((*proxy_col)(0), m(1, 1));
    (*proxy_col).set(0, 123);
    EXPECT_NE((*proxy_col)(0), m(1, 1));
    EXPECT_THROW((*proxy_col).set(0, 0, 123), InvalidArgument);
    EXPECT_THROW((*proxy_col)(123), VectorIndexOutOfRangeException);
    EXPECT_THROW((*proxy_col)(0, 0), InvalidArgument);



    delete proxy_p1; // освободим выделенный в m[] указатель
    delete proxy_row; // освободим выделенный в m[] указатель
    delete proxy_col; // освободим выделенный в m[] указатель
}

TEST(Slices, KillMatrix) {
    Matrix m(3, 4);
    m(0, 0) = 1;
    m(0, 1) = 2;
    m(0, 2) = 3;
    m(0, 3) = 4;

    m(1, 0) = 5;
    m(1, 1) = 6;
    m(1, 2) = 7;
    m(1, 3) = 8;

    m(2, 0) = 9;
    m(2, 1) = 10;
    m(2, 2) = 11;
    m(2, 3) = 12;

    Matrix<>* m1 = new Matrix(m); 
    // Matrix<>* m2 = new Matrix(m); 

    Matrix_coords coord(1, 1, 2, 3);
    auto proxy_matrix = (*m1)[coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
    auto proxy_matrix_local = (*m1)[coord]; // для этого прокси создадим локальную копию
    proxy_matrix_local->set(0, 0, 50);

    Matrix_row_coord row_coord(1, 1, 2);
    auto proxy_row = (*m1)[row_coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
    auto proxy_row_local = (*m1)[row_coord]; // для этого прокси создадим локальную копию
    proxy_row_local->set(0, 100);

    Matrix_col_coord col_coord(1, 1, 2);
    auto proxy_col = (*m1)[row_coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
    auto proxy_col_local = (*m1)[row_coord]; // для этого прокси создадим локальную копию
    proxy_col_local->set(0, 150);


    delete m1;
    // delete m2;

    delete proxy_matrix; // освободим выделенный в m[] указатель
    delete proxy_row; // освободим выделенный в m[] указатель
    delete proxy_col; // освободим выделенный в m[] указатель
    delete proxy_matrix_local; // освободим выделенный в m[] указатель
    delete proxy_row_local; // освободим выделенный в m[] указательclea
    delete proxy_col_local; // освободим выделенный в m[] указатель
}

TEST(Slices, VecMulRowSlice) { 
    Vector v(10);
    v(0) = 1;
    v(1) = 2;
    v(2) = 3;
    v(3) = 4;
    v(4) = 5;
    v(5) = 6;
    v(6) = 7;
    v(7) = 8;
    v(8) = 9;
    v(9) = 10;

    Matrix m(10, 10);
    m(3,0) = 1;
    m(3,1) = 2;
    m(3,2) = 3;
    m(3,3) = 4;
    m(3,4) = 5;
    m(3,5) = 6;
    m(3,6) = 7;
    m(3,7) = 8;
    m(3,8) = 9;
    m(3,9) = 10;

    auto proxy_row = m[Matrix_row_coord(3, -1, -1)];
    Vector v_copy = *proxy_row;
    Vector copy_result(10, 0);
    copy_result(0) = 1;
    copy_result(1) = 2;
    copy_result(2) = 3;
    copy_result(3) = 4;
    copy_result(4) = 5;
    copy_result(5) = 6;
    copy_result(6) = 7;
    copy_result(7) = 8;
    copy_result(8) = 9;
    copy_result(9) = 10;

    EXPECT_EQ(v_copy, copy_result);

    delete proxy_row;

    // std::cout << v *


}

// TEST(Slices, KillMatrixAndVecMulForMe) {
//     Matrix m(3, 4);
//     m(0, 0) = 1;
//     m(0, 1) = 2;
//     m(0, 2) = 3;
//     m(0, 3) = 4;

//     m(1, 0) = 5;
//     m(1, 1) = 6;
//     m(1, 2) = 7;
//     m(1, 3) = 8;

//     m(2, 0) = 9;
//     m(2, 1) = 10;
//     m(2, 2) = 11;
//     m(2, 3) = 12;

//     Matrix<>* m1 = new Matrix(m); 
//     Matrix<>* m2 = new Matrix(m); 

//     std::cout << "START SPAWNING PROXY" << std::endl;
//     Matrix_coords coord(1, 1, 2, 3);
//     auto proxy_matrix = m[coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
//     auto proxy_matrix_local = m[coord]; // для этого прокси создадим локальную копию
//     proxy_matrix_local->set(0, 0, 50);

//     Matrix_row_coord row_coord(1, 1, 2);
//     auto proxy_row = m[row_coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
//     auto proxy_row_local = m[row_coord]; // для этого прокси создадим локальную копию
//     proxy_row_local->set(0, 100);

//     Matrix_col_coord col_coord(1, 1, 2);
//     auto proxy_col = m[row_coord]; // Используем auto потому что параметры шаблона матрицы были по умолчанию и не хочется их указывать
//     auto proxy_col_local = m[row_coord]; // для этого прокси создадим локальную копию
//     proxy_col_local->set(0, 150);

//     std::cout << "STOP SPAWNING PROXY" << std::endl;


//     delete m1;
//     delete m2;
//     std::cout << "DELETE PROXY " << proxy_matrix;
//     delete proxy_matrix; // освободим выделенный в m[] указатель
//     std::cout << "DELETE PROXY " << proxy_matrix;
//     delete proxy_row; // освободим выделенный в m[] указатель
//     std::cout << "DELETE PROXY " << proxy_matrix;
//     delete proxy_col; // освободим выделенный в m[] указатель
//     std::cout << "DELETE PROXY " << proxy_matrix;
//     delete proxy_matrix_local; // освободим выделенный в m[] указатель
//     std::cout << "DELETE PROXY " << proxy_matrix;
//     delete proxy_row_local; // освободим выделенный в m[] указательclea
//     std::cout << "DELETE PROXY " << proxy_matrix;
//     delete proxy_col_local; // освободим выделенный в m[] указатель
// }

TEST(Parsing, Vector_parse) {
    Vector<Complex_number<double, double>> vcff("parsing/VCFF.txt");
    Vector<Complex_number<double, double>> rvcff(50000);
    rvcff(0) = Complex_number<double>(100);
    rvcff(5999) = Complex_number<double>(23, 5.1);
    rvcff(6) = Complex_number<double>(5.5, 3.5);
    rvcff(21) = Complex_number<double>(44, 1.1);
    EXPECT_EQ(vcff, rvcff);

    Vector<Complex_number<int, int>> vcii("parsing/VCII.txt");
    Vector<Complex_number<int, int>> rvcii(50000);
    rvcii(0) = Complex_number<int>(100);
    rvcii(5999) = Complex_number<int>(23, 5);
    rvcii(6) = Complex_number<int>(5, 3);
    rvcii(21) = Complex_number<int>(44, 1);
    EXPECT_EQ(vcii, rvcii);

    Vector<Complex_number<int, double>> vcif("parsing/VCIF.txt");
    Vector<Complex_number<int, double>> rvcif(50000);
    rvcif(0) = Complex_number<int, double>(100);
    rvcif(5999) = Complex_number<int, double>(23, 5.1);
    rvcif(6) = Complex_number<int, double>(5, 3.5);
    rvcif(21) = Complex_number<int, double>(44, 1.1);
    EXPECT_EQ(vcif, rvcif);

    Vector<Complex_number<double, int>> vcfi("parsing/VCFI.txt");
    Vector<Complex_number<double, int>> rvcfi(50000);
    rvcfi(0) = Complex_number<double, int>(100);
    rvcfi(5999) = Complex_number<double, int>(23, 5);
    rvcfi(6) = Complex_number<double, int>(5.5, 3);
    rvcfi(21) = Complex_number<double, int>(44, 1);
    EXPECT_EQ(vcfi, rvcfi);


    Vector<Rational_number<int>> vr("parsing/VR.txt");
    Vector<Rational_number<int>> rvr(50000);
    rvr(0) = Rational_number<int>(100);
    rvr(5999) = Rational_number<int>(23, 5);
    rvr(6) = Rational_number<int>(5, 3);
    rvr(21) = Rational_number<int>(44, 1);
    EXPECT_EQ(vr, rvr);

    Vector<bool> vb("parsing/VB.txt");
    Vector<bool> rvb(50000);
    rvb.set(0, 1);
    rvb.set(5999, 1);
    rvb.set(6, 1);
    rvb.set(21, 1);
    EXPECT_EQ(vb, rvb);
}

// TEST(Parsing, Matrix_parse) {
//     Matrix<Complex_number<double, double>> mcff("parsing/MCFF.txt");
//     Matrix<Complex_number<double, double>> rmcff(50000, 500);
//     rmcff(0, 0) = Complex_number<double>(100);
//     rmcff(5999, 1) = Complex_number<double>(23, 5.1);
//     rmcff(6, 0) = Complex_number<double>(5.5, 3.5);
//     rmcff(21, 1) = Complex_number<double>(44, 1.1);
//     EXPECT_EQ(mcff, rmcff);
//     Matrix<Complex_number<int, int>> mcii("parsing/MCII.txt");
//     Matrix<Complex_number<int, int>> rmcii(50000, 500);
//     rmcii(0, 0) = Complex_number<int>(100);
//     rmcii(5999, 1) = Complex_number<int>(23, 5);
//     rmcii(6, 0) = Complex_number<int>(5, 3);
//     rmcii(21, 1) = Complex_number<int>(44, 1);
//     EXPECT_EQ(mcii, rmcii);
//     Matrix<Complex_number<int, double>> mcif("parsing/MCIF.txt");
//     Matrix<Complex_number<int, double>> rmcif(50000, 500);
//     rmcif(0, 0) = Complex_number<int, double>(100);
//     rmcif(5999, 1) = Complex_number<int, double>(23, 5.1);
//     rmcif(6, 0) = Complex_number<int, double>(5, 3.5);
//     rmcif(21, 1) = Complex_number<int, double>(44, 1.1);
//     EXPECT_EQ(mcif, rmcif);
//     Matrix<Complex_number<double, int>> mcfi("parsing/MCFI.txt");
//     Matrix<Complex_number<double, int>> rmcfi(50000, 500);
//     rmcfi(0, 0) = Complex_number<double, int>(100);
//     rmcfi(5999, 1) = Complex_number<double, int>(23, 5);
//     rmcfi(6, 0) = Complex_number<double, int>(5.5, 3);
//     rmcfi(21, 1) = Complex_number<double, int>(44, 1);
//     EXPECT_EQ(mcfi, rmcfi);
//     Matrix<Rational_number<int>> mr("parsing/MR.txt");
//     Matrix<Rational_number<int>> rmr(50000, 5000);
//     rmr(0, 0) = Rational_number<int>(100);
//     rmr(5999, 1) = Rational_number<int>(23, 5);
//     rmr(6, 0) = Rational_number<int>(5, 3);
//     rmr(21, 1) = Rational_number<int>(44, 1);
//     EXPECT_EQ(mr, rmr);
//     Matrix<bool> mb("parsing/MB.txt");
//     Matrix<bool> rmb(50000, 500);
//     rmb.set(0, 0, 1);
//     rmb.set(5999, 1, 1);
//     rmb.set(6, 0, 1);
//     rmb.set(21, 1, 1);
//     EXPECT_EQ(mb, rmb);
// }

int run_all_my_tests(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}