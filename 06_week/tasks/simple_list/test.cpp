#include <gtest/gtest.h>

#include "simple_list.cpp"

TEST(SimpleListTest, DefaultConstructor) {
    SimpleList list;
    EXPECT_EQ(list.Size(), 0);
    EXPECT_TRUE(list.Empty());
}

TEST(SimpleListTest, CopyConstructor) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");
    list1.PushBack("three");

    SimpleList list2(list1);

    EXPECT_EQ(list2.Size(), 3);
    EXPECT_FALSE(list2.Empty());
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "three");
}

TEST(SimpleListTest, CopyIndependency) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");
    list1.PushBack("three");

    SimpleList list2(list1);

    EXPECT_EQ(list2.Size(), 3);
    EXPECT_FALSE(list2.Empty());
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "three");

    list2.PopFront();
    EXPECT_EQ(list2.Front(), "two");
    EXPECT_EQ(list1.Front(), "one");
}

TEST(SimpleListTest, MoveConstructor) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    SimpleList list2(std::move(list1));

    EXPECT_EQ(list2.Size(), 2);
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "two");

    EXPECT_EQ(list1.Size(), 0);
    EXPECT_TRUE(list1.Empty());
}

TEST(SimpleListTest, UseAfterMove) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    SimpleList list2(std::move(list1));

    EXPECT_EQ(list2.Size(), 2);
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "two");

    EXPECT_EQ(list1.Size(), 0);
    EXPECT_TRUE(list1.Empty());

    list1.PushBack("one");
    EXPECT_EQ(list1.Front(), "one");
    EXPECT_EQ(list1.Back(), "one");
}

TEST(SimpleListTest, CopyAssignment) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    SimpleList list2;
    list2.PushBack("three");

    list2 = list1;

    EXPECT_EQ(list2.Size(), 2);
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "two");


    list2.PushBack("four");
    EXPECT_EQ(list2.Size(), 3);
    EXPECT_EQ(list2.Back(), "four");
    EXPECT_EQ(list1.Size(), 2);
    EXPECT_EQ(list1.Back(), "two");
}

TEST(SimpleListTest, CopyAssignmentIndependency) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    list2 = list1;

    EXPECT_EQ(list2.Size(), 2);
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "two");

    list2.PushBack("third");
    EXPECT_EQ(list2.Size(), 3);
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "third");
    EXPECT_EQ(list1.Size(), 2);
    EXPECT_EQ(list1.Front(), "one");
    EXPECT_EQ(list1.Back(), "two");
}

TEST(SimpleListTest, MoveAssignment) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    SimpleList list2;
    list2.PushBack("three");

    list2 = std::move(list1);

    EXPECT_EQ(list2.Size(), 2);
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "two");

    EXPECT_EQ(list1.Size(), 0);
    EXPECT_TRUE(list1.Empty());
}

TEST(SimpleListTest, SelfAssignment) {
    SimpleList list;
    list.PushBack("one");
    list.PushBack("two");

    list = list;

    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front(), "one");
    EXPECT_EQ(list.Back(), "two");
}

TEST(SimpleListTest, SelfMoveAssignment) {
    SimpleList list;
    list.PushBack("one");
    list.PushBack("two");

    list = static_cast<SimpleList&&>(list);

    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front(), "one");
    EXPECT_EQ(list.Back(), "two");
}

TEST(SimpleListTest, FrontBack) {
    SimpleList list;
    list.PushBack("first");
    list.PushBack("second");
    list.PushBack("third");

    EXPECT_EQ(list.Front(), "first");
    EXPECT_EQ(list.Back(), "third");

    const SimpleList& const_list = list;
    EXPECT_EQ(const_list.Front(), "first");
    EXPECT_EQ(const_list.Back(), "third");

    list.Front() = "new_first";
    list.Back() = "new_third";

    EXPECT_EQ(list.Front(), "new_first");
    EXPECT_EQ(list.Back(), "new_third");
    EXPECT_EQ(const_list.Front(), "new_first");
    EXPECT_EQ(const_list.Back(), "new_third");
}

