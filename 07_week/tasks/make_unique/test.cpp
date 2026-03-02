#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "make_unique.cpp"


struct CtorTracker {
    static inline int default_ctor = 0;
    static inline int copy_ctor = 0;
    static inline int move_ctor = 0;
    static inline int param_ctor = 0;
    static inline int dtor_calls = 0;

    static void reset() {
        default_ctor = 0;
        copy_ctor = 0;
        move_ctor = 0;
        param_ctor = 0;
        dtor_calls = 0;
    }
};

class TestObject : public CtorTracker {
public:
    TestObject() : name_("default"), value_(0) {
        ++default_ctor;
    }

    TestObject(const std::string& name, int value) : name_(name), value_(value) {
        ++param_ctor;
    }

    TestObject(const TestObject& other) : name_(other.name_), value_(other.value_) {
        ++copy_ctor;
    }

    TestObject(TestObject&& other) noexcept
        : name_(std::move(other.name_)), value_(other.value_) {
        ++move_ctor;
        other.value_ = 0;
    }

    ~TestObject() {
        ++dtor_calls;
    }

    std::string getName() const { return name_; }
    int getValue() const { return value_; }

private:
    std::string name_;
    int value_;

};

class MoveOnly : public CtorTracker {
public:
    MoveOnly(int value) : value_(value) { ++param_ctor; }
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&& other) noexcept : value_(other.value_) {
        ++move_ctor;
        other.value_ = 0;
    }
    ~MoveOnly() { ++dtor_calls; }
    int getValue() const { return value_; }

private:
    int value_;
};

class CopyOnly : public CtorTracker {
public:
    CopyOnly(int value) : value_(value) { ++param_ctor; }
    CopyOnly(const CopyOnly& other) : value_(other.value_) { ++copy_ctor; }
    CopyOnly(CopyOnly&&) = delete;
    ~CopyOnly() { ++dtor_calls; }
    int getValue() const { return value_; }

private:
    int value_;
};

class ThrowOnConstruction {
public:
    static inline bool should_throw = false;
    ThrowOnConstruction() {
        if (should_throw) throw std::runtime_error("Construction failed");
    }
    ThrowOnConstruction(int) {
        if (should_throw) throw std::runtime_error("Construction failed");
    }
};


TEST(MakeUniqueTest, BasicCreation) {
    CtorTracker::reset();
    {
        auto ptr = MakeUnique<TestObject>();
        EXPECT_EQ(CtorTracker::default_ctor, 1);
        EXPECT_EQ(CtorTracker::copy_ctor, 0);
        EXPECT_EQ(CtorTracker::move_ctor, 0);
        EXPECT_EQ(CtorTracker::param_ctor, 0);
    }
    EXPECT_EQ(CtorTracker::dtor_calls, 1);
}

TEST(MakeUniqueTest, ParameterizedCreation) {
    CtorTracker::reset();
    {
        auto ptr = MakeUnique<TestObject>("test", 42);
        EXPECT_EQ(ptr->getName(), "test");
        EXPECT_EQ(ptr->getValue(), 42);
        EXPECT_EQ(CtorTracker::param_ctor, 1);
        EXPECT_EQ(CtorTracker::copy_ctor, 0);
        EXPECT_EQ(CtorTracker::move_ctor, 0);
    }
    EXPECT_EQ(CtorTracker::dtor_calls, 1);
}

TEST(MakeUniqueTest, DifferentTypes) {
    auto int_ptr = MakeUnique<int>(42);
    EXPECT_EQ(*int_ptr, 42);

    auto str_ptr = MakeUnique<std::string>("hello");
    EXPECT_EQ(*str_ptr, "hello");

    auto vec_ptr = MakeUnique<std::vector<int>>(3, 10);
    EXPECT_EQ(vec_ptr->size(), 3);
    EXPECT_EQ((*vec_ptr)[0], 10);
    EXPECT_EQ((*vec_ptr)[1], 10);
    EXPECT_EQ((*vec_ptr)[2], 10);

    static_assert(std::is_same_v<decltype(int_ptr), std::unique_ptr<int>>);
    static_assert(std::is_same_v<decltype(str_ptr), std::unique_ptr<std::string>>);
}

