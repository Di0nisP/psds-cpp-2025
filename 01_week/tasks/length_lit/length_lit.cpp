// Футы → метры
constexpr double operator""_ft_to_m(long double v) {
    return static_cast<double>(v * 0.3048);
}

// Футы → сантиметры
constexpr double operator""_ft_to_cm(long double v) {
    return static_cast<double>(v * 30.48);
}

// Футы → дюймы
constexpr double operator""_ft_to_in(long double v) {
    return static_cast<double>(v * 12.0);
}

// Дюймы → футы
constexpr double operator""_in_to_ft(long double v) {
    return static_cast<double>(v / 12.0);
}

// Дюймы → сантиметры
constexpr double operator""_in_to_cm(long double v) {
    return static_cast<double>(v * 2.54);
}

// Дюймы → метры
constexpr double operator""_in_to_m(long double v) {
    return static_cast<double>(v * 0.0254);
}

// Сантиметры → метры
constexpr double operator""_cm_to_m(long double v) {
    return static_cast<double>(v / 100.0);
}

// Сантиметры → дюймы
constexpr double operator""_cm_to_in(long double v) {
    return static_cast<double>(v / 2.54);
}

// Сантиметры → футы
constexpr double operator""_cm_to_ft(long double v) {
    return static_cast<double>(v / 30.48);
}

// Метры → сантиметры
constexpr double operator""_m_to_cm(long double v) {
    return static_cast<double>(v * 100.0);
}

// Метры → футы
constexpr double operator""_m_to_ft(long double v) {
    return static_cast<double>(v / 0.3048);
}

// Метры → дюймы
constexpr double operator""_m_to_in(long double v) {
    return static_cast<double>(v / 0.0254);
}