#include <gtest/gtest.h>
#include "Rational.hpp"
#include "Complex.hpp"
#include "Exceptions.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Matrix_coords.hpp"
#include "Matrix_proxy.hpp"



TEST(RationalTest, ConstructorWithNumeratorAndDenominator) {
    Rational<int> r1(6, 3);
    EXPECT_EQ(r1._numerator, 6);
    EXPECT_EQ(r1._denominator, 3);

    Rational<int> r2(0, 7);
    EXPECT_EQ(r2._numerator, 0);
    EXPECT_EQ(r2._denominator, 1);

    EXPECT_THROW(Rational<int>(5, 0), DivisionByZeroException<int>);
}


TEST(RationalTest, ConstructorWithNumeratorOnly) {
    Rational<int> r1(5);
    EXPECT_EQ(r1._numerator, 5);
    EXPECT_EQ(r1._denominator, 1);

    Rational<int> r2(-8);
    EXPECT_EQ(r2._numerator, -8);
    EXPECT_EQ(r2._denominator, 1);

    Rational<int> r3(0);
    EXPECT_EQ(r3._numerator, 0);
    EXPECT_EQ(r3._denominator, 1);
}


TEST(RationalTest, ConstructorWithString) {
    Rational<int> r1("42");
    EXPECT_EQ(r1._numerator, 42);
    EXPECT_EQ(r1._denominator, 1);

    Rational<int> r2("-17");
    EXPECT_EQ(r2._numerator, -17);
    EXPECT_EQ(r2._denominator, 1);

    Rational<int> r3("5/3");
    EXPECT_EQ(r3._numerator, 5);
    EXPECT_EQ(r3._denominator, 3);

    Rational<int> r4("-8/4");
    EXPECT_EQ(r4._numerator, -8);
    EXPECT_EQ(r4._denominator, 4);

    EXPECT_THROW(Rational<int>("4/0"), DivisionByZeroException<int>);
}


TEST(RationalTest, Exceptions) {
    EXPECT_THROW(Rational<int>(10, 0), DivisionByZeroException<int>);
    EXPECT_THROW(Rational<int>("5/0"), DivisionByZeroException<int>);
    EXPECT_THROW(Rational<int>("abc"), InvalidArgument);
    EXPECT_THROW(Rational<int>("10/xyz"), InvalidArgument);
    EXPECT_THROW(Rational<int>(""), InvalidArgument);
}


TEST(RationalTest, StringParsing) {
    Rational<int> r1("100/20");
    EXPECT_EQ(r1._numerator, 100);
    EXPECT_EQ(r1._denominator, 20);

    Rational<int> r2("-12/6");
    EXPECT_EQ(r2._numerator, -12);
    EXPECT_EQ(r2._denominator, 6);

    Rational<int> r3("0/7");
    EXPECT_EQ(r3._numerator, 0);
    EXPECT_EQ(r3._denominator, 1);
}


TEST(RationalTest, TemplateSupport) {
    Rational<long> r1(9223372036854775807, 2); 
    EXPECT_EQ(r1._numerator, 9223372036854775807);
    EXPECT_EQ(r1._denominator, 2);

    Rational<short> r2(32767, 5); 
    EXPECT_EQ(r2._numerator, 32767);
    EXPECT_EQ(r2._denominator, 5);
}


TEST(RationalTest, Gcd) {
    EXPECT_EQ(Rational<int>::gcd(12, 8), 4);     
    EXPECT_EQ(Rational<int>::gcd(100, 25), 25);  
    EXPECT_EQ(Rational<int>::gcd(-12, 8), 4);    
    EXPECT_EQ(Rational<int>::gcd(0, 10), 10);    
    EXPECT_EQ(Rational<int>::gcd(7, 1), 1);     
    EXPECT_THROW(Rational<int>::gcd(0, 0), InvalidArgument);
}


TEST(RationalTest, MakeCanonical) {
    Rational<int> r1(4, 6);
    auto canonical1 = r1.make_canonical();
    EXPECT_EQ(canonical1._numerator, 2);
    EXPECT_EQ(canonical1._denominator, 3);

    Rational<int> r2(0, 5);
    auto canonical2 = r2.make_canonical();
    EXPECT_EQ(canonical2._numerator, 0);
    EXPECT_EQ(canonical2._denominator, 1);

    Rational<int> r3(-10, 20);
    auto canonical3 = r3.make_canonical();
    EXPECT_EQ(canonical3._numerator, -1);
    EXPECT_EQ(canonical3._denominator, 2);
}


TEST(RationalTest, ConversionOperators) {
    Rational<int> r1(7, 3);
    Rational<int> r2(-7, 3);

    EXPECT_EQ(static_cast<int>(r1), 2);  
    EXPECT_EQ(static_cast<int>(r2), -2);

    EXPECT_NEAR(static_cast<double>(r1), 2.333333, 1e-6);
    EXPECT_NEAR(static_cast<double>(r2), -2.333333, 1e-6);
}


