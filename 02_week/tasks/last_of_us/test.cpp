#include <gtest/gtest.h>

#include "last_of_us.cpp"

#include <vector>
#include <random>
#include <algorithm>

bool IsPositive(int x) {
    return x > 0;
}

bool IsNegative(int x) {
    return x < 0;
}

bool IsEven(int x) {
    return x % 2 == 0;
}

bool IsOdd(int x) {
    return x % 2 != 0;
}

class FindLastElementTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_array_ = {1, -2, 3, -4, 5, -6, 7, -8, 9, -10};
        empty_array_ = {};
        single_element_ = {42};
        mixed_array_ = {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
        palindromes_ = {121, 1331, 12321, 45654, 1, 22, 333};
    }

    std::vector<int> test_array_;
    std::vector<int> empty_array_;
    std::vector<int> single_element_;
    std::vector<int> mixed_array_;
    std::vector<int> palindromes_;
};

TEST_F(FindLastElementTest, BasicPositiveNumbers) {
    const int* result = FindLastElement(test_array_.data(), test_array_.data() + test_array_.size(), IsPositive);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 9);
    EXPECT_EQ(result, test_array_.data() + 8);
}

TEST_F(FindLastElementTest, BasicNegativeNumbers) {
    const int* result = FindLastElement(test_array_.data(), test_array_.data() + test_array_.size(), IsNegative);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, -10);
    EXPECT_EQ(result, test_array_.data() + 9);
}

TEST_F(FindLastElementTest, BasicNotFound) {
    const int* result = FindLastElement(test_array_.data(), test_array_.data() + test_array_.size(),
        [](int value) { return value > 10; });

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result, test_array_.data() + test_array_.size());
}

TEST_F(FindLastElementTest, BasicFirstElement) {
    const int* result = FindLastElement(test_array_.data(), test_array_.data() + test_array_.size(),
        [](int value) { return value == 1; });

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result, test_array_.data());
    EXPECT_EQ(*result, 1);
}

TEST_F(FindLastElementTest, EmptyArray) {
    const int* result = FindLastElement(empty_array_.data(), empty_array_.data(), IsPositive);
    EXPECT_EQ(result, empty_array_.data());
}

TEST_F(FindLastElementTest, SingleElementPositive) {
    const int* result = FindLastElement(single_element_.data(), single_element_.data() + 1, IsPositive);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 42);
}

TEST_F(FindLastElementTest, SingleElementNotFound) {
    const int* result = FindLastElement(single_element_.data(), single_element_.data() + 1, IsNegative);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result, single_element_.data() + 1);
}

TEST_F(FindLastElementTest, EvenNumbers) {
    const int* result = FindLastElement(mixed_array_.data(), mixed_array_.data() + mixed_array_.size(), IsEven);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 10);
    EXPECT_EQ(result, mixed_array_.data() + 4);
}

TEST_F(FindLastElementTest, OddNumbers) {
    const int* result = FindLastElement(mixed_array_.data(), mixed_array_.data() + mixed_array_.size(), IsOdd);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 9);
    EXPECT_EQ(result, mixed_array_.data() + 9);
}

TEST_F(FindLastElementTest, InRange) {
    const int* result = FindLastElement(mixed_array_.data(), mixed_array_.data() + mixed_array_.size(),
        [](int value) { return value >= 0 && value <= 2; });
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 1);
    EXPECT_EQ(result, mixed_array_.data() + 5);
}

TEST_F(FindLastElementTest, InvalidPointers) {
    const int* result = FindLastElement(nullptr, test_array_.data() + 5, IsPositive);
    EXPECT_EQ(result, test_array_.data() + 5);

    result = FindLastElement(test_array_.data(), nullptr, IsPositive);
    EXPECT_EQ(result, nullptr);

    result = FindLastElement(nullptr, nullptr, IsPositive);
    EXPECT_EQ(result, nullptr);

    result = FindLastElement(test_array_.data() + 5, test_array_.data(), IsPositive);
    EXPECT_EQ(result, test_array_.data());
}

TEST_F(FindLastElementTest, RandomArray) {
    std::vector<int> random_array(100);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-100, 100);

    for (int& val : random_array) {
        val = dist(gen);
    }

    const int* expected = nullptr;
    for (size_t i = 0; i < random_array.size(); ++i) {
        if (random_array[i] > 0) {
            expected = random_array.data() + i;
        }
    }

    const int* result = FindLastElement(random_array.data(), random_array.data() + random_array.size(), IsPositive);

    if (expected == nullptr) {
        EXPECT_EQ(result, nullptr);
    } else {
        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result, expected);
        EXPECT_EQ(*result, *expected);
    }
}

TEST_F(FindLastElementTest, ConstCorrectness) {
    const std::vector<int> const_array = {1, 2, 3, 4, 5};
    const int* result = FindLastElement(const_array.data(), const_array.data() + const_array.size(), IsEven);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 4);
    EXPECT_EQ(result, const_array.data() + 3);
}