
template <typename T>
void SwapPtr(T*& pp1, T*& pp2) {
    auto temp { pp1 };
    pp1 = pp2;
    pp2 = temp;
}