TEST(RationalTest, Floor) {
    Rational<int> r1(7, 3);   
    Rational<int> r2(-7, 3); 

    EXPECT_EQ(r1.floor(), 2);  
    EXPECT_EQ(r2.floor(), -3); 
}


TEST(RationalTest, Round) {
    Rational<int> r1(7, 3);    
    Rational<int> r2(-7, 3);   
    Rational<int> r3(4, 3);    
    Rational<int> r4(-4, 3);   

    EXPECT_EQ(r1.round(), 2);   
    EXPECT_EQ(r2.round(), -2);  
    EXPECT_EQ(r3.round(), 1);   
    EXPECT_EQ(r4.round(), -1); 
}

TEST(RationalTest, AddTwoRationalsWithDifferentNumeratorTypes) {
    
    Rational<int> r1(3, 4);
    Rational<long long> r2(5, 7);

    
    Rational<long long> expected_result(41, 28);

    auto result = r1 + r2;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, AddRationalAndInteger) {
    Rational<int> r1(3, 4);
    int value = 2;
    
    Rational<int> expected_result(11, 4);
    
    auto result = r1 + value;
    EXPECT_EQ(result, expected_result);

    result = value + r1;
    EXPECT_EQ(result, expected_result); 
}

TEST(RationalTest, AddTwoRationalsWithSameNumeratorTypes) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);

    
    Rational<int> expected_result(41, 28);

    auto result = r1 + r2;
    EXPECT_EQ(result, expected_result);
}


TEST(RationalTest, SubtractTwoRationalsWithDifferentNumeratorTypes) {
    
    Rational<int> r1(3, 4);
    Rational<long long> r2(5, 7);

    
    Rational<long long> expected_result(1, 28);

    auto result = r1 - r2;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, SubtractRationalAndInteger) {
    Rational<int> r1(3, 4);
    int value = 2;

    
    Rational<int> expected_result(-5, 4);
    
    auto result = r1 - value;
    EXPECT_EQ(result, expected_result);

    result = value - r1;
    
    expected_result = Rational<int>(5, 4);
    EXPECT_EQ(result, expected_result); 
}

TEST(RationalTest, SubtractTwoRationalsWithSameNumeratorTypes) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);

    
    Rational<int> expected_result(1, 28);

    auto result = r1 - r2;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, MultiplyTwoRationalsWithDifferentNumeratorTypes) {
    
    Rational<int> r1(3, 4);
    Rational<long long> r2(5, 7);

    
    Rational<long long> expected_result(15, 28);

    auto result = r1 * r2;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, MultiplyRationalAndInteger) {
    Rational<int> r1(3, 4);
    int value = 2;

    
    Rational<int> expected_result(3, 2);

    auto result = r1 * value;
    EXPECT_EQ(result, expected_result);

    result = value * r1;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, MultiplyTwoRationalsWithSameNumeratorTypes) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);

    
    Rational<int> expected_result(15, 28);

    auto result = r1 * r2;
    EXPECT_EQ(result, expected_result);
}


TEST(RationalTest, DivideTwoRationalsWithDifferentNumeratorTypes) {
    Rational<int> r1(3, 4);
    Rational<long long> r2(5, 7);

    
    Rational<long long> expected_result(21, 20);

    auto result = r1 / r2;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, DivideRationalByInteger) {
    Rational<int> r1(3, 4);
    int value = 2;

    
    Rational<int> expected_result(3, 8);

    auto result = r1 / value;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, DivideIntegerByRational) {
    Rational<int> r1(3, 4);
    int value = 2;

    
    Rational<int> expected_result(8, 3);

    auto result = value / r1;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, DivideTwoRationalsWithSameNumeratorTypes) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);

    
    Rational<int> expected_result(21, 20);

    auto result = r1 / r2;
    EXPECT_EQ(result, expected_result);
}

TEST(RationalTest, LessThan) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);
    EXPECT_FALSE(r1 < r2);
    EXPECT_TRUE(r2 < r1);

    Rational<int> r3(6, 8); 
    EXPECT_FALSE(r1 < r3);

    Rational<int> r4(-3, 4);
    EXPECT_TRUE(r4 < r1);
}

TEST(RationalTest, LessThanOrEqual) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);
    EXPECT_FALSE(r1 <= r2);
    EXPECT_TRUE(r2 <= r1);

    Rational<int> r3(6, 8); 
    EXPECT_TRUE(r1 <= r3);
    EXPECT_TRUE(r3 <= r1);

    Rational<int> r4(-3, 4);
    EXPECT_TRUE(r4 <= r1);
}

