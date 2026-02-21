#include <gtest/gtest.h>

#include "smart_ptr.cpp"

TEST(SharedPtrTest, DefaultConstructor) {
    SharedPtr ptr;
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr.UseCount(), 0);
    EXPECT_FALSE(ptr);
}

TEST(SharedPtrTest, ConstructorFromRawPointer) {
    auto* raw = new std::string("test");
    SharedPtr ptr(raw);
    EXPECT_EQ(ptr.Get(), raw);
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, "test");
}

TEST(SharedPtrTest, CopyConstructor) {
    SharedPtr ptr1(new std::string("test"));
    SharedPtr ptr2(ptr1);

    EXPECT_EQ(ptr1.Get(), ptr2.Get());
    EXPECT_EQ(ptr1.UseCount(), 2);
    EXPECT_EQ(ptr2.UseCount(), 2);
    EXPECT_EQ(*ptr1, "test");
    EXPECT_EQ(*ptr2, "test");
}

TEST(SharedPtrTest, MoveConstructor) {
    SharedPtr ptr1(new std::string("test"));
    std::string* raw = ptr1.Get();

    SharedPtr ptr2(std::move(ptr1));

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr1.UseCount(), 0);
    EXPECT_EQ(ptr2.Get(), raw);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(*ptr2, "test");
}

TEST(SharedPtrTest, CopyAssignment) {
    SharedPtr ptr1(new std::string("test1"));
    SharedPtr ptr2(new std::string("test2"));

    ptr2 = ptr1;

    EXPECT_EQ(ptr1.Get(), ptr2.Get());
    EXPECT_EQ(ptr1.UseCount(), 2);
    EXPECT_EQ(ptr2.UseCount(), 2);
    EXPECT_EQ(*ptr1, "test1");
    EXPECT_EQ(*ptr2, "test1");
}

TEST(SharedPtrTest, MoveAssignment) {
    SharedPtr ptr1(new std::string("test1"));
    SharedPtr ptr2(new std::string("test2"));
    std::string* raw = ptr1.Get();

    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr1.UseCount(), 0);
    EXPECT_EQ(ptr2.Get(), raw);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(*ptr2, "test1");
}

TEST(SharedPtrTest, SelfAssignment) {
    SharedPtr ptr(new std::string("test"));
    std::string* raw = ptr.Get();

    ptr = ptr;

    EXPECT_EQ(ptr.Get(), raw);
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(*ptr, "test");
}

TEST(SharedPtrTest, SelfMoveAssignment) {
    SharedPtr ptr(new std::string("test"));
    std::string* raw_ptr = ptr.Get();
    size_t use_count = ptr.UseCount();

    ptr = static_cast<SharedPtr&&>(ptr);

    EXPECT_EQ(ptr.Get(), raw_ptr);
    EXPECT_EQ(ptr.UseCount(), use_count);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, "test");
}

TEST(SharedPtrTest, DereferenceOperators) {
    SharedPtr ptr(new std::string("test"));

    EXPECT_EQ(*ptr, "test");
    EXPECT_EQ(ptr->size(), 4);

    *ptr = "new value";
    EXPECT_EQ(*ptr, "new value");
}

TEST(SharedPtrTest, GetMethod) {
    auto* raw = new std::string("test");
    SharedPtr ptr(raw);
    EXPECT_EQ(ptr.Get(), raw);
}

TEST(SharedPtrTest, ResetMethod) {
    SharedPtr ptr(new std::string("test"));
    auto* new_raw = new std::string("new");

    ptr.Reset(new_raw);

    EXPECT_EQ(ptr.Get(), new_raw);
    EXPECT_EQ(ptr.UseCount(), 1);
    EXPECT_EQ(*ptr, "new");

    ptr.Reset();
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_EQ(ptr.UseCount(), 0);
}

