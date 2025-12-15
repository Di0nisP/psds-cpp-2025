#include <cstddef> // size_t
#include <iterator> // std::next, std::prev

char* FindLongestSubsequence(const char* cbegin, const char* cend, size_t& count) {
    if (cbegin == nullptr || cend == nullptr || cbegin >= cend) {
        return (count = 0, nullptr);
    }

    const char* bestStart { cbegin };
    size_t bestLen { 1 };

    const char* currStart { cbegin };
    size_t currLen { 1 };

    auto CheckCurrentSequence = [&]() {
        if (currLen > bestLen) {
            bestLen = currLen;
            bestStart = currStart;
        }
    };

    for (const char* it { std::next(cbegin) }; it < cend; ++it) {
        if (*it == *(std::prev(it))) {
            // Продолжаем текущую последовательность
            ++currLen;
        } else {
            // Последовательность прервалась
            CheckCurrentSequence();
            // Начинаем новую последовательность
            currStart = it;
            currLen = 1;
        }
    }

    // Проверка последней последовательности
    CheckCurrentSequence();

    //? Тест MutableString на момент написания кода предполагает,
    //? что возвращается неконстантный указатель
    return (count = bestLen, const_cast<char*>(bestStart));
}