TEST(SimpleListTest, PushBack) {
    SimpleList list;

    list.PushBack("one");
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Back(), "one");
    EXPECT_EQ(list.Front(), "one");

    list.PushBack("two");
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Back(), "two");
    EXPECT_EQ(list.Front(), "one");

    std::string str = "threeLongStringToAvoidSSO";
    const char* original_data = str.data();
    std::string original_value = str;
    list.PushBack(std::move(str));
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list.Back(), original_value);
    EXPECT_EQ(list.Front(), "one");

    if (original_value.size() > 16) {
        const char* list_data = list.Back().data();
        EXPECT_EQ(list_data, original_data);
    }

    list.PushBack("four");
    EXPECT_EQ(list.Back(), "four");
    EXPECT_EQ(list.Front(), "one");
}

TEST(SimpleListTest, PushFront) {
    SimpleList list;

    list.PushFront("one");
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front(), "one");
    EXPECT_EQ(list.Back(), "one");

    list.PushFront("two");
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front(), "two");
    EXPECT_EQ(list.Back(), "one");

    std::string str = "threeLongStringToAvoidSSO";
    const char* original_data = str.data();
    std::string original_value = str;
    list.PushFront(std::move(str));
    EXPECT_TRUE(str.empty());
    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list.Front(), original_value);
    EXPECT_EQ(list.Back(), "one");

    if (original_value.size() > 16) {
        const char* list_data = list.Back().data();
        EXPECT_EQ(list_data, original_data);
    }

    list.PushFront("four");
    EXPECT_EQ(list.Front(), "four");
    EXPECT_EQ(list.Back(), "one");
}

TEST(SimpleListTest, PopBack) {
    SimpleList list;
    list.PushBack("one");
    list.PushBack("two");
    list.PushBack("three");

    list.PopBack();
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Back(), "two");

    list.PopBack();
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Back(), "one");

    list.PopBack();
    EXPECT_EQ(list.Size(), 0);
    EXPECT_TRUE(list.Empty());

    list.PopBack();
    EXPECT_TRUE(list.Empty());
}

TEST(SimpleListTest, PopFront) {
    SimpleList list;
    list.PushBack("one");
    list.PushBack("two");
    list.PushBack("three");

    list.PopFront();
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front(), "two");

    list.PopFront();
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front(), "three");

    list.PopFront();
    EXPECT_EQ(list.Size(), 0);
    EXPECT_TRUE(list.Empty());

    list.PopFront();
    EXPECT_TRUE(list.Empty());
}

TEST(SimpleListTest, PushPopMix) {
    SimpleList list;

    list.PushBack("a");
    list.PushFront("b");
    list.PushBack("c");

    EXPECT_EQ(list.Front(), "b");
    EXPECT_EQ(list.Back(), "c");
    EXPECT_EQ(list.Size(), 3);

    list.PopFront();
    EXPECT_EQ(list.Front(), "a");
    EXPECT_EQ(list.Back(), "c");

    list.PopBack();
    EXPECT_EQ(list.Front(), "a");
    EXPECT_EQ(list.Back(), "a");

    list.PushFront("d");
    EXPECT_EQ(list.Front(), "d");
    EXPECT_EQ(list.Back(), "a");
}

TEST(SimpleListTest, Clear) {
    SimpleList list;
    list.PushBack("one");
    list.PushBack("two");
    list.PushBack("three");

    EXPECT_FALSE(list.Empty());
    EXPECT_EQ(list.Size(), 3);

    list.Clear();
    EXPECT_TRUE(list.Empty());
    EXPECT_EQ(list.Size(), 0);

    list.Clear();
    EXPECT_TRUE(list.Empty());

    list.PushBack("new");
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front(), "new");
    EXPECT_EQ(list.Back(), "new");

    list.PushFront("another");
    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front(), "another");
    EXPECT_EQ(list.Back(), "new");
}

TEST(SimpleListTest, Swap) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    std::string* ptr1_first = &list1.Front();
    std::string* ptr1_last = &list1.Back();

    SimpleList list2;
    list2.PushBack("three");
    list2.PushBack("four");
    list2.PushBack("five");

    std::string* ptr2_first = &list2.Front();
    std::string* ptr2_last = &list2.Back();

    list1.Swap(list2);

    EXPECT_EQ(list1.Size(), 3);
    EXPECT_EQ(list2.Size(), 2);

    EXPECT_EQ(list1.Front(), "three");
    EXPECT_EQ(list1.Back(), "five");
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "two");


    EXPECT_EQ(&list1.Front(), ptr2_first);
    EXPECT_EQ(&list1.Back(), ptr2_last);
    EXPECT_EQ(&list2.Front(), ptr1_first);
    EXPECT_EQ(&list2.Back(), ptr1_last);

    EXPECT_EQ(*ptr1_first, "one");
    EXPECT_EQ(*ptr2_first, "three");
}

