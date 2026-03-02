#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

template <typename T, typename... Args>
concept CtorTConstraints = 
    (sizeof...(Args) == 0 && std::default_initializable<T>) ||
    (sizeof...(Args) > 0  && std::constructible_from<T, Args...>);

template <typename T, typename... Args>
concept MakeUniqueConstraints = 
    !std::is_array_v<T> && // Без массивов
    CtorTConstraints<T, Args...>;

template <typename T, typename... Args>
requires MakeUniqueConstraints<T, Args...>
std::unique_ptr<T> MakeUnique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}   