TEST(MakeUniqueTest, CopyObject) {
    CtorTracker::reset();
    TestObject original("original", 500);
    EXPECT_EQ(CtorTracker::param_ctor, 1);
    {
        auto ptr = MakeUnique<TestObject>(original);
        EXPECT_EQ(ptr->getName(), "original");
        EXPECT_EQ(ptr->getValue(), 500);
    }
    EXPECT_EQ(CtorTracker::copy_ctor, 1);
    EXPECT_EQ(CtorTracker::move_ctor, 0);
}

TEST(MakeUniqueTest, MoveObject) {
    CtorTracker::reset();
    TestObject original("movable", 600);
    EXPECT_EQ(CtorTracker::param_ctor, 1);
    {
        auto ptr = MakeUnique<TestObject>(std::move(original));
        EXPECT_EQ(ptr->getName(), "movable");
        EXPECT_EQ(ptr->getValue(), 600);
    }

    EXPECT_EQ(CtorTracker::copy_ctor, 0);
    EXPECT_EQ(CtorTracker::move_ctor, 1);
}

TEST(MakeUniqueTest, MoveOnlyType) {
    CtorTracker::reset();
    {
        auto ptr = MakeUnique<MoveOnly>(42);
        EXPECT_EQ(ptr->getValue(), 42);
        static_assert(!std::is_copy_constructible_v<MoveOnly>);
        static_assert(std::is_move_constructible_v<MoveOnly>);
    }
    EXPECT_EQ(CtorTracker::param_ctor, 1);
    EXPECT_EQ(CtorTracker::move_ctor, 0);
}

TEST(MakeUniqueTest, CopyOnlyType) {
    CtorTracker::reset();
    CopyOnly original(100);
    EXPECT_EQ(CtorTracker::param_ctor, 1);
    {
        auto ptr = MakeUnique<CopyOnly>(original);
        EXPECT_EQ(ptr->getValue(), 100);
        static_assert(std::is_copy_constructible_v<CopyOnly>);
        static_assert(!std::is_move_constructible_v<CopyOnly>);
    }
    EXPECT_EQ(CtorTracker::copy_ctor, 1);
}

TEST(MakeUniqueTest, ExceptionSafety) {
    ThrowOnConstruction::should_throw = true;

    EXPECT_THROW({ auto ptr = MakeUnique<ThrowOnConstruction>(); }, std::runtime_error);
    EXPECT_THROW({ auto ptr = MakeUnique<ThrowOnConstruction>(42); }, std::runtime_error);

    ThrowOnConstruction::should_throw = false;

    EXPECT_NO_THROW({
        auto ptr = MakeUnique<ThrowOnConstruction>();
        auto ptr2 = MakeUnique<ThrowOnConstruction>(42);
    });
}

TEST(MakeUniqueTest, OwnershipSemantics) {
    auto ptr = MakeUnique<int>(42);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(*ptr, 42);

    auto ptr2 = std::move(ptr);
    EXPECT_FALSE(ptr);
    EXPECT_TRUE(ptr2);
    EXPECT_EQ(*ptr2, 42);

    ptr2.reset();
    EXPECT_FALSE(ptr2);
}

TEST(MakeUniqueTest, MultipleArguments) {
    struct MultiArg {
        int a, b, c;
        std::string d;
        MultiArg(int x, int y, int z, std::string s) : a(x), b(y), c(z), d(std::move(s)) {}
    };

    auto ptr = MakeUnique<MultiArg>(1, 2, 3, std::string("test"));
    EXPECT_EQ(ptr->a, 1);
    EXPECT_EQ(ptr->b, 2);
    EXPECT_EQ(ptr->c, 3);
    EXPECT_EQ(ptr->d, "test");
}

