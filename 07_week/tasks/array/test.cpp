#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "array.cpp"

#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <type_traits>


class TestObject {
public:
    TestObject() : value_(0) {
        ++alive_count_;
        ++default_count_;
    }

    explicit TestObject(int value) : value_(value) {
        ++alive_count_;
    }

    TestObject(const TestObject& other) : value_(other.value_) {
        ++alive_count_;
        ++copy_count_;
    }

    TestObject(TestObject&& other) noexcept : value_(other.value_) {
        other.value_ = 0;
        ++alive_count_;
        ++move_count_;
    }

    ~TestObject() {
        --alive_count_;
    }

    TestObject& operator=(const TestObject& other) {
        if (this != &other) {
            value_ = other.value_;
            ++copy_count_;
        }
        return *this;
    }

    TestObject& operator=(TestObject&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            other.value_ = 0;
            ++move_count_;
        }
        return *this;
    }

    int GetValue() const { return value_; }
    void SetValue(int value) { value_ = value; }

    static int AliveCount() { return alive_count_; }
    static int CopyCount() { return copy_count_; }
    static int MoveCount() { return move_count_; }
    static int DefaultCount() { return default_count_; }

    static void ResetCounters() {
        alive_count_ = 0;
        copy_count_ = 0;
        move_count_ = 0;
        default_count_ = 0;
    }

    bool operator==(const TestObject& other) const { return value_ == other.value_; }
    bool operator<(const TestObject& other) const { return value_ < other.value_; }

private:
    int value_;
    static inline int alive_count_ = 0;
    static inline int copy_count_ = 0;
    static inline int move_count_ = 0;
    static inline int default_count_ = 0;
};

TEST(ArrayTest, DefaultConstructor) {
    Array<int, 5> arr = {};
    EXPECT_EQ(arr.Size(), 5);
    EXPECT_FALSE(arr.Empty());
}

TEST(ArrayTest, SizeZero) {
    Array<int, 0> empty;
    EXPECT_EQ(empty.Size(), 0);
    EXPECT_TRUE(empty.Empty());
}

TEST(ArrayTest, InitializerListConstructor) {
    Array<int, 5> arr = {1, 2, 3, 4, 5};

    EXPECT_EQ(arr.Size(), 5);
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[4], 5);

    Array<int, 5> arr2 = {10, 20};
    EXPECT_EQ(arr2[0], 10);
    EXPECT_EQ(arr2[1], 20);
}

TEST(ArrayTest, CopyConstructor) {
    TestObject::ResetCounters();
    {
        Array<TestObject, 3> arr1;
        arr1[0] = TestObject(10);
        arr1[1] = TestObject(20);
        arr1[2] = TestObject(30);

        int copyBefore = TestObject::CopyCount();

        Array<TestObject, 3> arr2 = arr1;
        EXPECT_EQ(TestObject::CopyCount(), copyBefore + 3);

        EXPECT_EQ(arr2[0].GetValue(), 10);
        EXPECT_EQ(arr2[1].GetValue(), 20);
        EXPECT_EQ(arr2[2].GetValue(), 30);

        arr2[0].SetValue(100);
        EXPECT_EQ(arr2[0].GetValue(), 100);
        EXPECT_EQ(arr1[0].GetValue(), 10);
    }
    EXPECT_EQ(TestObject::AliveCount(), 0);
}

TEST(ArrayTest, MoveConstructor) {
    TestObject::ResetCounters();
    {
        Array<TestObject, 3> arr1;
        arr1[0] = TestObject(10);
        arr1[1] = TestObject(20);
        arr1[2] = TestObject(30);

        int moveBefore = TestObject::MoveCount();

        Array<TestObject, 3> arr2 = std::move(arr1);
        EXPECT_EQ(TestObject::MoveCount(), moveBefore + 3);

        EXPECT_EQ(arr2[0].GetValue(), 10);
        EXPECT_EQ(arr2[1].GetValue(), 20);
        EXPECT_EQ(arr2[2].GetValue(), 30);
    }
    EXPECT_EQ(TestObject::AliveCount(), 0);
}

TEST(ArrayTest, CopyAssignment) {
    TestObject::ResetCounters();
    {
        Array<TestObject, 3> arr1;
        arr1[0] = TestObject(10);
        arr1[1] = TestObject(20);
        arr1[2] = TestObject(30);

        Array<TestObject, 3> arr2;
        int copyBefore = TestObject::CopyCount();

        arr2 = arr1;
        EXPECT_EQ(TestObject::CopyCount(), copyBefore + 3);

        EXPECT_EQ(arr2[0].GetValue(), 10);
        EXPECT_EQ(arr2[1].GetValue(), 20);
        EXPECT_EQ(arr2[2].GetValue(), 30);

        arr2 = arr2;
        EXPECT_EQ(TestObject::CopyCount(), copyBefore + 3);
    }
}