TEST(SharedPtrTest, SwapMethod) {
    SharedPtr ptr1(new std::string("test1"));
    SharedPtr ptr2(new std::string("test2"));

    std::string* raw1 = ptr1.Get();
    std::string* raw2 = ptr2.Get();

    ptr1.Swap(ptr2);

    EXPECT_EQ(ptr1.Get(), raw2);
    EXPECT_EQ(ptr2.Get(), raw1);
    EXPECT_EQ(*ptr1, "test2");
    EXPECT_EQ(*ptr2, "test1");
}

TEST(SharedPtrTest, UseCountMethod) {
    SharedPtr ptr1(new std::string("test"));
    EXPECT_EQ(ptr1.UseCount(), 1);

    {
        SharedPtr ptr2(ptr1);
        EXPECT_EQ(ptr1.UseCount(), 2);
        EXPECT_EQ(ptr2.UseCount(), 2);

        SharedPtr ptr3(ptr2);
        EXPECT_EQ(ptr1.UseCount(), 3);
        EXPECT_EQ(ptr2.UseCount(), 3);
        EXPECT_EQ(ptr3.UseCount(), 3);
    }
    EXPECT_EQ(ptr1.UseCount(), 1);
}

TEST(SharedPtrTest, BoolOperator) {
    SharedPtr ptr1;
    EXPECT_FALSE(ptr1);

    SharedPtr ptr2(new std::string("test"));
    EXPECT_TRUE(ptr2);

    ptr2.Reset();
    EXPECT_FALSE(ptr2);
}

TEST(SharedPtrTest, UseAfterMoveConstructor) {
    SharedPtr ptr1(new std::string("test"));
    std::string* raw_ptr = ptr1.Get();
    size_t initial_use_count = ptr1.UseCount();

    SharedPtr ptr2(std::move(ptr1));

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr1.UseCount(), 0);
    EXPECT_FALSE(ptr1);

    EXPECT_EQ(ptr2.Get(), raw_ptr);
    EXPECT_EQ(ptr2.UseCount(), initial_use_count);
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(*ptr2, "test");

    ptr1.Reset(new std::string("new after move"));
    EXPECT_NE(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr1.UseCount(), 1);
    EXPECT_EQ(*ptr1, "new after move");

    EXPECT_EQ(*ptr2, "test");
}

TEST(SharedPtrTest, UseAfterMoveAssignment) {
    SharedPtr ptr1(new std::string("test1"));
    SharedPtr ptr2(new std::string("test2"));

    std::string* raw_ptr1 = ptr1.Get();

    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr1.UseCount(), 0);
    EXPECT_FALSE(ptr1);

    EXPECT_EQ(ptr2.Get(), raw_ptr1);
    EXPECT_EQ(ptr2.UseCount(), 1);
    EXPECT_EQ(*ptr2, "test1");

    ptr1.Reset(new std::string("reused"));
    EXPECT_EQ(*ptr1, "reused");
    EXPECT_EQ(ptr1.UseCount(), 1);
}

TEST(SharedPtrTest, MultipleMoves) {
    SharedPtr ptr1(new std::string("test"));
    std::string* original_ptr = ptr1.Get();

    SharedPtr ptr2(std::move(ptr1));
    SharedPtr ptr3(std::move(ptr2));
    SharedPtr ptr4(std::move(ptr3));

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_EQ(ptr1.UseCount(), 0);
    EXPECT_FALSE(ptr1);

    EXPECT_EQ(ptr2.Get(), nullptr);
    EXPECT_EQ(ptr2.UseCount(), 0);
    EXPECT_FALSE(ptr2);

    EXPECT_EQ(ptr3.Get(), nullptr);
    EXPECT_EQ(ptr3.UseCount(), 0);
    EXPECT_FALSE(ptr3);

    EXPECT_EQ(ptr4.Get(), original_ptr);
    EXPECT_EQ(ptr4.UseCount(), 1);
    EXPECT_TRUE(ptr4);
    EXPECT_EQ(*ptr4, "test");
}

