

const int* FindLastElement(const int* cbegin, const int* cend, bool (*predicate)(int)) {
    if (cbegin == nullptr || cend == nullptr || cbegin >= cend || predicate == nullptr) {
        return cend;
    }

    auto rcbegin = cend;
    while (rcbegin != cbegin) {
        if (predicate(*(--rcbegin))) {
            return rcbegin;
        }
    }

    return cend;
}