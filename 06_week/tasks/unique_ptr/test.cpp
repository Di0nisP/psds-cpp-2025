#include <gtest/gtest.h>

#include "unique_ptr.cpp"

TEST(UniquePtrTest, DefaultConstructor) {
    UniquePtr ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_FALSE(ptr);
}

TEST(UniquePtrTest, ConstructorFromRawPointer) {
    auto* raw = new std::string("test");
    UniquePtr ptr(raw);
    EXPECT_EQ(ptr.Get(), raw);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, "test");
}

TEST(UniquePtrTest, CopyConstructorDeleted) {
    bool is_copy_constructible = std::is_copy_constructible_v<UniquePtr>;
    EXPECT_FALSE(is_copy_constructible) << "Copy constructor should be deleted";
}

TEST(UniquePtrTest, CopyAssignmentDeleted) {
    bool is_copy_assignable = std::is_copy_assignable_v<UniquePtr>;
    EXPECT_FALSE(is_copy_assignable) <<  "Copy assignment should be deleted";
}

TEST(UniquePtrTest, MoveOperationsAvailable) {
    bool is_move_constructible = std::is_move_constructible_v<UniquePtr>;
    bool is_move_assignable = std::is_move_assignable_v<UniquePtr>;
    EXPECT_TRUE(is_move_constructible) << "Move constructor should be available";
    EXPECT_TRUE(is_move_assignable) << "Move assignment should be available";
}

TEST(UniquePtrTest, MoveConstructor) {
    UniquePtr ptr1(new std::string("test"));
    auto* expected = ptr1.Get();
    UniquePtr ptr2(std::move(ptr1));

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_FALSE(ptr1);
    EXPECT_EQ(ptr2.Get(), expected);
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(*ptr2, "test");
}

TEST(UniquePtrTest, MoveAssignment) {
    UniquePtr ptr1(new std::string("test1"));
    auto* expected = ptr1.Get();
    UniquePtr ptr2(new std::string("test2"));

    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr2.Get(), expected);
    EXPECT_EQ(*ptr2, "test1");
}

TEST(UniquePtrTest, SelfMoveAssignment) {
    UniquePtr ptr(new std::string("test"));
    auto* expected = ptr.Get();

    ptr = static_cast<UniquePtr&&>(ptr);

    EXPECT_EQ(ptr.Get(), expected);
    EXPECT_EQ(*ptr, "test");
}

TEST(UniquePtrTest, DereferenceOperator) {
    UniquePtr ptr(new std::string("test"));
    EXPECT_EQ(*ptr, "test");

    *ptr = "new value";
    EXPECT_EQ(*ptr, "new value");
}

TEST(UniquePtrTest, ArrowOperator) {
    UniquePtr ptr(new std::string("test"));
    EXPECT_EQ(ptr->size(), 4);
    EXPECT_EQ(ptr->empty(), false);
}

TEST(UniquePtrTest, GetMethod) {
    auto* raw = new std::string("test");
    UniquePtr ptr(raw);
    EXPECT_EQ(ptr.Get(), raw);
}

TEST(UniquePtrTest, ReleaseMethod) {
    UniquePtr ptr(new std::string("test"));
    auto* raw = ptr.Release();

    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_FALSE(ptr);
    EXPECT_EQ(*raw, "test");

    delete raw;
}

TEST(UniquePtrTest, ResetMethod) {
    UniquePtr ptr(new std::string("test"));
    auto* new_raw = new std::string("new");

    ptr.Reset(new_raw);

    EXPECT_EQ(ptr.Get(), new_raw);
    EXPECT_EQ(*ptr, "new");

    ptr.Reset();
    EXPECT_EQ(ptr.Get(), nullptr);
}

TEST(UniquePtrTest, SwapMethod) {
    UniquePtr ptr1(new std::string("test1"));
    UniquePtr ptr2(new std::string("test2"));

    std::string* raw1 = ptr1.Get();
    std::string* raw2 = ptr2.Get();

    ptr1.Swap(ptr2);

    EXPECT_EQ(ptr1.Get(), raw2);
    EXPECT_EQ(ptr2.Get(), raw1);
    EXPECT_EQ(*ptr1, "test2");
    EXPECT_EQ(*ptr2, "test1");
}

TEST(UniquePtrTest, BoolOperator) {
    UniquePtr ptr1;
    EXPECT_FALSE(ptr1);

    UniquePtr ptr2(new std::string("test"));
    EXPECT_TRUE(ptr2);

    ptr2.Reset();
    EXPECT_FALSE(ptr2);
}

TEST(UniquePtrTest, DestructorAsan) {
    auto* raw = new std::string("test");
    {
        UniquePtr ptr(raw);
        EXPECT_EQ(ptr.Get(), raw);
    }
}

TEST(MakeUniqueTest, MakeUniqueWithCopy) {
    std::string str = "test";
    UniquePtr ptr = MakeUnique(str);

    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(*ptr, "test");
    EXPECT_EQ(str, "test");
}

TEST(MakeUniqueTest, MakeUniqueWithMove) {
    std::string str = "testStringMustBeLongToAvoidSSO";
    auto* expected = str.data();
    UniquePtr ptr = MakeUnique(std::move(str));

    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(ptr->data(), expected);
    EXPECT_EQ(*ptr, "testStringMustBeLongToAvoidSSO");
}

TEST(SwapFunctionTest, SwapFunction) {
    UniquePtr ptr1(new std::string("test1"));
    UniquePtr ptr2(new std::string("test2"));

    std::string* raw1 = ptr1.Get();
    std::string* raw2 = ptr2.Get();

    Swap(ptr1, ptr2);

    EXPECT_EQ(ptr1.Get(), raw2);
    EXPECT_EQ(ptr2.Get(), raw1);
    EXPECT_EQ(*ptr1, "test2");
    EXPECT_EQ(*ptr2, "test1");
}

TEST(UniquePtrTest, UsageInCondition) {
    UniquePtr ptr1;
    UniquePtr ptr2(new std::string("test"));

    if (ptr1) {
        FAIL() << "ptr1 should be false";
    }

    if (!ptr2) {
        FAIL() << "ptr2 should be true";
    }
}

TEST(UniquePtrTest, OwnershipAfterMove) {
    UniquePtr ptr1(new std::string("test"));
    auto* raw = ptr1.Get();
    auto* expected_str = ptr1->data();
    UniquePtr ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.Get(), nullptr);

    EXPECT_NE(ptr2.Get(), nullptr);
    EXPECT_EQ(ptr2.Get(), raw);
    EXPECT_EQ(ptr2->data(), expected_str);
    EXPECT_EQ(*ptr2, "test");
}