// WeakPtr Tests

TEST(WeakPtrTest, DefaultConstructor) {
    WeakPtr wp;
    EXPECT_TRUE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 0);
}

TEST(WeakPtrTest, ConstructorFromSharedPtr) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp(sp);

    EXPECT_FALSE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 1);
}

TEST(WeakPtrTest, CopyConstructor) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp1(sp);
    WeakPtr wp2(wp1);

    EXPECT_FALSE(wp1.Expired());
    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(wp1.UseCount(), 1);
    EXPECT_EQ(wp2.UseCount(), 1);
}

TEST(WeakPtrTest, MoveConstructor) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp1(sp);

    WeakPtr wp2(std::move(wp1));

    EXPECT_TRUE(wp1.Expired());
    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(wp2.UseCount(), 1);
    auto sp_from_wp = wp2.Lock();
    EXPECT_EQ(*sp_from_wp, "test");
}

TEST(WeakPtrTest, CopyAssignment) {
    SharedPtr sp1(new std::string("test1"));
    SharedPtr sp2(new std::string("test2"));
    WeakPtr wp1(sp1);
    WeakPtr wp2(sp2);

    wp2 = wp1;

    EXPECT_EQ(wp2.UseCount(), 1);
    EXPECT_FALSE(wp2.Expired());
    auto sp_from_wp = wp2.Lock();
    EXPECT_EQ(*sp_from_wp, "test1");
}

TEST(WeakPtrTest, MoveAssignment) {
    SharedPtr sp1(new std::string("test1"));
    SharedPtr sp2(new std::string("test2"));
    WeakPtr wp1(sp1);
    WeakPtr wp2(sp2);

    wp2 = std::move(wp1);

    EXPECT_TRUE(wp1.Expired());
    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(wp2.UseCount(), 1);
    auto sp_from_wp = wp2.Lock();
    EXPECT_EQ(*sp_from_wp, "test1");
}

TEST(WeakPtrTest, AssignmentFromSharedPtr) {
    SharedPtr sp1(new std::string("test1"));
    SharedPtr sp2(new std::string("test2"));
    WeakPtr wp(sp1);

    wp = sp2;

    EXPECT_EQ(wp.UseCount(), 1);
    EXPECT_FALSE(wp.Expired());
    auto sp_from_wp = wp.Lock();
    EXPECT_EQ(*sp_from_wp, "test2");
}

TEST(WeakPtrTest, ResetMethod) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp(sp);

    EXPECT_FALSE(wp.Expired());

    wp.Reset();

    EXPECT_TRUE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 0);
}

TEST(WeakPtrTest, SwapMethod) {
    SharedPtr sp1(new std::string("test1"));
    SharedPtr sp2(new std::string("test2"));
    WeakPtr wp1(sp1);
    WeakPtr wp2(sp2);

    wp1.Swap(wp2);

    EXPECT_EQ(*wp1.Lock(), "test2");
    EXPECT_EQ(*wp2.Lock(), "test1");
}

TEST(WeakPtrTest, UseCountMethod) {
    SharedPtr sp1(new std::string("test"));
    WeakPtr wp1(sp1);

    EXPECT_EQ(wp1.UseCount(), 1);

    {
        SharedPtr sp2(sp1);
        EXPECT_EQ(wp1.UseCount(), 2);
    }

    EXPECT_EQ(wp1.UseCount(), 1);
    auto sp_from_wp = wp1.Lock();
    EXPECT_EQ(wp1.UseCount(), 2);
    EXPECT_EQ(sp1.UseCount(), 2);
}

TEST(WeakPtrTest, ExpiredMethod) {
    WeakPtr wp;
    EXPECT_TRUE(wp.Expired());

    SharedPtr sp(new std::string("test"));
    WeakPtr wp2(sp);
    EXPECT_FALSE(wp2.Expired());

    sp.Reset();
    EXPECT_TRUE(wp2.Expired());
}

