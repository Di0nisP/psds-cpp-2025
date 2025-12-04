#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "pretty_array.cpp"


class PrintArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        origin_cout = std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(origin_cout);
    }

    std::string GetOutput() {
        return buffer.str();
    }

    std::stringstream buffer;
    std::streambuf* origin_cout;
};

TEST_F(PrintArrayTest, EmptyArray) {
    int arr[] = {1};
    PrintArray(arr, arr);
    EXPECT_EQ(GetOutput(), "[]\n");
}

TEST_F(PrintArrayTest, Nullptr) {
    PrintArray(nullptr, nullptr);
    EXPECT_EQ(GetOutput(), "[]\n");
}

TEST_F(PrintArrayTest, SingleElement) {
    int arr[] = {42};
    PrintArray(arr, arr + 1);
    EXPECT_EQ(GetOutput(), "[42]\n");
}

TEST_F(PrintArrayTest, BasicArrayNoLimit) {
    int arr[] = {1, 2, 3, 4, 5};
    PrintArray(arr, arr + 5);
    EXPECT_EQ(GetOutput(), "[1, 2, 3, 4, 5]\n");
}

TEST_F(PrintArrayTest, PartialArray) {
    int arr[] = {1, 2, 3, 4, 5};
    PrintArray(arr + 1, arr + 4);
    EXPECT_EQ(GetOutput(), "[2, 3, 4]\n");
}

TEST_F(PrintArrayTest, ArrayWithLimitExact) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    PrintArray(arr, arr + 6, 3);
    std::string expected = "[1, 2, 3, ...\n 4, 5, 6]\n";
    EXPECT_EQ(GetOutput(), expected);
}

TEST_F(PrintArrayTest, ArrayWithLimitPartialLastLine) {
    int arr[] = {1, 2, 3, 4, 5};
    PrintArray(arr, arr + 5, 3);
    std::string expected = "[1, 2, 3, ...\n 4, 5]\n";
    EXPECT_EQ(GetOutput(), expected);
}

TEST_F(PrintArrayTest, ArrayWithLimitMultipleLines) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    PrintArray(arr, arr + 9, 2);
    std::string expected = "[1, 2, ...\n 3, 4, ...\n 5, 6, ...\n 7, 8, ...\n 9]\n";
    EXPECT_EQ(GetOutput(), expected);
}

TEST_F(PrintArrayTest, ArrayWithLimitMultipleLinesAndOneElement) {
    int arr[] = {1, 2, 3, 4, 5};
    PrintArray(arr, arr + 5, 1);
    std::string expected = "[1, ...\n 2, ...\n 3, ...\n 4, ...\n 5]\n";
    EXPECT_EQ(GetOutput(), expected);
}

TEST_F(PrintArrayTest, ArrayWithExplicitNoLimit) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    PrintArray(arr, arr + 9, 0);
    std::string expected = "[1, 2, 3, 4, 5, 6, 7, 8, 9]\n";
    EXPECT_EQ(GetOutput(), expected);
}

TEST_F(PrintArrayTest, LimitLargerThanArray) {
    int arr[] = {1, 2, 3};
    PrintArray(arr, arr + 3, 5);
    EXPECT_EQ(GetOutput(), "[1, 2, 3]\n");
}

TEST_F(PrintArrayTest, LimitEqualToArraySize) {
    int arr[] = {1, 2, 3};
    PrintArray(arr, arr + 3, 3);
    EXPECT_EQ(GetOutput(), "[1, 2, 3]\n");
}

TEST_F(PrintArrayTest, ReverseRange) {
    int arr[] = {1, 2, 3};
    PrintArray(arr + 2, arr - 1);
    EXPECT_EQ(GetOutput(), "[3, 2, 1]\n");
}

TEST_F(PrintArrayTest, ReverseRangeWithLimit) {
    int arr[] = {1, 2, 3, 4, 5};
    PrintArray(arr + 4, arr - 1, 3);
    EXPECT_EQ(GetOutput(), "[5, 4, 3, ...\n 2, 1]\n");
}

TEST_F(PrintArrayTest, WorkWithConstArray) {
    const int arr[] = {1, 2, 3, 4, 5};
    PrintArray(arr, arr + 5);
    EXPECT_EQ(GetOutput(), "[1, 2, 3, 4, 5]\n");
}

size_t CountSubstring(const std::string& str, const std::string& substr) {
    size_t count = 0;
    size_t pos = 0;
    while ((pos = str.find(substr, pos)) != std::string::npos) {
        ++count;
        pos += substr.length();
    }
    return count;
}

TEST_F(PrintArrayTest, LargeArrayWithLimit) {
    const int SIZE = 100;
    int arr[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = i + 1;
    }
    size_t limit = 17;
    PrintArray(arr, arr + SIZE, limit);
    std::string output = GetOutput();

    size_t expected_newlines = limit ? (SIZE + limit - 1) / limit : 1;
    size_t expected_dots = expected_newlines - 1;
    EXPECT_EQ(CountSubstring(output,"..."), expected_dots);
    EXPECT_EQ(CountSubstring(output,"\n"), expected_newlines);
}