TEST(ArrayTest, MoveAssignment) {
    TestObject::ResetCounters();
    {
        Array<TestObject, 3> arr1;
        arr1[0] = TestObject(10);
        arr1[1] = TestObject(20);
        arr1[2] = TestObject(30);

        Array<TestObject, 3> arr2;
        int moveBefore = TestObject::MoveCount();

        arr2 = std::move(arr1);
        EXPECT_EQ(TestObject::MoveCount(), moveBefore + 3);

        EXPECT_EQ(arr2[0].GetValue(), 10);
        EXPECT_EQ(arr2[1].GetValue(), 20);
        EXPECT_EQ(arr2[2].GetValue(), 30);

        arr2 = static_cast<Array<TestObject, 3>&&>(arr2);
    }
}

TEST(ArrayTest, ElementAccess) {
    Array<int, 5> arr = {1, 2, 3, 4, 5};

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[4], 5);

    arr[2] = 100;
    EXPECT_EQ(arr[2], 100);

    EXPECT_EQ(arr.Front(), 1);
    EXPECT_EQ(arr.Back(), 5);

    arr.Front() = 10;
    arr.Back() = 50;
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[4], 50);

    int* data = arr.Data();
    EXPECT_EQ(data[0], 10);
    EXPECT_EQ(data[4], 50);

    data[1] = 20;
    EXPECT_EQ(arr[1], 20);
}

TEST(ArrayTest, ConstElementAccess) {
    const Array<int, 5> arr = {1, 2, 3, 4, 5};

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[4], 5);

    EXPECT_EQ(arr.Front(), 1);
    EXPECT_EQ(arr.Back(), 5);

    const int* data = arr.Data();
    EXPECT_EQ(data[0], 1);
    EXPECT_EQ(data[4], 5);

    static_assert(std::is_same_v<decltype(arr[0]), const int&>);
    static_assert(std::is_same_v<decltype(arr.Front()), const int&>);
    static_assert(std::is_same_v<decltype(arr.Data()), const int*>);
}

TEST(ArrayTest, FillMethod) {
    Array<int, 5> arr;
    arr.Fill(42);

    for (size_t i = 0; i < arr.Size(); ++i) {
        EXPECT_EQ(arr[i], 42);
    }

    Array<std::string, 3> strArr;
    strArr.Fill("hello");
    EXPECT_EQ(strArr[0], "hello");
    EXPECT_EQ(strArr[1], "hello");
    EXPECT_EQ(strArr[2], "hello");
}

TEST(ArrayTest, SwapMethod) {
    Array<int, 5> arr1 = {1, 2, 3, 4, 5};
    Array<int, 5> arr2 = {10, 20, 30, 40, 50};

    arr1.Swap(arr2);

    EXPECT_EQ(arr1[0], 10);
    EXPECT_EQ(arr1[4], 50);
    EXPECT_EQ(arr2[0], 1);
    EXPECT_EQ(arr2[4], 5);

    // Внешняя функция swap
    swap(arr1, arr2);
    EXPECT_EQ(arr1[0], 1);
    EXPECT_EQ(arr2[0], 10);
}

TEST(ArrayTest, SwapMethodNoCopy) {
    Array<TestObject, 3> arr1 = {
        TestObject(1),
        TestObject(2),
        TestObject(3)
    };
    Array<TestObject, 3> arr2 = {
        TestObject(10),
        TestObject(20),
        TestObject(30)
    };

    auto copy_count = TestObject::CopyCount();
    arr1.Swap(arr2);
    EXPECT_EQ(copy_count, TestObject::CopyCount());

    EXPECT_EQ(arr1[0].GetValue(), 10);
    EXPECT_EQ(arr1[2].GetValue(), 30);
    EXPECT_EQ(arr2[0].GetValue(), 1);
    EXPECT_EQ(arr2[2].GetValue(), 3);
}

TEST(ArrayTest, Iterators) {
    Array<int, 5> arr = {1, 2, 3, 4, 5};

    int expected = 1;
    for (auto& elem : arr) {
        EXPECT_EQ(elem, expected++);
    }

    for (auto it = arr.begin(); it != arr.end(); ++it) {
        *it *= 2;
    }

    EXPECT_EQ(arr[0], 2);
    EXPECT_EQ(arr[1], 4);
    EXPECT_EQ(arr[2], 6);
    EXPECT_EQ(arr[3], 8);
    EXPECT_EQ(arr[4], 10);

    const Array<int, 5>& constArr = arr;
    expected = 2;
    for (auto it = constArr.cbegin(); it != constArr.cend(); ++it) {
        EXPECT_EQ(*it, expected);
        expected += 2;
    }
}