TEST(WeakPtrTest, LockMethod) {
    SharedPtr sp1(new std::string("test"));
    WeakPtr wp(sp1);

    SharedPtr sp2 = wp.Lock();
    EXPECT_EQ(sp2.Get(), sp1.Get());
    EXPECT_EQ(sp1.UseCount(), 2);
    EXPECT_EQ(sp2.UseCount(), 2);
    EXPECT_EQ(wp.UseCount(), 2);

    sp1.Reset();
    EXPECT_FALSE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 1);

    sp2.Reset();
    EXPECT_TRUE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 0);

    SharedPtr sp3 = wp.Lock();
    EXPECT_EQ(sp3.Get(), nullptr);
    EXPECT_EQ(sp3.UseCount(), 0);
}

TEST(WeakPtrTest, UseAfterMoveConstructor) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp1(sp);
    std::string* raw_ptr = sp.Get();

    WeakPtr wp2(std::move(wp1));

    EXPECT_TRUE(wp1.Expired());
    EXPECT_EQ(wp1.UseCount(), 0);

    wp1 = sp;
    EXPECT_FALSE(wp1.Expired());
    EXPECT_EQ(wp1.UseCount(), 1);
    EXPECT_EQ(wp1.Lock().Get(), raw_ptr);

    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(wp2.UseCount(), 1);
    EXPECT_EQ(wp2.Lock().Get(), raw_ptr);
}

TEST(WeakPtrTest, UseAfterMoveAssignment) {
    SharedPtr sp1(new std::string("test1"));
    SharedPtr sp2(new std::string("test2"));

    WeakPtr wp1(sp1);
    WeakPtr wp2(sp2);

    std::string* raw_ptr1 = sp1.Get();

    wp2 = std::move(wp1);

    EXPECT_TRUE(wp1.Expired());
    EXPECT_EQ(wp1.UseCount(), 0);

    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(wp2.UseCount(), 1);
    EXPECT_EQ(wp2.Lock().Get(), raw_ptr1);
    EXPECT_EQ(*(wp2.Lock()), "test1");

    wp1 = sp2;
    EXPECT_FALSE(wp1.Expired());
    EXPECT_EQ(*(wp1.Lock()), "test2");
}

TEST(WeakPtrTest, MultipleMoves) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp1(sp);

    WeakPtr wp2(std::move(wp1));
    WeakPtr wp3(std::move(wp2));
    WeakPtr wp4(std::move(wp3));

    EXPECT_TRUE(wp1.Expired());
    EXPECT_TRUE(wp2.Expired());
    EXPECT_TRUE(wp3.Expired());

    EXPECT_FALSE(wp4.Expired());
    EXPECT_EQ(wp4.UseCount(), 1);
    EXPECT_EQ(*(wp4.Lock()), "test");
}

TEST(WeakPtrTest, SelfMoveAssignment) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp(sp);
    std::string* raw_ptr = sp.Get();

    wp = static_cast<WeakPtr&&>(wp);

    EXPECT_FALSE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 1);
    EXPECT_EQ(wp.Lock().Get(), raw_ptr);
}

TEST(WeakPtrTest, UseAfterMoveWithExpiredObject) {
    WeakPtr wp;

    {
        SharedPtr sp(new std::string("temporary"));
        wp = sp;
        EXPECT_FALSE(wp.Expired());
    }

    EXPECT_TRUE(wp.Expired());

    WeakPtr wp2(std::move(wp));

    EXPECT_TRUE(wp.Expired());
    EXPECT_TRUE(wp2.Expired());

    SharedPtr locked = wp2.Lock();
    EXPECT_EQ(locked.Get(), nullptr);
    EXPECT_EQ(locked.UseCount(), 0);
}

// Interaction tests

