#include <iostream>

void _change_const_value(const int& value) {
    int& mutable_value = const_cast<int&>(value);
    mutable_value = 100;
}

void change_const_value() {
    std::cout << __func__ << ':' << std::endl;
    int value = 42;
    std::cout << "before const_cast: " << value << std::endl;
    _change_const_value(value);
    std::cout << "after const_cast: " << value << std::endl;
    std::cout << std::endl;

    std::cout << "UB: " << std::endl;
    const int cvalue = 42;
    std::cout << "before const_cast: " << cvalue << std::endl;
    _change_const_value(cvalue); // UB
    std::cout << "after const_cast: " << cvalue << std::endl;
    std::cout << std::endl;
}

void dangerous_example_ref() {
    std::cout << __func__ << ':' << std::endl;
    const int truly_const = 42;
    std::cout << "before const_cast: " << truly_const << std::endl;

    int& ref = const_cast<int&>(truly_const); // UB
    ref = 100;

    std::cout << "after const_cast: " << truly_const << std::endl; // maybe 42
    std::cout << "ref: " << ref << std::endl;                      // maybe 100

    std::cout << "&truly_const: " << &truly_const << std::endl;
    std::cout << "&ref: " << &ref << std::endl;
    std::cout << std::endl;
}

void const_cast_pointers() {
    std::cout << __func__ << ':' << std::endl;
    int value = 100;
    const int* const_ptr = &value;
    std::cout << "value before modification: " << value << std::endl;

    int* mutable_ptr = const_cast<int*>(const_ptr); // const_cast
    *mutable_ptr = 200;
    std::cout << "value after modification: " << value << std::endl;
    std::cout << std::endl;

    const int* new_const_ptr = const_cast<const int*>(mutable_ptr);
    // *new_const_ptr = 300; // compile error

    std::cout << "UB: " << std::endl;
    const int cvalue = 100;
    std::cout << "value before modification: " << cvalue << std::endl;
    const int* ptr = &cvalue;
    int* cmutable_ptr = const_cast<int*>(ptr);
    *cmutable_ptr = 200;
    std::cout << "value after modification: " << cvalue << std::endl;
    std::cout << std::endl;
}

void dangerous_example_ptr() {
    std::cout << __func__ << ':' << std::endl;
    const int cvalue = 100;
    const int* ptr = &cvalue;
    int* mutable_ptr = const_cast<int*>(ptr);
    *mutable_ptr = 200;

    std::cout << "value before modification: " << cvalue << std::endl;
    std::cout << "value after modification: " << cvalue << std::endl;
    std::cout << "ptr: " << ptr << std::endl;
    std::cout << "mutable_ptr: " << mutable_ptr << std::endl;
    std::cout << std::endl;
}

int main() {
    change_const_value();
    dangerous_example_ref();
    const_cast_pointers();
    dangerous_example_ptr();
}