#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "little_big.cpp"


class PrintMemoryTest : public ::testing::Test {
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

TEST(FunctionSignatureTest, IntSignature) {
    static_assert(std::is_same_v<decltype(static_cast<void (*)(int, bool)>(&PrintMemory)), void (*)(int, bool)>,
        "function must have signature: void PrintMemory(int, bool)");
}

TEST(FunctionSignatureTest, DoubleSignature) {
    static_assert(std::is_same_v<decltype(static_cast<void (*)(double, bool)>(&PrintMemory)), void (*)(double, bool)>,
        "function must have signature: void PrintMemory(double, bool)");
}

TEST_F(PrintMemoryTest, IntZero) {
    PrintMemory(0);
    std::string output = GetOutput();
    EXPECT_EQ(output, "0x00000000\n");
}

TEST_F(PrintMemoryTest, IntNegative) {
    PrintMemory(-1);
    std::string output = GetOutput();
    EXPECT_EQ(output, "0xFFFFFFFF\n");
}

TEST_F(PrintMemoryTest, IntHex) {
    {
        PrintMemory(0x12345678);
        std::string output = GetOutput();
        EXPECT_EQ(output, "0x78563412\n");
    }
    buffer.str("");
    {
        PrintMemory(static_cast<int>(0x89ABCDEF));
        std::string output = GetOutput();
        EXPECT_EQ(output, "0xEFCDAB89\n");
    }
}

TEST_F(PrintMemoryTest, IntReverseChecks) {
    {
        PrintMemory(0x12345678, true);
        std::string output = GetOutput();
        EXPECT_EQ(output, "0x12345678\n");
    }
    buffer.str("");
    {
        PrintMemory(0x12345678, false);
        std::string output = GetOutput();
        EXPECT_EQ(output, "0x78563412\n");
    }
}

TEST_F(PrintMemoryTest, BothFunctionsWork) {
    PrintMemory(42);
    std::string int_output = GetOutput();

    buffer.str("");

    PrintMemory(42.0);
    std::string double_output = GetOutput();

    EXPECT_NE(int_output, double_output);
}

TEST_F(PrintMemoryTest, DoubleZero) {
    PrintMemory(0.0);
    std::string output = GetOutput();
    EXPECT_EQ(output, "0x0000000000000000\n");
}

TEST_F(PrintMemoryTest, DoublePositive) {
    PrintMemory(36.6);
    std::string output = GetOutput();
    EXPECT_EQ(output, "0xCDCCCCCCCC4C4240\n");
}

TEST_F(PrintMemoryTest, DoubleNegative) {
    PrintMemory(-42.0);
    std::string output = GetOutput();
    EXPECT_EQ(output, "0x00000000000045C0\n");
}

TEST_F(PrintMemoryTest, DoubleExponent) {
    PrintMemory(-1.6513e47);
    std::string output = GetOutput();
    EXPECT_EQ(output, "0x6DD2606FAFECBCC9\n");
}

std::string GetReverse(const std::string& str) {
    std::string result = str;
    for (int i = str.size() - 2, j = 2; i > 1; i-=2, j+=2) {
        result[j] = str[i - 1];
        result[j + 1] = str[i];
    }
    return result;
}

TEST_F(PrintMemoryTest, DoubleReverseChecks) {
    {
        PrintMemory(3.123456e37, true);
        std::string output = GetOutput();
        std::string r_output = GetReverse(output);
        buffer.str("");
        PrintMemory(3.123456e37, false);
        output = GetOutput();
        EXPECT_EQ(r_output, output);
    }
    buffer.str("");
    {
        PrintMemory(3.123456e-37, true);
        std::string output = GetOutput();
        std::string r_output = GetReverse(output);
        buffer.str("");
        PrintMemory(3.123456e-37, false);
        output = GetOutput();
        EXPECT_EQ(r_output, output);
    }
}
