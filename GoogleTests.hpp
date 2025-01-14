#pragma once
#include <gtest/gtest.h>
#include <cmath>
#include "Exceptions.hpp"
#include "Rational_number.hpp"
#include "Utils.hpp"
#include "Complex_number.hpp"
#include "Vector.hpp"

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

TEST(Vector, Constructors) {
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

TEST(Vector, Operators) {
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

TEST(Vector, Complex) {
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

TEST(Vector, BoolVector) {
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

    std::cout << v4;
}

int run_all_my_tests(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}