#pragma once
#include <gtest/gtest.h>
#include "Exceptions.hpp"
#include "Rational_number.hpp"


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

int run_all_my_tests(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}