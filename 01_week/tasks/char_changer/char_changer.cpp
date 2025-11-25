#include <cctype>
#include <cstddef>
#include <stdexcept>


size_t CharChanger(char array[], size_t size, char delimiter = ' ') {
    if (!array || size == 0) {
        throw std::invalid_argument{"Empty array"};
    }

    size_t src {}; // индекс чтения
    size_t dst {}; // индекс записи

    while (array[src] != '\0' && src < size) {
        char c { array[src++] }; // текущий символ

        // Подсчет количества одинаковых символов
        size_t count {1};
        while (array[src] != '\0' && src < size) {
            const char next { array[src] };
            if (c != next) 
                break;
            count++;
            src++;
        }

        // Изменение символа в соответствии с правилами
        bool isspace {};
        if (std::isdigit(static_cast<unsigned char>(c))) {
            c = '*';
        } else if (std::islower(static_cast<unsigned char>(c))) {
            c = std::toupper(static_cast<unsigned char>(c));
        } else if (std::isupper(static_cast<unsigned char>(c))) {
            // без изменений
        } else if (std::isspace(static_cast<unsigned char>(c))) {
            c = delimiter;
            isspace = true;
        } else {
            c = '_';
        }

        // Запись измененного символа и количества в массив
        array[dst++] = c;
        if (count > 1 && !isspace) {
            if (count >= 10) {
                array[dst++] = '0';
            } else {
                array[dst++] = static_cast<char>('0' + count);
            }
        }
    }

    return (array[dst] = '\0', dst); // позиция конца строки
}
