#include <bit>
#include <cstdint>
#include <iostream>
#include <utility>


enum class CheckFlags : uint8_t {
    NONE = 0,
    TIME = (1 << 0),
    DATE = (1 << 1),
    USER = (1 << 2),
    CERT = (1 << 3),
    KEYS = (1 << 4),
    DEST = (1 << 5),
    ALL = TIME | DATE | USER | CERT | KEYS | DEST
};

void PrintCheckFlags(CheckFlags flags) {  
    const auto value { static_cast<uint8_t>(flags) };
    const auto all { static_cast<uint8_t>(CheckFlags::ALL) };
    
    // Есть биты вне диапазона
    if ((value & ~all) != 0) {
        std::cout << "";
        return;
    }

    if (value == static_cast<uint8_t>(CheckFlags::NONE)) {
        std::cout << "[]";
        return;
    }

    std::string out {'['};
    bool first { true };
    for (uint8_t i = 0; std::cmp_less(i, std::popcount(all)); ++i) {
        if ((value & (1u << i)) != 0) {
            if (!first) out += ',';
            switch (i) {
            case 0: out += "TIME"; break;
            case 1: out += "DATE"; break;
            case 2: out += "USER"; break;
            case 3: out += "CERT"; break;
            case 4: out += "KEYS"; break;
            case 5: out += "DEST"; break;
            }
            first = false;
        }
    }
    out += ']';
    std::cout << out;
}