TEST(ArrayTest, EqualityOperators) {
    Array<int, 3> arr1 = {1, 2, 3};
    Array<int, 3> arr2 = {1, 2, 3};
    Array<int, 3> arr3 = {1, 2, 4};

    EXPECT_TRUE(arr1 == arr2);
    EXPECT_FALSE(arr1 != arr2);

    EXPECT_TRUE(arr1 != arr3);
    EXPECT_FALSE(arr1 == arr3);

    const Array<int, 3>& const_arr1 = arr1;
    const Array<int, 3>& const_arr2 = arr2;
    const Array<int, 3>& const_arr3 = arr3;

    EXPECT_TRUE(const_arr1 == const_arr2);
    EXPECT_FALSE(const_arr1 != const_arr2);

    EXPECT_TRUE(const_arr1 != const_arr3);
    EXPECT_FALSE(const_arr1 == const_arr3);
}

TEST(ArrayTest, RelationalOperators) {
    Array<int, 3> arr1 = {1, 2, 3};
    const Array<int, 3> arr2 = {1, 2, 4};
    const Array<int, 3> arr3 = {0, 2, 3};

    EXPECT_TRUE(arr1 < arr2);
    EXPECT_TRUE(arr1 <= arr2);
    EXPECT_FALSE(arr1 > arr2);
    EXPECT_FALSE(arr1 >= arr2);

    EXPECT_TRUE(arr3 < arr1);
    EXPECT_TRUE(arr3 <= arr1);
    EXPECT_FALSE(arr3 > arr1);
    EXPECT_FALSE(arr3 >= arr1);

    const Array<int, 3> arr4 = {1, 2, 3};
    EXPECT_TRUE(arr1 <= arr4);
    EXPECT_TRUE(arr1 >= arr4);
    EXPECT_FALSE(arr1 < arr4);
    EXPECT_FALSE(arr1 > arr4);
}

TEST(ArrayTest, GetFunction) {
    Array<int, 5> arr = {1, 2, 3, 4, 5};

    EXPECT_EQ(get<0>(arr), 1);
    EXPECT_EQ(get<2>(arr), 3);
    EXPECT_EQ(get<4>(arr), 5);

    get<1>(arr) = 100;
    EXPECT_EQ(arr[1], 100);

    const Array<int, 5>& constArr = arr;
    EXPECT_EQ(get<0>(constArr), 1);

    static_assert(std::is_same_v<decltype(get<0>(arr)), int&>);
    static_assert(std::is_same_v<decltype(get<0>(constArr)), const int&>);

    Array<int, 3> temp = {10, 20, 30};
    int&& rvalue = get<1>(std::move(temp));
    EXPECT_EQ(rvalue, 20);
}

TEST(ArrayTest, STLCompatibility) {
    Array<int, 5> arr = {5, 2, 4, 1, 3};

    std::sort(arr.begin(), arr.end());
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
    EXPECT_EQ(arr[3], 4);
    EXPECT_EQ(arr[4], 5);

    auto it = std::find(arr.begin(), arr.end(), 3);
    EXPECT_NE(it, arr.end());
    EXPECT_EQ(*it, 3);

    Array<int, 5> squared;
    std::transform(arr.begin(), arr.end(), squared.begin(),
        [](int x) { return x * x; });
    EXPECT_EQ(squared[0], 1);
    EXPECT_EQ(squared[4], 25);

    int sum = std::accumulate(arr.begin(), arr.end(), 0);
    EXPECT_EQ(sum, 15);

    auto min = std::min_element(arr.begin(), arr.end());
    auto max = std::max_element(arr.begin(), arr.end());
    EXPECT_EQ(*min, 1);
    EXPECT_EQ(*max, 5);
}

TEST(ArrayTest, DifferentTypes) {
    Array<int, 3> intArr = {1, 2, 3};
    EXPECT_EQ(intArr.Size(), 3);

    Array<double, 3> doubleArr = {1.1, 2.2, 3.3};
    EXPECT_DOUBLE_EQ(doubleArr[1], 2.2);

    Array<std::string, 3> strArr = {"hello", "world", "!"};
    EXPECT_EQ(strArr[0], "hello");
    EXPECT_EQ(strArr[2], "!");

    Array<bool, 3> boolArr = {true, false, true};
    EXPECT_TRUE(boolArr[0]);
    EXPECT_FALSE(boolArr[1]);
    EXPECT_TRUE(boolArr[2]);
}

TEST(ArrayTest, ManyOperations) {
    constexpr int iterations = 10000;

    for (int i = 0; i < iterations; ++i) {
        Array<int, 10> arr;
        for (int j = 0; j < 10; ++j) {
            arr[j] = i * j;
        }

        for (int j = 0; j < 10; ++j) {
            EXPECT_EQ(arr[j], i * j);
        }

        Array<int, 10> arr2 = arr;
        for (int j = 0; j < 10; ++j) {
            EXPECT_EQ(arr2[j], i * j);
        }

        Array<int, 10> arr3;
        arr3.Fill(0);
        swap(arr2, arr3);
        EXPECT_EQ(arr3[0], i * 0);
    }
}

TEST(ArrayTest, SizeTest) {
    static_assert(sizeof(Array<int, 10>) == sizeof(int) * 10,
        "Array should not have any additional fields");
    static_assert(sizeof(Array<double, 5>) == sizeof(double) * 5,
        "Array should not have any additional fields");
}