TEST(MakeUniqueTest, AsanTest) {
    constexpr int iterations = 10000;

    for (int i = 0; i < iterations; ++i) {
        auto ptr = MakeUnique<int>(i);
        EXPECT_EQ(*ptr, i);
    }
}

TEST(MakeUniqueTest, CompareWithStdMakeUnique) {
    auto std_ptr = std::make_unique<std::string>("test");
    auto our_ptr = MakeUnique<std::string>("test");
    EXPECT_EQ(*std_ptr, *our_ptr);

    auto std_move_ptr = std::make_unique<std::unique_ptr<int>>(std::make_unique<int>(42));
    auto our_move_ptr = MakeUnique<std::unique_ptr<int>>(std::make_unique<int>(42));
    EXPECT_EQ(**std_move_ptr, **our_move_ptr);
}

TEST(MakeUniqueTest, ConstCorrectness) {
    struct ConstTest {
        int value;
        ConstTest(int v) : value(v) {}
        void modify() { ++value; }
        int get() const { return value; }
    };

    auto ptr1 = MakeUnique<ConstTest>(10);
    ptr1->modify();

    const auto ptr2 = MakeUnique<ConstTest>(20);
    // ptr2->modify();  // compile error (const unique_ptr)
    EXPECT_EQ(ptr2->get(), 20);

    auto ptr3 = MakeUnique<const ConstTest>(30);
    // ptr3->modify();  // compile error (const obj)
    EXPECT_EQ(ptr3->get(), 30);
}

class ForwardingTester {
public:
    ForwardingTester(const std::string& s) : value_(s) {
        lastConstructor_ = "lvalue copy";
    }

    ForwardingTester(std::string&& s) : value_(std::move(s)) {
        lastConstructor_ = "rvalue move";
    }

    ForwardingTester(const char* s) : value_(s) {
        lastConstructor_ = "const char*";
    }

    static std::string getLastConstructor() { return lastConstructor_; }
    static void reset() { lastConstructor_ = ""; }

    std::string getValue() const { return value_; }

private:
    std::string value_;
    static inline std::string lastConstructor_ = "";
};

TEST(MakeUniqueTest, PerfectForwardingLValue) {
    ForwardingTester::reset();
    std::string name = "test";
    auto ptr = MakeUnique<ForwardingTester>(name);
    EXPECT_EQ(ForwardingTester::getLastConstructor(), "lvalue copy");
}

TEST(MakeUniqueTest, PerfectForwardingRValue) {
    ForwardingTester::reset();
    auto ptr = MakeUnique<ForwardingTester>(std::string("test"));
    EXPECT_EQ(ForwardingTester::getLastConstructor(), "rvalue move");
}

TEST(MakeUniqueTest, PerfectForwardingConstChar) {
    ForwardingTester::reset();
    auto ptr = MakeUnique<ForwardingTester>("test");
    EXPECT_EQ(ForwardingTester::getLastConstructor(), "const char*");
}

class CopyMoveTracker {
public:
    CopyMoveTracker(const std::string& s) : value_(s) {}

    CopyMoveTracker(const CopyMoveTracker& other) : value_(other.value_) {
        ++copyCount_;
    }

    CopyMoveTracker(CopyMoveTracker&& other) noexcept : value_(std::move(other.value_)) {
        ++moveCount_;
    }

    static void reset() { copyCount_ = 0; moveCount_ = 0; }
    static int getCopyCount() { return copyCount_; }
    static int getMoveCount() { return moveCount_; }

    std::string getValue() const { return value_; }

private:
    static inline int copyCount_ = 0;
    static inline int moveCount_ = 0;
    std::string value_;
};

TEST(MakeUniqueTest, PerfectForwardingCopyVsMove) {
    CopyMoveTracker::reset();

    CopyMoveTracker tracker("test");
    auto ptr1 = MakeUnique<CopyMoveTracker>(tracker);
    auto ptr2 = MakeUnique<CopyMoveTracker>(std::move(tracker));

    EXPECT_EQ(CopyMoveTracker::getCopyCount(), 1);
    EXPECT_EQ(CopyMoveTracker::getMoveCount(), 1);
}