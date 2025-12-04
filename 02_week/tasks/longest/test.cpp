#include <gtest/gtest.h>

#include "longest.cpp"

#include <vector>
#include <random>
#include <algorithm>


TEST(LongestSubsequenceTest, BasicString) {
    std::string str = "aaabbbccc";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 3);
    EXPECT_EQ(*result, 'a');
    EXPECT_EQ(result, str.data());
}

TEST(LongestSubsequenceTest, BasicStringNotFirst) {
    std::string str = "abbbccddde";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 3);
    EXPECT_EQ(*result, 'b');
    EXPECT_EQ(result, str.data() + 1);
}

TEST(LongestSubsequenceTest, AllSameCharacters) {
    std::string str = "aaaaa";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 5);
    EXPECT_EQ(*result, 'a');
    EXPECT_EQ(result, str.data());
}

TEST(LongestSubsequenceTest, AllDifferentCharacters) {
    std::string str = "abcde";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 1);
    EXPECT_EQ(*result, 'a');
    EXPECT_EQ(result, str.data());
}

TEST(LongestSubsequenceTest, MultipleEqualSequences) {
    std::string str = "aabbbaacccaa";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 3);
    EXPECT_EQ(result, str.data() + 2);
    EXPECT_EQ(*result, 'b');
}

TEST(LongestSubsequenceTest, BinaryString) {
    std::string str = "1110001111";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 4);
    EXPECT_EQ(*result, '1');
    EXPECT_EQ(result, str.data() + 6);
}

TEST(LongestSubsequenceTest, EmptyString) {
    std::string str = "";
    size_t count = 100;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(count, 0);
}

TEST(LongestSubsequenceTest, SingleCharacter) {
    std::string str = "a";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 1);
    EXPECT_EQ(*result, 'a');
    EXPECT_EQ(result, str.data());
}

TEST(LongestSubsequenceTest, WithSpaces) {
    std::string str = "a  bb   ccc";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 3);
    EXPECT_EQ(*result, ' ');
    EXPECT_EQ(result, str.data() + 5);
}

TEST(LongestSubsequenceTest, MixedCase) {
    std::string str = "AbBBAaAACCCcDD";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 3);
    EXPECT_EQ(*result, 'C');
    EXPECT_EQ(result, str.data() + 8);
}

TEST(LongestSubsequenceTest, InvalidPointers) {
    std::string str = "test";
    size_t count = 10;
    
    const char* result = FindLongestSubsequence(nullptr, str.data() + str.size(), count);
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(count, 0);
    
    result = FindLongestSubsequence(str.data(), nullptr, count);
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(count, 0);
    
    result = FindLongestSubsequence(str.data() + 2, str.data(), count);
    EXPECT_EQ(result, nullptr);
    EXPECT_EQ(count, 0);
}

TEST(LongestSubsequenceTest, MutableString) {
    std::string str = "aaabbbccc";
    size_t count = 0;
    
    char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 3);
    EXPECT_EQ(*result, 'a');
    
    result[0] = 'X';
    result[1] = 'X';
    result[2] = 'X';
    EXPECT_EQ(str, "XXXbbbccc");
}

TEST(LongestSubsequenceTest, LongString) {
    std::string str;
    str.append(10, 'a');
    str.append(5, 'b');
    str.append(15, 'c');
    str.append(8, 'd');
    str.append(12, 'e');
    
    size_t count = 0;
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 15);
    EXPECT_EQ(*result, 'c');
    EXPECT_EQ(result, str.data() + 15);
}

TEST(LongestSubsequenceTest, ConstCorrectnes) {
    const std::string str = "aaabbbccc";
    size_t count = 0;

    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 3);
    EXPECT_EQ(*result, 'a');
}

TEST(LongestSubsequenceTest, MixedTwoCharSequence) {
    std::string str = "xyxxyxyy";
    size_t count = 0;
    
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, 2); // "xxxx" в начале
    EXPECT_EQ(*result, 'x');
    EXPECT_EQ(result, str.data() + 2);
}

TEST(LongestSubsequenceTest, VeryLongSequence) {
    const size_t LENGTH = 1000;
    std::string str(LENGTH, 'z'); // 1000 символов 'z'
    
    str.insert(0, "ab");
    str.append("cd");
    
    size_t count = 0;
    const char* result = FindLongestSubsequence(str.data(), str.data() + str.size(), count);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(count, LENGTH);
    EXPECT_EQ(*result, 'z');
    EXPECT_EQ(result, str.data() + 2);
}