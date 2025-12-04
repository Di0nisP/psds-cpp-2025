#include <iostream>

// compile with clang++ and g++
void ptr_address() {
    std::cout << __func__ << ':' << std::endl;
    int x = 0;
    int y = 1;
    std::cout << "&x: " << &x << '\n' << "&y: " << &y << std::endl;

    int* ptr_x = &x;
    std::cout << "sizeof(int*): " << sizeof(ptr_x) << '\n'
              << "ptr_x: " << ptr_x << '\n'
              << "&ptr_x: " << &ptr_x << std::endl;

    int* ptr_y = &y;
    std::cout << "sizeof(int*): " << sizeof(ptr_y) << '\n'
              << "ptr_y: " << ptr_y << '\n'
              << "&ptr_y: " << &ptr_y << std::endl;

    int z = 2;
    std::cout << "&z: " << &z << std::endl;
    std::cout << std::endl;
}

void ptr_type() {
    std::cout << __func__ << ':' << std::endl;
    int x = 0;
    int* ptr_x = &x;
    std::cout << "type for int*: " << typeid(ptr_x).name() <<  std::endl;

    double y = 0;
    double* ptr_y = &y;
    std::cout << "type for double*: " << typeid(ptr_y).name() <<  std::endl;

    int** ptr_ptr_x = &ptr_x;
    std::cout << "type for int**: " << typeid(ptr_ptr_x).name() <<  std::endl;
    std::cout << std::endl;
}

int main() {
    ptr_address();
    ptr_type();
}