TEST(SimpleListTest, SwapWithEmpty) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    std::string* ptr1_first = &list1.Front();
    std::string* ptr1_last = &list1.Back();

    SimpleList list2;

    list1.Swap(list2);

    EXPECT_EQ(list1.Size(), 0);
    EXPECT_TRUE(list1.Empty());

    EXPECT_EQ(list2.Size(), 2);
    EXPECT_EQ(list2.Front(), "one");
    EXPECT_EQ(list2.Back(), "two");

    EXPECT_EQ(&list2.Front(), ptr1_first);
    EXPECT_EQ(&list2.Back(), ptr1_last);

    EXPECT_EQ(*ptr1_first, "one");
    EXPECT_EQ(*ptr1_last, "two");
}

TEST(SimpleListTest, ExternalSwap) {
    SimpleList list1;
    list1.PushBack("one");
    std::string* ptr1 = &list1.Front();

    SimpleList list2;
    list2.PushBack("two");
    list2.PushBack("three");
    std::string* ptr2_first = &list2.Front();
    std::string* ptr2_last = &list2.Back();

    Swap(list1, list2);

    EXPECT_EQ(list1.Size(), 2);
    EXPECT_EQ(list2.Size(), 1);

    EXPECT_EQ(list1.Front(), "two");
    EXPECT_EQ(list1.Back(), "three");
    EXPECT_EQ(list2.Front(), "one");

    EXPECT_EQ(&list1.Front(), ptr2_first);
    EXPECT_EQ(&list1.Back(), ptr2_last);
    EXPECT_EQ(&list2.Front(), ptr1);
}

TEST(SimpleListTest, SwapDoesNotCopy) {
    SimpleList list1;
    list1.PushBack("one");
    list1.PushBack("two");

    SimpleList list2;
    list2.PushBack("three");
    list2.PushBack("four");

    std::string* list1_first = &list1.Front();
    std::string* list1_last = &list1.Back();
    std::string* list2_first = &list2.Front();
    std::string* list2_last = &list2.Back();

    list1.Swap(list2);

    bool same_objects = (
        (&list1.Front() == list2_first || &list1.Front() == list2_first) &&
        (&list1.Back() == list2_last || &list1.Back() == list2_last) &&
        (&list2.Front() == list1_first || &list2.Front() == list1_first) &&
        (&list2.Back() == list1_last || &list2.Back() == list1_last)
    );

    EXPECT_TRUE(same_objects);

    EXPECT_EQ(&list1.Front(), list2_first);
    EXPECT_EQ(&list1.Back(), list2_last);
    EXPECT_EQ(&list2.Front(), list1_first);
    EXPECT_EQ(&list2.Back(), list1_last);
}

TEST(SimpleListTest, SelfSwap) {
    SimpleList list;
    list.PushBack("one");
    list.PushBack("two");

    std::string* first = &list.Front();
    std::string* last = &list.Back();

    list.Swap(list);

    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front(), "one");
    EXPECT_EQ(list.Back(), "two");

    EXPECT_EQ(&list.Front(), first);
    EXPECT_EQ(&list.Back(), last);
}

TEST(SimpleListTest, ReferenceStability) {
    SimpleList list;

    list.PushBack("first");
    std::string& first_ref = list.Front();

    list.PushBack("second");
    std::string& second_ref = list.Back();

    list.PushBack("third");
    std::string& third_ref = list.Back();

    // Check that reference is valid
    EXPECT_EQ(first_ref, "first");
    EXPECT_EQ(second_ref, "second");
    EXPECT_EQ(third_ref, "third");

    first_ref = "modified_first";
    second_ref = "modified_second";
    third_ref = "modified_third";

    EXPECT_EQ(list.Front(), "modified_first");
    EXPECT_EQ(list.Back(), "modified_third");

    list.PushFront("new_front");
    EXPECT_EQ(first_ref, "modified_first");
    EXPECT_EQ(second_ref, "modified_second");
    EXPECT_EQ(third_ref, "modified_third");

    list.PopBack();  // pop "modified_third"

    EXPECT_EQ(first_ref, "modified_first");
    EXPECT_EQ(second_ref, "modified_second");
}

