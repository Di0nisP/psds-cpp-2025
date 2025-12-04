#include <iostream>


void reinterpret() {
    std::cout << __func__ << ':' << std::endl;
    int number = 0x12345678;
    int* ptr_int = &number;
    char* ptr_char = reinterpret_cast<char*>(ptr_int);

    std::cout << "original int = " << std::hex << number << std::endl;
    std::cout << "reinterpret as bytes = ";
    for (int i = 0; i < sizeof(int); ++i) {
        std::cout << std::hex << static_cast<int>(ptr_char[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

int main() {
    reinterpret();
}