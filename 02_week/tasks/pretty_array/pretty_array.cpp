#include <iterator>
#include <iostream>
#include <cstddef>

static constexpr size_t NO_LIMIT = 0;
static constexpr const size_t LIMIT_DEAFULT {NO_LIMIT};

void PrintArray(const int* cbegin, const int* cend, size_t limit = LIMIT_DEAFULT) {
    if (cbegin == nullptr || cend == nullptr) {
        std::cout << "[]\n";
        return;
    }
    
    // Сохраняем текущее состояние потока
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);

    std::cout.flags(std::ios::fmtflags(0));  // сброс всех флагов
    std::cout << std::dec;                         // десятичный вывод
    std::cout.width(0);                      // ширина по умолчанию
    std::cout.fill(' ');                       // пробел как fill

    const std::string separator { ", " };
    const std::string lineBreak { ", ...\n " };

    const auto total { static_cast<size_t>(cbegin < cend ? std::distance(cbegin, cend) : std::distance(cend, cbegin)) };

    auto PrintElement = [&](const size_t idx, const int* const p) {
        std::cout << *p;

        const auto num { idx + 1 };

        if (num < total) {
            if (limit != NO_LIMIT && num % limit == 0) 
            {
                std::cout << lineBreak;
            } else {
                std::cout << separator;
            }
        }
    };

    const bool isReverse { cbegin > cend };

    std::cout << "[";
    const int* p { cbegin };
    if (isReverse) {
        for (size_t i = 0; p > cend; --p, ++i) {
            PrintElement(i, p);
        }
    } else {
        for (size_t i = 0; p < cend; ++p, ++i) {
            PrintElement(i, p);
        }
    }
    std::cout << "]\n";

    // Восстанавливаем состояние потока
    std::cout.copyfmt(oldState);
}