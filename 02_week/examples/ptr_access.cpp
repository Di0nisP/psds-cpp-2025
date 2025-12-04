#include <iostream>


void ptr_access() {
    std::cout << __func__ << ':' << std::endl;
    int abc = 0xBADC0FEE;
    std::cout << abc << std::endl;
    int* ptr_abc = &abc;
    std::cout << "address-of abc = " << &abc << std::endl;
    std::cout << "ptr_abc = " << ptr_abc << std::endl;
    std::cout << "access using [0] = " << ptr_abc[0] << std::endl;
    std::cout << "access using * = " << *ptr_abc << std::endl;
    std::cout << "address-of ptr_abc = " << &ptr_abc << std::endl;
    std::cout << std::endl;
}

int main() {
    ptr_access();
}