TEST(RationalTest, GreaterThan) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);
    EXPECT_FALSE(r2 > r1);
    EXPECT_TRUE(r1 > r2);

    Rational<int> r3(6, 8); 
    EXPECT_FALSE(r1 > r3);

    Rational<int> r4(-3, 4);
    EXPECT_TRUE(r1 > r4);
}

TEST(RationalTest, GreaterThanOrEqual) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);
    EXPECT_FALSE(r2 >= r1);
    EXPECT_TRUE(r1 >= r2);

    Rational<int> r3(6, 8); 
    EXPECT_TRUE(r1 >= r3);
    EXPECT_TRUE(r3 >= r1);

    Rational<int> r4(-3, 4);
    EXPECT_TRUE(r1 >= r4);
}

TEST(RationalTest, Equal) {
    Rational<int> r1(3, 4);
    Rational<int> r2(6, 8); 
    EXPECT_TRUE(r1 == r2);

    Rational<int> r3(5, 7);
    EXPECT_FALSE(r1 == r3);

    Rational<int> r4(-3, 4);
    Rational<int> r5(-6, 8); 
    EXPECT_TRUE(r4 == r5);
}

TEST(RationalTest, NotEqual) {
    Rational<int> r1(3, 4);
    Rational<int> r2(5, 7);
    EXPECT_TRUE(r1 != r2);

    Rational<int> r3(6, 8); 
    EXPECT_FALSE(r1 != r3);

    Rational<int> r4(-3, 4);
    Rational<int> r5(-6, 8); 
    EXPECT_FALSE(r4 != r5);
}


TEST(ComplexTest, EqualityOperator) {
    Complex<int> c1(3, 4);
    Complex<int> c2(3, 4);
    Complex<int> c3(5, -2);

    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
}

TEST(ComplexTest, InequalityOperator) {
    Complex<int> c1(3, 4);
    Complex<int> c2(5, 6);

    EXPECT_TRUE(c1 != c2);
    EXPECT_FALSE(c1 != c1);
}

TEST(ComplexTest, ComparisonOperatorsByModulus) {
    Complex<double> c1(3.0, 4.0); 
    Complex<double> c2(1.0, 1.0); 

    EXPECT_TRUE(c1 > c2);
    EXPECT_FALSE(c1 < c2);
    EXPECT_TRUE(c1 >= c2);
    EXPECT_FALSE(c1 <= c2);

    Complex<double> c3(3.0, 4.0);
    EXPECT_TRUE(c1 >= c3);
    EXPECT_TRUE(c1 <= c3);
}


TEST(ComplexTest, AdditionOperator) {
    Complex<int> c1(3, 4);
    Complex<int> c2(5, -2);

    Complex<int> result = c1 + c2;
    EXPECT_EQ(result, Complex<int>(8, 2));
}

TEST(ComplexTest, SubtractionOperator) {
    Complex<int> c1(3, 4);
    Complex<int> c2(5, -2);

    Complex<int> result = c1 - c2;
    EXPECT_EQ(result, Complex<int>(-2, 6));
}

TEST(ComplexTest, MultiplicationOperator) {
    Complex<int> c1(1, 2);
    Complex<int> c2(3, 4);

    Complex<int> result = c1 * c2;
    
    EXPECT_EQ(result, Complex<int>(-5, 10));
}

TEST(ComplexTest, DivisionOperator) {
    Complex<double> c1(1.0, 2.0);
    Complex<double> c2(3.0, 4.0);

    Complex<double> result = c1 / c2;
    
    double denom = 3.0 * 3.0 + 4.0 * 4.0; 
    EXPECT_DOUBLE_EQ(result.real, (1.0 * 3.0 + 2.0 * 4.0) / denom);
    EXPECT_DOUBLE_EQ(result.image, (2.0 * 3.0 - 1.0 * 4.0) / denom);
}


TEST(ComplexTest, MixedOperations) {
    Complex<int> c1(2, 3);
    Complex<int> c2(1, -1);

    Complex<int> sum = c1 + c2;
    Complex<int> difference = c1 - c2;
    Complex<int> product = c1 * c2;
    Complex<int> division = c1 / Complex<int>(1, 1);

    EXPECT_EQ(sum, Complex<int>(3, 2));
    EXPECT_EQ(difference, Complex<int>(1, 4));
    EXPECT_EQ(product, Complex<int>(5, 1));
    EXPECT_EQ(division, Complex<int>(2.5, 0.5));
}


TEST(ComplexTest, ComparisonWithDifferentTypes) {
    Complex<int> c1(3, 4);
    Complex<double> c2(3.0, 4.0);

    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 != c2);
    EXPECT_FALSE(c1 < c2);
    EXPECT_FALSE(c1 > c2);
    EXPECT_TRUE(c1 <= c2);
    EXPECT_TRUE(c1 >= c2);
}


