#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>

#include "swap_ptr.cpp"

namespace fs = std::filesystem;

fs::path FindSourceCode() {
    std::vector<fs::path> search_paths = {
        "02_week/tasks/swap_ptr/swap_ptr.cpp",
        "tasks/swap_ptr/swap_ptr.cpp",
        "swap_ptr/swap_ptr.cpp",
        "swap_ptr.cpp",
        "tasks/swap_ptr.cpp"
    };

    for (const auto& path : search_paths) {
        if (fs::exists(path) && fs::is_regular_file(path)) {
            return fs::absolute(path);
        }
    }

    return {};
}

TEST(CodeInspectionTest, NoBitsetInCode) {
    fs::path code_path = FindSourceCode();
    std::ifstream file(code_path);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    ASSERT_TRUE(content.size()) << "Run the test from the root of repository\n";
    size_t pos = content.find("swap");
    EXPECT_EQ(pos, std::string::npos)
        << "Обнаружено использование swap в коде! " << pos;
}

TEST(SwapPtrTest, SwapIntPointers) {
    int a = 10, b = 20;
    int* ptr1 = &a;
    int* ptr2 = &b;

    EXPECT_EQ(ptr1, &a);
    EXPECT_EQ(ptr2, &b);
    EXPECT_EQ(*ptr1, 10);
    EXPECT_EQ(*ptr2, 20);

    SwapPtr(ptr1, ptr2);

    EXPECT_EQ(ptr1, &b);
    EXPECT_EQ(ptr2, &a);
    EXPECT_EQ(*ptr1, 20);
    EXPECT_EQ(*ptr2, 10);
}

TEST(SwapPtrTest, SwapIntPointersTwice) {
    int a = 10, b = 20;
    int* ptr1 = &a;
    int* ptr2 = &b;

    EXPECT_EQ(ptr1, &a);
    EXPECT_EQ(ptr2, &b);
    EXPECT_EQ(*ptr1, 10);
    EXPECT_EQ(*ptr2, 20);

    SwapPtr(ptr1, ptr2);
    SwapPtr(ptr1, ptr2);

    EXPECT_EQ(ptr1, &a);
    EXPECT_EQ(ptr2, &b);
    EXPECT_EQ(*ptr1, 10);
    EXPECT_EQ(*ptr2, 20);
}

TEST(SwapPtrTest, SwapWithNullptr) {
    int value = 42;
    int* ptr1 = &value;
    int* ptr2 = nullptr;
    
    SwapPtr(ptr1, ptr2);
    
    EXPECT_EQ(ptr1, nullptr);
    EXPECT_EQ(ptr2, &value);
    EXPECT_EQ(*ptr2, 42);
}

TEST(SwapPtrTest, SwapBothNullptr) {
    int* ptr1 = nullptr;
    int* ptr2 = nullptr;

    SwapPtr(ptr1, ptr2);
    
    EXPECT_EQ(ptr1, nullptr);
    EXPECT_EQ(ptr2, nullptr);
}

TEST(SwapPtrTest, SwapPtrToSameObject) {
    int value = 123;
    int* ptr1 = &value;
    int* ptr2 = &value;
    
    SwapPtr(ptr1, ptr2);

    EXPECT_EQ(ptr1, &value);
    EXPECT_EQ(ptr2, &value);
    EXPECT_EQ(*ptr1, 123);
    EXPECT_EQ(*ptr2, 123);
}

TEST(SwapPtrTest, SwapArrayPointers) {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6};
    int* ptr1 = arr1;
    int* ptr2 = arr2;
    
    SwapPtr(ptr1, ptr2);
    
    EXPECT_EQ(ptr1, arr2);
    EXPECT_EQ(ptr2, arr1);

    EXPECT_EQ(ptr1[0], 4);
    EXPECT_EQ(ptr1[1], 5);
    EXPECT_EQ(ptr1[2], 6);
    EXPECT_EQ(ptr2[0], 1);
    EXPECT_EQ(ptr2[1], 2);
    EXPECT_EQ(ptr2[2], 3);
}

TEST(SwapPtrTest, SwapConstPointers) {
    int a = 10, b = 20;
    const int* ptr1 = &a;
    const int* ptr2 = &b;

    SwapPtr(ptr1, ptr2);
    
    EXPECT_EQ(ptr1, &b);
    EXPECT_EQ(ptr2, &a);
    EXPECT_EQ(*ptr1, 20);
    EXPECT_EQ(*ptr2, 10);
}

TEST(SwapPtrTest, SwapPointerToPointer) {
    int a = 100, b = 200;
    int* ptr1 = &a;
    int* ptr2 = &b;
    int** pptr1 = &ptr1;
    int** pptr2 = &ptr2;

    SwapPtr(pptr1, pptr2);
    
    EXPECT_EQ(pptr1, &ptr2);
    EXPECT_EQ(pptr2, &ptr1);
    EXPECT_EQ(ptr1, &a);
    EXPECT_EQ(ptr2, &b);
    EXPECT_EQ(**pptr1, b);
    EXPECT_EQ(**pptr2, a);
}

TEST(SwapPtrTest, VerifyValuesNotChanged) {
    int a = 10, b = 20;
    int* ptr1 = &a;
    int* ptr2 = &b;
    
    SwapPtr(ptr1, ptr2);

    EXPECT_EQ(a, 10);
    EXPECT_EQ(b, 20);

    EXPECT_EQ(*ptr1, 20);
    EXPECT_EQ(*ptr2, 10);
}
