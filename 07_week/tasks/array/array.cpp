#include <algorithm>
#include <cstddef> // size_t
#include <utility> // std::forward, std::move
#include <concepts> // std::default_initializable

/**
 * @brief Ограничение для типов, 
 * которые могут быть использованы в Array.
 * 
 * Требует, чтобы T мог быть сконструирован по умолчанию.
 *
 * @tparam T Тип элементов массива
 * @tparam N Размер массива
 */
template <typename T, std::size_t N>
concept ArrayConstraints = std::default_initializable<T>; 
// иначе всё равно будет UB при попытке создать массив из неинициализируемых объектов

template <typename T, std::size_t N>
requires ArrayConstraints<T, N>
struct Array 
{
    Array() = default;
    
    /**
     * @brief Конструктор, который позволяет инициализировать массив 
     * с помощью списка аргументов.
     * 
     * @tparam U Типы аргументов конструктора, 
     * которые должны быть конвертируемы в T
     * @param values Аргументы для инициализации элементов массива
     */
    template <typename... U>
    requires (sizeof...(U) <= N) && (std::convertible_to<U, T> && ...)
    Array(U&&... values);
    //Array(std::initializer_list<T> init);

    /// @name Copy and Move Semantics
    /// @{
    Array(const Array& values) = default;
    Array& operator=(const Array& other) = default;

    Array(Array&& values) noexcept = default;
    Array& operator=(Array&& other) noexcept = default;
    /// @}

    ~Array() = default;

    /// @name Data Access
    /// @{
    constexpr T* Data() noexcept { return data_; }
    constexpr const T* Data() const noexcept { return data_; }
    /// @}

    /// @name Element Access
    /// @{
    constexpr T& operator[](size_t index) noexcept { return Data()[index]; }
    constexpr const T& operator[](size_t index) const noexcept { return Data()[index]; }

    constexpr T& Front() noexcept { return Data()[0]; }
    constexpr const T& Front() const noexcept { return Data()[0]; }

    constexpr T& Back() noexcept { return Data()[N - 1]; }
    constexpr const T& Back() const noexcept { return Data()[N - 1]; }

    constexpr T* begin() noexcept { return Data(); }
    constexpr const T* begin() const noexcept { return Data(); }
    constexpr const T* cbegin() const noexcept { return Data(); }

    constexpr T* end() noexcept { return Data() + Size(); }
    constexpr const T* end() const noexcept { return Data() + Size(); }
    constexpr const T* cend() const noexcept { return Data() + Size(); }
    /// @}

    /// @name Capacity
    /// @{
    constexpr std::size_t Size() const noexcept { return N; }
    constexpr std::size_t Empty() const noexcept { return N == 0; }
    /// @}

    /// @name Modifiers
    /**
     * @note Гарантирует, 
     * что операция fill не выбросит исключений, 
     * если тип T поддерживает noexcept copy assignment (копирующее присваивание).
     *
     * @note Функция существует только для типов T, 
     * которые концептуально copyable (копируемые).
     */
    void Fill(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>)
    requires std::copyable<T>;

    /**
     * @note Гарантирует, 
     * что операция swap не выбросит исключений, 
     * если тип T поддерживает noexcept swap.
     *
     * @note Функция существует только для типов T, 
     * которые концептуально swappable (обмениваемые).
     */
    void Swap(Array& other) noexcept(std::is_nothrow_swappable_v<T>)
    requires std::swappable<T>;
    /// @}

    /// @name Comparison Operators
    /// @{
    auto operator<=>(const Array& other) const = default;
    /// @}

private:
    T data_[N]{}; 
};

// Или "костыль" `T data_[N == 0 ? 1 : N]{};`
template <typename T>
struct Array<T, 0> 
{
    constexpr std::size_t Size() const noexcept { return 0; }
    constexpr bool Empty() const noexcept { return true; }

    constexpr T* Data() noexcept { return nullptr; }
    constexpr const T* Data() const noexcept { return nullptr; }

    // Element Access для N==0 по контракту UB, обычно не вызываются
};

template <typename T, std::size_t N>
void swap(Array<T, N>& a, Array<T, N>& b) noexcept(noexcept(a.Swap(b))) 
{
    a.Swap(b);
}

template <std::size_t I,typename T, std::size_t N>
requires (I < N)
constexpr T& get(Array<T, N>& arr) noexcept(noexcept(arr[I]))
{
    return arr[I];
}

template <std::size_t I,typename T, std::size_t N>
requires (I < N)
constexpr const T& get(const Array<T, N>& arr) noexcept(noexcept(arr[I]))
{
    return arr[I];
}

template <std::size_t I,typename T, std::size_t N>
requires (I < N)
constexpr T&& get(Array<T, N>&& arr) noexcept(noexcept(arr[I]))
{
    return std::move(arr[I]);
}

// ---------------------------------------------------------------------------
// Array implementation
// ---------------------------------------------------------------------------

/*
template <typename T, std::size_t N>
requires std::default_initializable<T>
Array<T, N>::Array(std::initializer_list<T> init) 
{
    size_t i = 0;
    for (const auto& elem : init) {
        if (i < N) {
            data_[i++] = elem;
        } else {
            break;
        }
    }
} //*/

template <typename T, std::size_t N>
requires ArrayConstraints<T, N>
template <typename... U>
requires (sizeof...(U) <= N) && (std::convertible_to<U, T> && ...)
Array<T, N>::Array(U&&... values)
    : data_{static_cast<T>(std::forward<U>(values))...} 
{

}

/*
template <typename T, std::size_t N>
requires std::default_initializable<T>
Array<T, N>::Array(const Array<T, N>& values)
{
    std::copy_n(values.data_, N, data_);
}

template <typename T, std::size_t N>
requires std::default_initializable<T>
Array<T, N>& Array<T, N>::operator=(const Array<T, N>& other) 
{
    if (this != &other)
        std::copy_n(other.data_, N, data_);

    return *this;
}

template <typename T, std::size_t N>
requires std::default_initializable<T>
Array<T, N>::Array(Array<T, N>&& values) noexcept
    : data_{std::exchange(values.data_, data_)} 
{

}

template <typename T, std::size_t N>
requires std::default_initializable<T>
Array<T, N>& Array<T, N>::operator=(Array<T, N>&& other) noexcept
{
    if (this != &other)
        data_ = std::exchange(other.data_, data_);

    return *this;
} //*/

template <typename T, std::size_t N>
requires ArrayConstraints<T, N>
void Array<T, N>::Fill(const T& value) noexcept(std::is_nothrow_copy_assignable_v<T>) 
requires std::copyable<T>
{
    std::fill_n(Data(), N, value);
}

template <typename T, std::size_t N>
requires ArrayConstraints<T, N>
void Array<T, N>::Swap(Array& other) noexcept(std::is_nothrow_swappable_v<T>)
requires std::swappable<T>
{
    std::swap_ranges(Data(), Data() + Size(), other.Data());
}