TEST(SharedWeakTest, WeakPtrExpiresWhenAllSharedPtrsDestroyed) {
    WeakPtr wp;

    {
        SharedPtr sp(new std::string("test"));
        wp = sp;
        EXPECT_FALSE(wp.Expired());
        EXPECT_EQ(wp.UseCount(), 1);
    }

    EXPECT_TRUE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 0);
}

TEST(SharedWeakTest, MultipleWeakPtrs) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp1(sp);
    WeakPtr wp2(sp);
    WeakPtr wp3(wp1);

    EXPECT_EQ(wp1.UseCount(), 1);
    EXPECT_EQ(wp2.UseCount(), 1);
    EXPECT_EQ(wp3.UseCount(), 1);

    {
        SharedPtr sp2(sp);
        EXPECT_EQ(wp1.UseCount(), 2);
        EXPECT_EQ(wp2.UseCount(), 2);
        EXPECT_EQ(wp3.UseCount(), 2);
    }

    EXPECT_EQ(wp1.UseCount(), 1);
    EXPECT_EQ(wp2.UseCount(), 1);
    EXPECT_EQ(wp3.UseCount(), 1);
}


TEST(SharedWeakTest, WeakPtrObservingMovedSharedPtr) {
    SharedPtr sp1(new std::string("test"));
    WeakPtr wp(sp1);

    SharedPtr sp2(std::move(sp1));

    EXPECT_FALSE(wp.Expired());
    EXPECT_EQ(wp.UseCount(), 1);
    EXPECT_EQ(*(wp.Lock()), "test");
    EXPECT_EQ(wp.Lock().Get(), sp2.Get());
}

TEST(SharedWeakTest, MultipleWeakPtrsAfterMoves) {
    SharedPtr sp(new std::string("test"));
    WeakPtr wp1(sp);
    WeakPtr wp2(wp1);
    WeakPtr wp3(wp2);

    SharedPtr sp2(std::move(sp));

    EXPECT_FALSE(wp1.Expired());
    EXPECT_FALSE(wp2.Expired());
    EXPECT_FALSE(wp3.Expired());

    EXPECT_EQ(wp1.UseCount(), 1);
    EXPECT_EQ(wp2.UseCount(), 1);
    EXPECT_EQ(wp3.UseCount(), 1);

    EXPECT_EQ(wp1.Lock().Get(), sp2.Get());
    EXPECT_EQ(wp2.Lock().Get(), sp2.Get());
    EXPECT_EQ(wp3.Lock().Get(), sp2.Get());
}

TEST(SharedWeakTest, MoveSharedPtrWithActiveWeakPtrs) {
    SharedPtr sp1(new std::string("test"));
    WeakPtr wp1(sp1);
    WeakPtr wp2(sp1);

    SharedPtr sp2(std::move(sp1));

    EXPECT_FALSE(wp1.Expired());
    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(wp1.UseCount(), 1);
    EXPECT_EQ(wp2.UseCount(), 1);

    SharedPtr sp3 = wp1.Lock();
    EXPECT_EQ(sp3.Get(), sp2.Get());
    EXPECT_EQ(sp2.UseCount(), 2);
    EXPECT_EQ(sp3.UseCount(), 2);
}

TEST(ComplexTest, ComplexMoveSequence) {
    SharedPtr sp1(new std::string("original"));
    WeakPtr wp1(sp1);

    SharedPtr sp2(std::move(sp1));
    WeakPtr wp2(sp2);

    EXPECT_EQ(sp1.Get(), nullptr);
    EXPECT_FALSE(sp1);
    EXPECT_FALSE(wp1.Expired());
    EXPECT_EQ(wp1.Lock().Get(), sp2.Get());

    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(wp2.Lock().Get(), sp2.Get());

    WeakPtr wp3(std::move(wp2));

    EXPECT_TRUE(wp2.Expired());
    EXPECT_FALSE(wp3.Expired());
    EXPECT_EQ(wp3.Lock().Get(), sp2.Get());

    sp2.Reset();

    EXPECT_TRUE(wp1.Expired());
    EXPECT_TRUE(wp2.Expired());
    EXPECT_TRUE(wp3.Expired());

    EXPECT_EQ(wp1.Lock().Get(), nullptr);
    EXPECT_EQ(wp2.Lock().Get(), nullptr);
    EXPECT_EQ(wp3.Lock().Get(), nullptr);
}

