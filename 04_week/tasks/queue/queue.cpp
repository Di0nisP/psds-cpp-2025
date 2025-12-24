#include <vector>
#include <stack>
#include <algorithm>



class Queue {
public:
    Queue() = default;

    Queue(std::stack<int> s) {
        while (!s.empty()) {
            input_.push_back(s.top());
            s.pop();
        }
        std::reverse(input_.begin(), input_.end());
    }

    Queue(const std::vector<int>& vec) 
        : input_(vec)
    {}

    Queue(const std::initializer_list<int>& init) 
        : input_(init.begin(), init.end())
    {}

    Queue(std::size_t capacity) {
        input_.reserve(capacity);
    }

    ~Queue() = default;

    
    bool Empty() const noexcept {
        return Size() == 0;
    }
    auto Size() const noexcept -> std::size_t {
        return input_.size() + output_.size();
    }

    /// @name Modifiers
    /// @{
    void Push(const int value) noexcept {
        input_.push_back(value);
    }
    bool Pop() noexcept {
        if (output_.empty()) {
            if (input_.empty()) {   
                return false;
            }
            std::reverse(input_.begin(), input_.end());
            std::swap(input_, output_);
        }
        output_.pop_back();
        return true;
    }
    void Swap(Queue& other) noexcept {
        std::swap(input_, other.input_);
        std::swap(output_, other.output_);
    }
    void Clear() noexcept {
        input_.clear();
        output_.clear();
    }
    /// @}

    /// @name Front/Back
    /// @{
    auto Front()        -> int&;
    auto Front() const  -> const int&;
    auto Back()         -> int&;
    auto Back() const   -> const int&;
    /// @}

    /// @name Comparison operators
    /// @{
    bool operator==(const Queue& other) const noexcept;
    bool operator!=(const Queue& other) const noexcept {
        return !(*this == other);
    }
    /// @}

private:
    template <typename Self>
    static auto& FrontImpl(Self& self);
    template <typename Self>
    static auto& BackImpl(Self& self);

private:
    std::vector<int> input_ {};
    std::vector<int> output_{};
};

template<typename Self>
auto& Queue::FrontImpl(Self& self) {
    if (self.output_.empty()) {
        return self.input_.front();
    }
    return self.output_.back();
}
auto Queue::Front() -> int& {
    return FrontImpl(*this);
}
auto Queue::Front() const -> const int& {
    return FrontImpl(*this);
}


template <typename Self>
auto& Queue::BackImpl(Self& self) {
    if (self.input_.empty())
        return self.output_.front();
    return self.input_.back();
}
auto Queue::Back() -> int& {
    return BackImpl(*this);
}
auto Queue::Back() const -> const int& {
    return BackImpl(*this);
}

bool Queue::operator==(const Queue& other) const noexcept {
    if (Size() != other.Size()) {
        return false;
    }

    const auto sz { Size() };
    for (std::size_t i = 0; i < sz; ++i) {
        if (output_.size() > i && other.output_.size() > i) {
            if (      output_[      output_.size() - 1 - i] != 
                other.output_[other.output_.size() - 1 - i]) 
            {
                return false;
            }
        } else if (output_.size() > i) {
            if (output_[output_.size() - 1 - i] != other.input_[i - other.output_.size()]) {
                return false;
            }
        } else if (other.output_.size() > i) {
            if (input_[i - output_.size()] != other.output_[other.output_.size() - 1 - i]) {
                return false;
            }
        } else {
            if (      input_[i -       output_.size()] != 
                other.input_[i - other.output_.size()]) 
            {
                return false;
            }
        }
    }

    return true;
}