TEST(ComplexTest, Modulus) {
    Complex<double> c1(3.0, 4.0);

    EXPECT_DOUBLE_EQ(c1.modulus(), 5.0); 
}

TEST(ComplexTest, AdditionWithNumber) {
    Complex<int> c1(3, 4);

    
    Complex<int> result = c1 + 2;
    EXPECT_EQ(result, Complex<int>(5, 4));

    
    Complex<double> c2(3.5, 4.5);
    Complex<double> result2 = c2 + 1.5;
    EXPECT_EQ(result2, Complex<double>(5.0, 4.5));
}

TEST(ComplexTest, SubtractionWithNumber) {
    Complex<int> c1(7, 5);

    
    Complex<int> result = c1 - 2;
    EXPECT_EQ(result, Complex<int>(5, 5));

    
    Complex<double> c2(7.5, 5.5);
    Complex<double> result2 = c2 - 2.5;
    EXPECT_EQ(result2, Complex<double>(5.0, 5.5));
}

TEST(ComplexTest, MultiplicationWithNumber) {
    Complex<int> c1(2, 3);

    
    Complex<int> result = c1 * 3;
    EXPECT_EQ(result, Complex<int>(6, 9));

    
    Complex<double> c2(2.5, 3.5);
    Complex<double> result2 = c2 * 2.0;
    EXPECT_EQ(result2, Complex<double>(5.0, 7.0));
}

TEST(ComplexTest, DivisionWithNumber) {
    Complex<int> c1(8, 4);

    Complex<int> result = c1 / 2;
    EXPECT_EQ(result, Complex<int>(4, 2));

    Complex<double> c2(8.0, 4.0);
    Complex<double> result2 = c2 / 2.0;
    EXPECT_EQ(result2, Complex<double>(4.0, 2.0));
}

// TEST(ComplexRationalTests, Addition) {
//     Complex<int> c1(3, 4);
//     Rational<int> r1(5, 2);
//     auto result = c1 + r1;
//     EXPECT_EQ(result.real, 5.5);  
//     EXPECT_EQ(result.image, 4);
// }

// TEST(ComplexRationalTests, Subtraction) {
//     Complex<int> c1(3, 4);
//     Rational<int> r1(5, 2);
//     auto result = c1 - r1;
//     EXPECT_EQ(result.real, 0.5);  
//     EXPECT_EQ(result.image, 4);
// }

// TEST(ComplexRationalTests, Multiplication) {
//     Complex<int> c1(3, 4);
//     Rational<int> r1(5, 2);
//     auto result = c1 * r1;
//     EXPECT_EQ(result.real, 7.5);  
//     EXPECT_EQ(result.image, 10); 
// }

// TEST(ComplexRationalTests, Division) {
//     Complex<int> c1(6, 8);
//     Rational<int> r1(5, 2);
//     auto result = c1 / r1;
//     EXPECT_EQ(result.real, 2.4);  
//     EXPECT_EQ(result.image, 3.2); 
// }

TEST(ComplexRationalTests, DivisionByZero) {
    Complex<int> c1(6, 8);
    Rational<int> r1(0, 1);
    EXPECT_THROW(c1 / r1, DivisionByZeroException<decltype(c1)>);
}


TEST(MatrixTest, ConstructorAndBasicProperties) {
    Matrix<> m(3, 4, 0.01);

    EXPECT_EQ(m.rows, 3);
    EXPECT_EQ(m.columns, 4);
    EXPECT_DOUBLE_EQ(m.eps, 0.01);
    EXPECT_EQ(m.to_string(), "Matrix(rows=3, columns=4, eps=0.01)\n");
}


