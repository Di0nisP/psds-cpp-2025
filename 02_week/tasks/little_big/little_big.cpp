#include <iomanip>
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <memory>

using namespace std;

using byte_view_t = const unsigned char*;

static const bool LITTLE_ENDIAN_STATUS { []() {
    const uint16_t number { 0x1 };
    return *reinterpret_cast<byte_view_t>(addressof(number)) == 0x1;
}() };
static const bool INVERT_DEFAULT { !LITTLE_ENDIAN_STATUS };

template<typename T>
concept IntOrDouble = std::is_same_v<T, int> || std::is_same_v<T, double>;

template<IntOrDouble T>
void PrintMemory(const T value, const bool invert = INVERT_DEFAULT)
{
    const byte_view_t p { reinterpret_cast<byte_view_t>(addressof(value)) };
    const size_t n { sizeof(value) };

    // Сохраняем текущее состояние потока
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    
    std::cout << std::uppercase << std::hex << std::setfill('0');

    std::cout << "0x";
    if (!invert) {
        for (std::size_t i = 0; i < n; ++i) {
            std::cout << std::setw(2) << static_cast<unsigned int>(p[i]);
        }
    } else {
        for (std::size_t i = 0; i < n; ++i) {
            const std::size_t j { n - 1 - i };
            std::cout << std::setw(2) << static_cast<unsigned int>(p[j]);
        }
    }
    std::cout << "\n";

    // Восстанавливаем состояние потока
    std::cout.copyfmt(oldState);
}