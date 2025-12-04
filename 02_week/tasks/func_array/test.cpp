#include <gtest/gtest.h>

#include <vector>

#include "func_array.cpp"


double Add(double a, double b) {
    return a + b;
}

double Subtract(double a, double b) {
    return a - b;
}

double Multiply(double a, double b) {
    return a * b;
}

double Divide(double a, double b) {
    return a / b;
}

double SquareSum(double a, double b) {
    return a * a + b * b;
}

double Max(double a, double b) {
    return a > b ? a : b;
}

double Min(double a, double b) {
    return a < b ? a : b;
}

using FuncTypeTest = double (*)(double, double);

TEST(MathOperationsTest, BasicOperations) {
    std::vector<FuncTypeTest> ops{Add, Subtract, Multiply, Divide};
    double a = 10.0, b = 5.0;
    double result = ApplyOperations(a, b, ops.data(), ops.size());
    EXPECT_DOUBLE_EQ(result, 72.0);
}

TEST(MathOperationsTest, SingleOperation) {
    std::vector<FuncTypeTest> ops{Multiply};
    double a = 8.0, b = 2.0;
    double result = ApplyOperations(a, b, ops.data(), ops.size());
    EXPECT_DOUBLE_EQ(result, 16.0);
}

TEST(MathOperationsTest, EmptyOperationsArray) {
    double a = 10.0, b = 5.0;
    double result = ApplyOperations(a, b, nullptr, 0);
    EXPECT_DOUBLE_EQ(result, 0.0);

    std::vector<FuncTypeTest> ops;
    result = ApplyOperations(a, b, ops.data(), 0);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(MathOperationsTest, WithPowerOperation) {
    std::vector<FuncTypeTest> ops{Add, SquareSum, Multiply};
    double a = 2.0, b = 3.0;
    double result = ApplyOperations(a, b, ops.data(), ops.size());
    EXPECT_DOUBLE_EQ(result, 24.0);
}

TEST(MathOperationsTest, NegativeNumbers) {
    std::vector<FuncTypeTest> ops{Add, Multiply};
    double a = -5.0, b = 3.0;
    double result = ApplyOperations(a, b, ops.data(), ops.size());
    EXPECT_DOUBLE_EQ(result, -17.0);
}

TEST(MathOperationsTest, CustomOperations) {
    auto Average = [](double a, double b) { return (a + b) / 2.0; };
    std::vector<FuncTypeTest> ops{SquareSum, Average};
    double a = 3.0, b = 4.0;
    double result = ApplyOperations(a, b, ops.data(), ops.size());
    EXPECT_DOUBLE_EQ(result, 28.5);
}

TEST(MathOperationsTest, ArrayWithNullptr) {
    std::vector<FuncTypeTest> ops{Add, nullptr, Multiply, nullptr};
    double a = 10.0, b = 2.0;
    double result = ApplyOperations(a, b, ops.data(), ops.size());
    EXPECT_DOUBLE_EQ(result, 32.0);
}