TEST(SimpleListTest, MixedOperations) {
    SimpleList list;

    list.PushBack("b");
    list.PushFront("a");
    list.PushBack("c");

    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list.Front(), "a");
    EXPECT_EQ(list.Back(), "c");

    list.PopFront();
    EXPECT_EQ(list.Front(), "b");
    EXPECT_EQ(list.Back(), "c");

    list.PushFront("x");
    EXPECT_EQ(list.Front(), "x");
    EXPECT_EQ(list.Back(), "c");

    list.PopBack();
    EXPECT_EQ(list.Front(), "x");
    EXPECT_EQ(list.Back(), "b");

    EXPECT_EQ(list.Size(), 2);

    list.Clear();
    EXPECT_TRUE(list.Empty());

    list.PushBack("new");
    EXPECT_EQ(list.Front(), "new");
    EXPECT_EQ(list.Back(), "new");
}

TEST(SimpleListTest, LargeList) {
    SimpleList list;
    const size_t COUNT = 1000;

    for (size_t i = 0; i < N; ++i) {
        list.PushBack(std::to_string(i));
    }

    EXPECT_EQ(list.Size(), COUNT);
    EXPECT_EQ(list.Front(), "0");
    EXPECT_EQ(list.Back(), std::to_string(COUNT - 1));

    for (size_t i = 0; i < COUNT; ++i) {
        EXPECT_EQ(list.Front(), std::to_string(i));
        list.PopFront();
    }
    EXPECT_TRUE(list.Empty());
}

TEST(SimpleListTest, NoMemoryLeaksAsan) {
    SimpleList* list = new SimpleList();

    for (int i = 0; i < 10; ++i) {
        list->PushBack("test");
    }
    delete list;

    {
        SimpleList list2;
        list2.PushBack("one");
        list2.PushBack("two");
        list2.PushBack("three");
    }
}

TEST(SimpleListTest, EmptyListOperations) {
    SimpleList list;

    EXPECT_TRUE(list.Empty());
    EXPECT_EQ(list.Size(), 0);

    list.PopFront();
    list.PopBack();
    list.Clear();

    EXPECT_TRUE(list.Empty());
    EXPECT_EQ(list.Size(), 0);

    list.PushBack("test");
    EXPECT_FALSE(list.Empty());
    EXPECT_EQ(list.Size(), 1);
}

TEST(SimpleListTest, RvalueReferences) {
    SimpleList list;

    list.PushBack(std::string("temporary1"));
    list.PushFront(std::string("temporary2"));

    EXPECT_EQ(list.Size(), 2);
    EXPECT_EQ(list.Front(), "temporary2");
    EXPECT_EQ(list.Back(), "temporary1");

    std::string str = "moved";
    list.PushBack(std::move(str));

    EXPECT_EQ(list.Size(), 3);
    EXPECT_EQ(list.Back(), "moved");
}

TEST(SimpleListTest, ConstCorrectness) {
    SimpleList list;
    list.PushBack("one");
    list.PushBack("two");

    const SimpleList& const_list = list;

    EXPECT_EQ(const_list.Size(), 2);
    EXPECT_FALSE(const_list.Empty());
    EXPECT_EQ(const_list.Front(), "one");
    EXPECT_EQ(const_list.Back(), "two");

    // const_list.PushBack("three"); // must be CE
    // const_list.PopFront(); // must be CE
    // const_list.Front() = "new"; // must be CE
}

TEST(SimpleListTest, SingleElement) {
    SimpleList list;

    list.PushBack("only");
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front(), "only");
    EXPECT_EQ(list.Back(), "only");

    list.PopFront();
    EXPECT_TRUE(list.Empty());

    list.PushFront("again");
    EXPECT_EQ(list.Size(), 1);
    EXPECT_EQ(list.Front(), "again");
    EXPECT_EQ(list.Back(), "again");

    list.PopBack();
    EXPECT_TRUE(list.Empty());
}

TEST(SimpleListTest, CopyAfterModifications) {
    SimpleList list1;
    list1.PushBack("a");
    list1.PushBack("b");

    SimpleList list2(list1);

    list1.PushBack("c");
    list1.PopFront();

    EXPECT_EQ(list1.Size(), 2);
    EXPECT_EQ(list1.Front(), "b");
    EXPECT_EQ(list1.Back(), "c");

    EXPECT_EQ(list2.Size(), 2);
    EXPECT_EQ(list2.Front(), "a");
    EXPECT_EQ(list2.Back(), "b");
}