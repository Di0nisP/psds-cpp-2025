#include <iostream>
#include <iomanip>
#include <cmath>


void SolveQuadratic(int a, int b, int c) {
    constexpr const int precision { 6 };
    
    // Сохраняем текущее состояние потока
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    
    std::cout << std::setprecision(precision);
    
    if (a == 0) {
        if (b == 0) {
            if (c == 0) {
                std::cout << "infinite solutions";
            } else {
                std::cout << "no solutions";
            }
        } else {
            std::cout << -static_cast<double>(c) / b;
        }
    } else {
        const int d { b * b - 4 * a * c };
        if (d < 0) {
            std::cout << "no solutions";
        } else if (d == 0) {
            if (b == 0) {
                std::cout << "0";
            } else {
                std::cout << -static_cast<double>(b) / (2 * a);
            }
        } else {
            const double sqrt_d { std::sqrt(d) };
            std::cout << (-b - sqrt_d) / (2 * a) << ' ' << (-b + sqrt_d) / (2 * a);
        }
    }

    // Восстанавливаем состояние потока
    std::cout.copyfmt(oldState);
}