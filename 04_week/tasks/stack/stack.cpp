#include <vector>


class Stack {
    using StackType = std::vector<int>;

public:
    Stack() = default;

    ~Stack() = default;

    /// @name Copy and move semantics
    /// @{
    Stack(const Stack& other) = default;
    Stack& operator=(const Stack& other) = default;
    Stack(Stack&& other) noexcept = default;
    Stack& operator=(Stack&& other) noexcept = default;
    /// @}

    auto Empty() const noexcept {
        return stack_.empty();
    }
    auto Size() const noexcept {
        return stack_.size();
    }

    /// @name Modifiers
    /// @{
    void Push(int value) noexcept {
        stack_.push_back(value);
    }
    bool Pop() noexcept {
        if (stack_.empty())
            return false;
        stack_.pop_back();
        return true;
    }

    void Swap(Stack& other) noexcept {
        std::swap(stack_, other.stack_);
    }

    void Clear() noexcept {
        stack_.clear();
    }
    /// @}

    auto Top() noexcept         -> int&;
    auto Top() const noexcept   -> const int&;

    bool operator==(const Stack& other) const noexcept;
    bool operator!=(const Stack& other) const noexcept;

private:
    template <typename Self>
    static auto& TopImpl(Self& self) noexcept;

private:
    StackType stack_ {};  ///< Вектор для хранения элементов стека
};

template <typename Self>
auto& Stack::TopImpl(Self& self) noexcept {
    return self.stack_.back();
}

auto Stack::Top() noexcept -> int& {
    return TopImpl(*this);
}
auto Stack::Top() const noexcept -> const int& {
    return TopImpl(*this);
}

bool Stack::operator==(const Stack& other) const noexcept {
    return stack_ == other.stack_;
}
bool Stack::operator!=(const Stack& other) const noexcept {
    return !(*this == other);
}