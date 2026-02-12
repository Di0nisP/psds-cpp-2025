#include <initializer_list>
#include <algorithm> // std::copy_n
#include <utility>   // std::exchange
#include <stdexcept> // std::out_of_range

class SimpleVector;

void swap(SimpleVector& lhs, SimpleVector& rhs);

int* begin(SimpleVector& v) noexcept;
int* end(SimpleVector& v) noexcept ;
// range‑based for не знает про cbegin и cend
const int* begin(const SimpleVector& v) noexcept;
const int* end(const SimpleVector& v) noexcept;

class SimpleVector {
public:
    SimpleVector() = default;
    SimpleVector(size_t size, int value = 0);
    SimpleVector(std::initializer_list<int> init);

    SimpleVector(const SimpleVector& other);
    SimpleVector& operator=(const SimpleVector& other);
    SimpleVector(SimpleVector&& other) noexcept;
    SimpleVector& operator=(SimpleVector&& other) noexcept;

    ~SimpleVector();

    int& operator[](size_t index);
    const int& operator[](size_t index) const;

    void Swap(SimpleVector& other);
    

    void PushBack(int value);
    void PopBack();
    int* Insert(const int* pos, int value);
    int* Erase(const int* pos);
    void Clear();
    
    void Resize(size_t new_size, int value = 0);
    void Reserve(size_t new_capacity);
    
    size_t Size() const noexcept;
    size_t Capacity() const noexcept;
    bool Empty() const noexcept;

    int* Data() noexcept;
    const int* Data() const noexcept;

    bool operator==(const SimpleVector& other) const;
    bool operator!=(const SimpleVector& other) const = default;

private:
    size_t size_     = 0;
    size_t capacity_ = 0;
    // Поведение аналогично std::vector: при capacity_ == 0, data_ == nullptr
    int*   data_     = nullptr;
};

void swap(SimpleVector& lhs, SimpleVector& rhs) 
{ 
    lhs.Swap(rhs); 
}

int* begin(SimpleVector& v) noexcept 
{
    return v.Data();
}
int* end(SimpleVector& v) noexcept 
{
    return v.Data() + v.Size();
}
const int* begin(const SimpleVector& v) noexcept 

{
    return v.Data();
}
const int* end(const SimpleVector& v) noexcept 
{
    return v.Data() + v.Size();
}


SimpleVector::SimpleVector(size_t size, int value)
    : size_(size), capacity_(size_)
    , data_(capacity_ > 0 ? new int[capacity_] : nullptr)
{
    std::fill_n(data_, size_, value);
}

SimpleVector::SimpleVector(std::initializer_list<int> init)
    : size_(init.size()), capacity_(size_)
    , data_(capacity_ > 0 ? new int[capacity_] : nullptr)
{
    std::copy_n(init.begin(), size_, data_);
}

SimpleVector::SimpleVector(const SimpleVector& other)
    : size_(other.size_), capacity_(other.capacity_)
    , data_(capacity_ > 0 ? new int[capacity_] : nullptr)
{
    std::copy_n(other.data_, size_, data_);
}

SimpleVector& SimpleVector::operator=(const SimpleVector& other)
{
    if (this != &other) {
        SimpleVector temp(other);
        Swap(temp);
    }
    return *this;
}

SimpleVector::SimpleVector(SimpleVector&& other) noexcept
    : size_(std::exchange(other.size_, 0))
    , capacity_(std::exchange(other.capacity_, 0))
    , data_(std::exchange(other.data_, nullptr))
{

}

SimpleVector& SimpleVector::operator=(SimpleVector&& other) noexcept
{    
    if (this != &other) {
        SimpleVector temp;
        Swap(other);
        temp.Swap(other);
    }

    return *this;
}

SimpleVector::~SimpleVector()
{
    if (data_ != nullptr)
        delete[] data_;
}

int& SimpleVector::operator[](size_t index)
{
    if (index >= size_)
        throw std::out_of_range("Index out of bounds");

    return data_[index];
}

const int& SimpleVector::operator[](size_t index) const
{
    if (index >= size_) 
        throw std::out_of_range("Index out of bounds");
    
    return data_[index];
}

void SimpleVector::Swap(SimpleVector& other)
{
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(data_, other.data_);
}

void SimpleVector::PushBack(int value)
{
    if (size_ == capacity_)
        Reserve(capacity_ == 0 ? 1 : capacity_ * 2);

    data_[size_++] = value;
}
void SimpleVector::PopBack()
{
    if (size_ > 0)
        --size_;
}
int* SimpleVector::Insert(const int* pos, int value)
{
    const size_t index = pos - data_;
    if (index > size_)
        return data_ + size_;

    if (size_ == capacity_)
        Reserve(capacity_ == 0 ? 1 : capacity_ * 2);

    // Перемещение слева направо
    std::move_backward(data_ + index, data_ + size_, data_ + size_ + 1);
    ++size_;

    return (data_[index] = value, data_ + index);
}
int* SimpleVector::Erase(const int* pos)
{
    const size_t index = pos - data_;
    if (index >= size_)
        return data_ + size_;

    // Перемещение справа налево
    std::move(data_ + index + 1, data_ + size_, data_ + index);
    --size_;

    return data_ + index;
}
void SimpleVector::Clear()
{
    size_ = 0;
}

void SimpleVector::Resize(size_t new_size, int value)
{
    if (new_size > size_) {
        if (new_size > capacity_)
            Reserve(new_size);

        std::fill(data_ + size_, data_ + new_size, value);
    }

    size_ = new_size;
}
void SimpleVector::Reserve(size_t new_capacity)
{
    if (new_capacity > capacity_) {
        int* new_data = new int[new_capacity];
        std::copy_n(data_, size_, new_data);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
}

size_t SimpleVector::Size() const noexcept
{
    return size_;
}
size_t SimpleVector::Capacity() const noexcept
{
    return capacity_;
}
bool SimpleVector::Empty() const noexcept
{
    return size_ == 0;
}

int* SimpleVector::Data() noexcept
{    
    return data_;
} 
const int* SimpleVector::Data() const noexcept
{    
    return data_;
}  

bool SimpleVector::operator==(const SimpleVector& other) const
{
    return Size() == other.Size() && std::equal(Data(), Data() + Size(), other.Data()); 
}