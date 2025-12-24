#include <cstddef>
#include <vector>

static constexpr const std::size_t MINIMAL_CAPACITY { 1 };

/// @brief Вспомогательная функция для обеспечения минимальной емкости буфера
static auto ValidCapacity(const std::size_t capacity) noexcept {
    return capacity >= MINIMAL_CAPACITY ? capacity : MINIMAL_CAPACITY;
}

class RingBuffer {
public:
    RingBuffer() = default;
    RingBuffer(const std::size_t capacity) {
        buffer_.reserve(ValidCapacity(capacity));
    }

    RingBuffer(const std::size_t capacity, int initial_value)
        : buffer_(ValidCapacity(capacity), initial_value)
    {}

    RingBuffer(const std::initializer_list<int>& init)
        : buffer_(init.begin(), init.end())
    {
        buffer_.reserve(ValidCapacity(init.size()));
    }

    ~RingBuffer() = default;

    /// @name Copy and move semantics
    /// @{
    RingBuffer(const RingBuffer& other);
    RingBuffer& operator=(const RingBuffer& other);
    
    RingBuffer(RingBuffer&& other) noexcept;
    RingBuffer& operator=(RingBuffer&& other) noexcept;
    /// @}

    /// @{
    auto Empty() const noexcept {
        return buffer_.empty();
    }
    auto Full() const noexcept {
        return buffer_.size() == buffer_.capacity();
    }
    auto Size() const noexcept -> std::size_t {
        return buffer_.size();
    }
    auto Capacity() const noexcept -> std::size_t {
        return buffer_.capacity();
    }
    /// @}

    /// @name Element access
    /// @{
    auto Vector() const noexcept -> std::vector<int>;
    auto operator[](const std::size_t index) noexcept       -> int&;
    auto operator[](const std::size_t index) const noexcept -> const int&;
    /// @}

    /// @name Front/Back
    /// @{
    auto Front()        -> int&;
    auto Front() const  -> const int&;
    auto Back()         -> int&;
    auto Back() const   -> const int&;
    /// @}

    /// @name Modifiers
    /// @{
    void Clear() noexcept;

    void Pop();
    bool TryPop(int& value);

    void Push(int value);
    bool TryPush(int value);
    
    void Resize(const std::size_t new_capacity);
    /// @}

private:
    template <typename Self>
    static auto& FrontImpl(Self& self);
    template <typename Self>
    static auto& BackImpl(Self& self);

    /**
     * @brief Метод для получения корректного индекса в кольцевом буфере
     * 
     * @param[in] index Индекс относительно начала буфера (самого старого элемента)
     * @return std::size_t Корректный индекс в векторе хранения элементов
     */
    auto ValidIndex(const std::size_t& index) const noexcept {
        return (head_ + index) % Size();
    }

private:
    std::vector<int> buffer_ {};  ///< Вектор для хранения элементов буфера
    std::size_t        head_ {};  ///< Индекс самого старого элемента в буфере
};

RingBuffer::RingBuffer(const RingBuffer& other) {
    buffer_.reserve(other.Capacity());
    buffer_ = other.buffer_;
    head_ = other.head_;
};
RingBuffer& RingBuffer::operator=(const RingBuffer& other) {
    if (this != &other) {
        buffer_.reserve(other.Capacity());
        buffer_ = other.buffer_;
        head_ = other.head_;
    }
    return *this;
}
RingBuffer::RingBuffer(RingBuffer&& other) noexcept {
    buffer_.reserve(other.Capacity());
    buffer_ = std::move(other.buffer_);
    head_ = other.head_;
}
RingBuffer& RingBuffer::operator=(RingBuffer&& other) noexcept {
    if (this != &other) {
        buffer_.reserve(other.Capacity());
        buffer_ = std::move(other.buffer_);
        head_ = other.head_;
    }
    return *this;
};

template<typename Self>
auto& RingBuffer::FrontImpl(Self& self) {
    return self[self.Size() - 1];
}
auto RingBuffer::Front() -> int& {
    return FrontImpl(*this);
}
auto RingBuffer::Front() const -> const int& {
    return FrontImpl(*this);
}

template <typename Self>
auto& RingBuffer::BackImpl(Self& self) {
    return self[0];
}
auto RingBuffer::Back() -> int& {
    return BackImpl(*this);
}
auto RingBuffer::Back() const -> const int& {
    return BackImpl(*this);
}

auto RingBuffer::Vector() const noexcept -> std::vector<int> {
    std::vector<int> buffer;
    buffer.reserve(Size());
    for (std::size_t i = 0; i < Size(); ++i) {
        buffer.push_back((*this)[i]);
    }
    return buffer;
}

auto RingBuffer::operator[](const std::size_t index) noexcept -> int& {
    return buffer_[ValidIndex(index)];
}
auto RingBuffer::operator[](const std::size_t index) const noexcept -> const int& {
    return buffer_[ValidIndex(index)];
}

void RingBuffer::Clear() noexcept {
    buffer_.clear();
    head_ = 0;
}

void RingBuffer::Pop() {
    if (!Empty()) {
        buffer_.erase(buffer_.begin() + ValidIndex(0));
        head_ = Empty() ? 0 : head_ % Size();
    }
}
bool RingBuffer::TryPop(int& value) {
    if (Empty()) {
        return false;
    }
    value = Back();
    Pop();
    return true;
}

void RingBuffer::Push(int value) {
    if (Full()) {
        Back() = value;
        head_ = ValidIndex(1); // Сдвигаем голову вперед по кольцу на один элемент
    } else {
        buffer_.push_back(value);
    }
}
bool RingBuffer::TryPush(int value) {
    if (Full()) {
        return false;
    }
    Push(value);
    return true;
}

void RingBuffer::Resize(const std::size_t new_capacity) {
    const auto valid_capacity { ValidCapacity(new_capacity) };
    if (valid_capacity < Size()) {
        if (head_ + valid_capacity <= Size()) {
            buffer_.erase(
                buffer_.begin() + ValidIndex(0), 
                buffer_.begin() + ValidIndex(Size() - valid_capacity));
        }
        head_ = head_ % valid_capacity;
    }
    buffer_.shrink_to_fit();
    buffer_.reserve(valid_capacity);
}