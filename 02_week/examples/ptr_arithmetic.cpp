#include <iostream>


void ptr_arithmetic() {
    std::cout << __func__ << ':' << std::endl;
    int arr[5] = {1, 2, 3, 4, 5};
    int* begin = arr;   // pointer to 1
    std::cout << *begin << std::endl;
    std::cout << begin[1] << std::endl;
    std::cout << begin << std::endl;
    std::cout << ++begin << std::endl;
    std::cout << *(++begin) << std::endl;
    std::cout << begin[0] << std::endl;
    std::cout << *(begin + 2) << std::endl;
    int* end = arr + 5; // pointer to pos after end of array
    auto distance = end - begin;  // type is std::ptrdiff_t
    std::cout << distance << std::endl;
    std::cout << std::endl;
}

int main() {
    ptr_arithmetic();
}

