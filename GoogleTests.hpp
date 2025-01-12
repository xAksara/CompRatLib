#pragma once
#include <gtest/gtest.h>
#include "Exceptions.hpp"
#include "Rational_number.hpp"
#include "Utils.hpp"
#include "Complex_number.hpp"


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

int run_all_my_tests(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}