TEST(MatrixTest, InsertAndAccess) {
    Matrix<> m(3, 3, 0.01);
    m.set(0, 0, 1.0);
    m.set(1, 1, 2.5);

    EXPECT_DOUBLE_EQ(m(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m(1, 1), 2.5);

    
    m.set(0, 1, 0.001);
    EXPECT_EQ(m.to_string(), "Matrix(rows=3, columns=3, eps=0.01)\n[0, 0] -> 1\n[1, 1] -> 2.5\n");
}


TEST(MatrixTest, AdditionAndSubtraction) {
    Matrix<> m1(2, 2), m2(2, 2);
    m1.set(0, 0, 1.0);
    m1.set(1, 1, 2.0);

    m2.set(0, 0, 3.0);
    m2.set(0, 1, 4.0);

    Matrix<> sum = m1 + m2;
    EXPECT_DOUBLE_EQ(sum(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(sum(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(sum(1, 1), 2.0);

    Matrix<> diff = m1 - m2;
    EXPECT_DOUBLE_EQ(diff(0, 0), -2.0);
    EXPECT_DOUBLE_EQ(diff(0, 1), -4.0);
    EXPECT_DOUBLE_EQ(diff(1, 1), 2.0);
}


TEST(MatrixTest, Transpose) {
    Matrix<> m(2, 3);
    m.set(0, 1, 1.0);
    m.set(1, 2, 2.0);

    Matrix<> transposed = ~m;
    EXPECT_DOUBLE_EQ(transposed(1, 0), 1.0);
    EXPECT_DOUBLE_EQ(transposed(2, 1), 2.0);
    EXPECT_EQ(transposed.rows, 3);
    EXPECT_EQ(transposed.columns, 2);
}


TEST(MatrixTest, Exceptions) {
    Matrix<> m(2, 2);

    
    EXPECT_THROW(m(3, 0), OutOfRangeException);

    
    Matrix<> m2(3, 3);
    EXPECT_THROW(m + m2, InvalidArgument);

    
    EXPECT_THROW(m / 0.0, DivisionByZeroException<decltype(m)>);
}


TEST(MatrixTest, ScalarMultiplication) {
    Matrix<> m(2, 2);
    m.set(0, 0, 1.0);
    m.set(1, 1, -2.0);

    Matrix<> scaled = m * 2.0;
    EXPECT_DOUBLE_EQ(scaled(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(scaled(1, 1), -4.0);
}


TEST(MatrixTest, MatrixMultiplication) {
    Matrix<> m1(2, 3), m2(3, 2);
    m1.set(0, 0, 1.0);
    m1.set(0, 1, 2.0);
    m1.set(0, 2, 3.0);

    m2.set(0, 0, 4.0);
    m2.set(1, 1, 5.0);
    m2.set(2, 0, 6.0);

    Matrix<> product = m1 * m2;
    EXPECT_DOUBLE_EQ(product(0, 0), 22.0);
    EXPECT_DOUBLE_EQ(product(0, 1), 10.0);
    EXPECT_EQ(product.rows, 2);
    EXPECT_EQ(product.columns, 2);
}

TEST(MatrixTest, Slices) {
    Matrix test_matrix(3, 3, 0.01);
    test_matrix.set(1, 1, 1);
    test_matrix.set(1, 2, 2);
    test_matrix.set(2, 1, 3);
    test_matrix.set(2, 2, 4);

    Matrix_coords coords(1, 1, 3, 3);
    auto proxy = test_matrix[coords];
    ASSERT_EQ(&test_matrix(1, 1), &proxy(0, 0));
    proxy.set(0, 0, 2);
    ASSERT_NE(&test_matrix(1, 1), &proxy(0, 0));
    ASSERT_EQ(test_matrix(1,1), 1);
}

TEST(MatrixTest, RowSlices) {
    Matrix test_matrix(3, 3, 0.01);
    test_matrix.set(1, 1, 1);
    test_matrix.set(1, 2, 2);
    test_matrix.set(2, 1, 3);
    test_matrix.set(2, 2, 4);

    Matrix_row_coord coords(1, 1, 3);
    auto proxy = test_matrix[coords];

    std::cout << proxy(0) << std::endl;  
    std::cout << proxy(1) << std::endl;  

    ASSERT_EQ(&test_matrix(1, 1), &proxy(0));
    proxy.set(1, 42.0); 
    ASSERT_NE(&test_matrix(1, 1), &proxy(0));
    ASSERT_EQ(proxy(1), 42.0);
}

TEST(MatrixTest, ColSlices) {
    Matrix test_matrix(3, 3, 0.01);
    test_matrix.set(1, 1, 1);
    test_matrix.set(1, 2, 2);
    test_matrix.set(2, 1, 3);
    test_matrix.set(2, 2, 4);

    Matrix_col_coord coords(1, 1, 3);
    auto proxy = test_matrix[coords];

    ASSERT_EQ(&test_matrix(1, 1), &proxy(0));
    proxy.set(1, 42.0); 
    ASSERT_NE(&test_matrix(1, 1), &proxy(0));
    ASSERT_EQ(proxy(1), 42.0);
}

TEST(VectorTest, ConstructorTest) {
    Vector<> vec(5, 0.1);
    EXPECT_EQ(vec.to_string(), "Vector(dim=5, epsilon=0.1)\n");
}

TEST(VectorTest, SetAndGetTest) {
    Vector<> vec(5);
    vec.set(0, 1.5);
    vec.set(4, -3.0);

    EXPECT_EQ(vec.get(0), 1.5);
    EXPECT_EQ(vec.get(4), -3.0);
    EXPECT_EQ(vec.get(1), 0.0); 

    vec.set(4, 0.0); 
    EXPECT_EQ(vec.get(4), 0.0);
}

TEST(VectorTest, OutOfRangeTest) {
    Vector<> vec(3);
    EXPECT_THROW(vec.set(3, 2.0), OutOfRangeException);
    EXPECT_THROW(vec.get(3), OutOfRangeException);
}

TEST(VectorTest, CopyConstructorTest) {
    Vector<> vec1(5);
    vec1.set(0, 2.0);

    Vector<> vec2 = vec1;
    EXPECT_EQ(vec2.get(0), 2.0);
    EXPECT_EQ(vec2.get(1), 0.0);
}

TEST(VectorTest, MoveConstructorTest) {
    Vector<> vec1(5);
    vec1.set(0, 2.0);

    Vector<> vec2 = std::move(vec1);
    EXPECT_EQ(vec2.get(0), 2.0);
    EXPECT_EQ(vec2.get(1), 0.0);
}

TEST(VectorTest, AdditionTest) {
    Vector<> vec1(3);
    vec1.set(0, 1.0);
    vec1.set(1, 2.0);

    Vector<> vec2(3);
    vec2.set(1, 3.0);
    vec2.set(2, 4.0);

    Vector<> result = vec1 + vec2;
    EXPECT_EQ(result.get(0), 1.0);
    EXPECT_EQ(result.get(1), 5.0);
    EXPECT_EQ(result.get(2), 4.0);
}

TEST(VectorTest, SubtractionTest) {
    Vector<> vec1(3);
    vec1.set(0, 5.0);
    vec1.set(1, 7.0);

    Vector<> vec2(3);
    vec2.set(1, 3.0);
    vec2.set(2, 2.0);

    Vector<> result = vec1 - vec2;
    EXPECT_EQ(result.get(0), 5.0);
    EXPECT_EQ(result.get(1), 4.0);
    EXPECT_EQ(result.get(2), -2.0);
}

TEST(VectorTest, NegationTest) {
    Vector<> vec(3);
    vec.set(0, 1.0);
    vec.set(1, -2.0);

    Vector<> result = -vec;
    EXPECT_EQ(result.get(0), -1.0);
    EXPECT_EQ(result.get(1), 2.0);
}

TEST(VectorTest, ScalarMultiplicationTest) {
    Vector<> vec(3);
    vec.set(0, 1.0);
    vec.set(1, -2.0);

    Vector<> result = vec * 2.0;
    EXPECT_EQ(result.get(0), 2.0);
    EXPECT_EQ(result.get(1), -4.0);
}

TEST(VectorTest, ScalarDivisionTest) {
    Vector<> vec(3);
    vec.set(0, 4.0);
    vec.set(1, -8.0);

    Vector<> result = vec / 2.0;
    EXPECT_EQ(result.get(0), 2.0);
    EXPECT_EQ(result.get(1), -4.0);

    EXPECT_THROW(vec / 0.0, DivisionByZeroException<decltype(result)>);
}

TEST(VectorMatrixMultiplicationTest, SimpleMultiplication) {
    Matrix<> mat1(3, 3);
    mat1.set(0, 0, 1.0);
    mat1.set(1, 1, 2.0);
    mat1.set(2, 2, 3.0);

    Vector<> vec1(3);
    vec1.set(0, 1.0);
    vec1.set(1, 2.0);
    vec1.set(2, 3.0);

    auto result = vec1 * mat1;
    EXPECT_EQ(result.get(0), 1.0);
    EXPECT_EQ(result.get(1), 4.0);
    EXPECT_EQ(result.get(2), 9.0);
}


TEST(VectorMatrixMultiplicationTest, SparseMatrix) {
    Matrix<> mat2(3, 4);
    mat2.set(0, 2, 5.0);
    mat2.set(1, 3, -1.0);
    mat2.set(2, 0, 7.0);

    Vector<> vec2(3);
    vec2.set(0, 2.0);
    vec2.set(1, 3.0);
    vec2.set(2, 4.0);

    auto result = vec2 * mat2;
    EXPECT_EQ(result.get(0), 28);
    EXPECT_EQ(result.get(1), 0);
    EXPECT_EQ(result.get(2), 10.0);
    EXPECT_EQ(result.get(3), -3.0);
}


TEST(VectorMatrixMultiplicationTest, DimensionMismatch) {
    Matrix<> mat3(2, 3);
    Vector<> vec3(3);
    EXPECT_THROW(vec3 * mat3, InvalidArgument);
}


TEST(VectorMatrixMultiplicationTest, EmptyMatrix) {
    Matrix<> mat4(3, 3);
    Vector<> vec4(3);
    vec4.set(0, 5.0);

    auto result = vec4 * mat4; 
    for (unsigned int i = 0; i < 3; ++i) {
        EXPECT_EQ(result.get(i), 0.0);
    }
} 


TEST(VectorMatrixMultiplicationTest, EpsilonFiltering) {
    Matrix<> mat5(3, 3, 0.1);
    mat5.set(0, 0, 0.05); 
    mat5.set(1, 1, 1.0);

    Vector<> vec5(3);
    vec5.set(1, 2.0);

    auto result = vec5 * mat5; 
    EXPECT_EQ(result.get(0), 0.0);
    EXPECT_EQ(result.get(1), 2.0);
    EXPECT_EQ(result.get(2), 0.0);
}

TEST(BitVectorTest, Constructor) {
    Vector<bool> v(128);
    for (size_t i = 0; i < 128; ++i) {
        EXPECT_FALSE(v.get(i)) << "All bits should initially be false";
    }
}

TEST(BitVectorTest, SetAndGet) {
    Vector<bool> v(128);

    v.set(5, true);
    EXPECT_TRUE(v.get(5)) << "Bit 5 should be true after being set";

    v.set(5, false);
    EXPECT_FALSE(v.get(5)) << "Bit 5 should be false after being reset";

    EXPECT_THROW(v.set(128, true), OutOfRangeException);
    EXPECT_THROW(v.get(128), OutOfRangeException);
}

TEST(BitVectorTest, LogicalAnd) {
    Vector<bool> v1(64);
    Vector<bool> v2(64);

    v1.set(5, true);
    v2.set(5, true);
    v2.set(10, true);

    Vector<bool> v3 = v1 * v2;
    EXPECT_TRUE(v3.get(5)) << "Logical AND should keep common true bits";
    EXPECT_FALSE(v3.get(10)) << "Bit 10 should be false after AND operation";
}

TEST(BitVectorTest, LogicalOr) {
    Vector<bool> v1(64);
    Vector<bool> v2(64);

    v1.set(5, true);
    v2.set(10, true);

    Vector<bool> v3 = v1 + v2;
    EXPECT_TRUE(v3.get(5)) << "Bit 5 should be true after OR operation";
    EXPECT_TRUE(v3.get(10)) << "Bit 10 should be true after OR operation";
    EXPECT_FALSE(v3.get(20)) << "Bit 20 should remain false after OR operation";
}

TEST(BitVectorTest, ToString) {
    Vector<bool> v(10);
    v.set(0, true);
    v.set(3, true);
    EXPECT_EQ(v.to_string(), "Vector(dim=10)\nIndex 0: 1\nIndex 3: 1\n") << "ToString should correctly represent the bit vector";
}

TEST(BitMatrix, Constructor) {
    Matrix<bool> m(4, 8);

    for (size_t row = 0; row < 4; ++row) {
        for (size_t col = 0; col < 8; ++col) {
            EXPECT_FALSE(m.get(row, col)) << "All bits should initially be false";
        }
    }
}

TEST(BitMatrix, SetAndGet) {
    Matrix<bool> m(4, 8);

    m.set(1, 3, true);
    EXPECT_TRUE(m.get(1, 3)) << "Bit at (1, 3) should be true after being set";

    m.set(1, 3, false);
    EXPECT_FALSE(m.get(1, 3)) << "Bit at (1, 3) should be false after being reset";

    EXPECT_THROW(m.set(4, 0, true), OutOfRangeException);
    EXPECT_THROW(m.get(0, 8), OutOfRangeException);
}

TEST(BitMatrix, LogicalAnd) {
    Matrix<bool> m1(3, 4);
    Matrix<bool> m2(3, 4);

    m1.set(1, 2, true);
    m2.set(1, 2, true);
    m2.set(2, 3, true);

    Matrix<bool> m3 = m1 * m2;

    EXPECT_TRUE(m3.get(1, 2)) << "Logical AND should keep common true bits";
    EXPECT_FALSE(m3.get(2, 3)) << "Bit at (2, 3) should be false after AND operation";
}

TEST(BitMatrix, LogicalOr) {
    Matrix<bool> m1(3, 4);
    Matrix<bool> m2(3, 4);

    m1.set(0, 0, true);
    m2.set(2, 3, true);

    Matrix<bool> m3 = m1 + m2;

    EXPECT_TRUE(m3.get(0, 0)) << "Bit at (0, 0) should be true after OR operation";
    EXPECT_TRUE(m3.get(2, 3)) << "Bit at (2, 3) should be true after OR operation";
    EXPECT_FALSE(m3.get(1, 1)) << "Bit at (1, 1) should remain false after OR operation";
}

TEST(BitMatrixTest, ToString) {
    Matrix<bool> v(10, 5);
    v.set(0, 1, true);
    v.set(3, 2, true);
    EXPECT_EQ(v.to_string(), "Matrix(rows = 10, columns = 5)\n[0, 1] -> 1\n[3, 2] -> 1\n") << "ToString should correctly represent the bit vector";
}

TEST(NewTest, Parentheses) {
    Matrix<> m1(2, 3), m2(2, 3);
    m1.set(0, 0, 1.0);
    m1.set(0, 1, 2.0);
    m1.set(1, 2, 3.0);

    m2(0, 0) = 1.0;
    m2(0, 1) =  2.0;
    m2(1, 2) = 3.0;

    ASSERT_EQ(m1, m2);

    Vector<> v1(3), v2(3);
    v1.set(0, 1);
    v1.set(1, 2);
    v1.set(2, 3);

    v2(0) = 1;
    v2(1) = 2;
    v2(2) = 3;

    ASSERT_EQ(v1, v2);
}

TEST(NewTest, Slices) {
    Matrix<>* test_matrix_p = new Matrix(3, 3, 0.01);
    auto& test_matrix = *test_matrix_p;
    test_matrix(1, 1) = 1;
    test_matrix(1, 2) = 2;
    test_matrix(2, 1) = 3;
    test_matrix(2, 2) = 4;

    Matrix_coords coords(1, 1, 3, 3);
    auto& proxy = test_matrix[coords];
    ASSERT_EQ(&test_matrix(1, 1), &proxy(0, 0)); // Проверяем что элементы среза и исходной матрицы указывают на один и тот же адресс в памяти
    proxy.set(0, 0, 2); // создаем локальную копию, поскольку пытаемся модифицировать срез
    ASSERT_NE(&test_matrix(1, 1), &proxy(0, 0)); // теперь адреса различаются
    ASSERT_EQ(test_matrix(1,1), 1);
    ASSERT_EQ(proxy(0,0), 2);

    delete test_matrix_p; // освобождаем память, убиваем матрицу
    ASSERT_THROW(proxy(0, 0), EmptyContainerError);
}

TEST(NewTest, RowSlices) {
    Matrix<>* test_matrix_p = new Matrix(3, 3, 0.01);
    auto& test_matrix = *test_matrix_p;
    test_matrix(1, 1) = 1;
    test_matrix(1, 2) = 2;
    test_matrix(2, 1) = 3;
    test_matrix(2, 2) = 4;

    Matrix_row_coord coords(1, 1, 3);
    auto& proxy = test_matrix[coords]; // proxy = [1, 2]
    ASSERT_EQ(&test_matrix(1, 1), &proxy(0)); // Проверяем что элементы среза и исходной матрицы указывают на один и тот же адресс в памяти
    proxy.set(0, 2); // создаем локальную копию, поскольку пытаемся модифицировать срез
    ASSERT_NE(&test_matrix(1, 1), &proxy(0)); // теперь адреса различаются
    ASSERT_EQ(test_matrix(1,1), 1);
    ASSERT_EQ(proxy(0), 2); // proxy = [2, 2]

    delete test_matrix_p; // освобождаем память, убиваем матрицу
    ASSERT_THROW(proxy(0), EmptyContainerError);
}

TEST(NewTest, ColSlices) {
    Matrix<>* test_matrix_p = new Matrix(3, 3, 0.01);
    auto& test_matrix = *test_matrix_p;
    test_matrix(1, 1) = 1;
    test_matrix(1, 2) = 2;
    test_matrix(2, 1) = 3;
    test_matrix(2, 2) = 4;

    Matrix_col_coord coords(1, 1, 3);
    auto& proxy = test_matrix[coords]; // proxy = [1, 3]
    ASSERT_EQ(&test_matrix(1, 1), &proxy(0)); // Проверяем что элементы среза и исходной матрицы указывают на один и тот же адресс в памяти
    proxy.set(0, 2); // создаем локальную копию, поскольку пытаемся модифицировать срез
    ASSERT_NE(&test_matrix(1, 1), &proxy(0)); // теперь адреса различаются
    ASSERT_EQ(test_matrix(1,1), 1);
    ASSERT_EQ(proxy(0), 2); // proxy = [2, 3]

    delete test_matrix_p; // освобождаем память, убиваем матрицу
    ASSERT_THROW(proxy(0), EmptyContainerError);
}

TEST(NewTests, RationalAndComplexArythmetic) {
    Rational r(5, 3);
    Complex c(1, 1);

    Complex sum1(Rational(8, 3), 1);
    ASSERT_EQ(c + r, sum1);
    Complex sub1(Rational(-2, 3), 1);
    ASSERT_EQ(c - r, sub1);
    Complex mul1(Rational(5, 3), Rational(5, 3));
    ASSERT_EQ(c * r, mul1);
    Complex div1(Rational(3, 5), Rational(3, 5));
    ASSERT_EQ(c / r, div1);

    Complex sum2(Rational(8, 3), 1);
    ASSERT_EQ(r+c, sum2);
    Complex sub2(Rational(2, 3), 1);
    ASSERT_EQ(r-c, sub2);
    Complex mul2(Rational(5, 3), Rational(5, 3));
    ASSERT_EQ(r*c, mul2);
    Complex div2(Rational(5, 6), Rational(-5, 6));
    auto temp = r/c;
    ASSERT_EQ(temp, div2);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}