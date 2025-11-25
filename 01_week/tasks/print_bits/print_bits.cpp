#include <cstddef>
#include <stdexcept>
#include <iostream>

void PrintBits(long long value, size_t bytes) {
    using value_t = decltype(value);
    
    if (bytes == 0 || bytes > sizeof(value)) {
        throw std::invalid_argument("Invalid number of bytes");
    }

    const auto bits { bytes * 8 };
    
    std::string out { "0b" };
    for (size_t i = 0; i < bits; ++i) {
        const value_t mask { (static_cast<value_t>(1) << (bits - 1 - i)) };
        const bool bitset { (value & mask) != 0 };
        out += (bitset ? '1' : '0');
        if ((i + 1) % 4 == 0 && i + 1 < bits) {
            out += '\'';
        }
    }
    std::cout << out << std::endl;

}