TEST(ComplexTest, ReuseAfterMove) {
    SharedPtr sp1(new std::string("first"));
    SharedPtr sp2(new std::string("second"));

    WeakPtr wp1(sp1);
    WeakPtr wp2(sp2);

    sp2 = std::move(sp1);

    sp1.Reset(new std::string("third"));
    EXPECT_EQ(*sp1, "third");
    EXPECT_EQ(*sp2, "first");

    EXPECT_FALSE(wp1.Expired());
    EXPECT_TRUE(wp2.Expired());

    EXPECT_EQ(*(wp1.Lock()), "first");
    EXPECT_EQ(wp1.Lock().Get(), sp2.Get());

    wp2 = sp1;
    EXPECT_FALSE(wp2.Expired());
    EXPECT_EQ(*wp2.Lock(), "third");
}

// MakeShared tests

TEST(MakeSharedTest, MakeSharedWithCopy) {
    std::string str = "test";
    SharedPtr ptr = MakeShared(str);

    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(*ptr, "test");
    EXPECT_EQ(str, "test");
    EXPECT_EQ(ptr.UseCount(), 1);
}

TEST(MakeSharedTest, MakeSharedWithMove) {
    std::string str = "testWithLongStringForAvoidSSO";
    auto* expected = str.data();
    SharedPtr ptr = MakeShared(std::move(str));

    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(ptr->data(), expected);
    EXPECT_EQ(*ptr, "testWithLongStringForAvoidSSO");
    EXPECT_EQ(ptr.UseCount(), 1);
}

// Swap function tests

TEST(SwapFunctionTest, SwapSharedPtr) {
    SharedPtr ptr1(new std::string("test1"));
    SharedPtr ptr2(new std::string("test2"));

    std::string* raw1 = ptr1.Get();
    std::string* raw2 = ptr2.Get();

    Swap(ptr1, ptr2);

    EXPECT_EQ(ptr1.Get(), raw2);
    EXPECT_EQ(ptr2.Get(), raw1);
    EXPECT_EQ(*ptr1, "test2");
    EXPECT_EQ(*ptr2, "test1");
}

TEST(SwapFunctionTest, SwapWeakPtr) {
    SharedPtr sp1(new std::string("test1"));
    SharedPtr sp2(new std::string("test2"));
    WeakPtr wp1(sp1);
    WeakPtr wp2(sp2);

    Swap(wp1, wp2);

    EXPECT_EQ(*wp1.Lock(), "test2");
    EXPECT_EQ(*wp2.Lock(), "test1");
}

// Compile-time checks

TEST(CompileTimeTest, CopyOperationsAvailable) {
    static_assert(std::is_copy_constructible_v<SharedPtr>,
        "SharedPtr should be copy constructible");
    static_assert(std::is_copy_assignable_v<SharedPtr>,
        "SharedPtr should be copy assignable");
    static_assert(std::is_copy_constructible_v<WeakPtr>,
        "WeakPtr should be copy constructible");
    static_assert(std::is_copy_assignable_v<WeakPtr>,
        "WeakPtr should be copy assignable");
}

TEST(CompileTimeTest, MoveOperationsAvailable) {
    static_assert(std::is_move_constructible_v<SharedPtr>,
        "SharedPtr should be move constructible");
    static_assert(std::is_move_assignable_v<SharedPtr>,
        "SharedPtr should be move assignable");
    static_assert(std::is_move_constructible_v<WeakPtr>,
        "WeakPtr should be move constructible");
    static_assert(std::is_move_assignable_v<WeakPtr>,
        "WeakPtr should be move assignable");
}
