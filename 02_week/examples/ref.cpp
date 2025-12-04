#include <iostream>

void ref_changes() {
    std::cout << __func__ << ':' << std::endl;
    int x = 0;
    std::cout << "x = " << x << std::endl; // 0

    int& y = x;
    std::cout << "y = " << y << std::endl; // 0
    y = 1;
    std::cout << "after y = 1:" << std::endl;
    std::cout << "x = " << x << std::endl; // 1
    std::cout << "y = " << y << std::endl; // 1

    int z = 5;
    y = z; // change x, not change ref for y
    std::cout << "after y = z:" << std::endl;
    std::cout << "x = " << x << std::endl; // 5
    std::cout << "y = " << y << std::endl; // 5
    std::cout << std::endl;
}

int main() {
    ref_changes();
}