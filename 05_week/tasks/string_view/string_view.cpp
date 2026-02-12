#include <cstring>
#include <string>

/**
 * @brief Класс, представляющий упрощенную реализацию std::string_view
 * @note Не может изменять строку, за которой наблюдает, и не владеет ею.
 */
class StringView {
public:
    StringView() = default;

    /// @param[in] cstr static storage duration string
    StringView(const char* cstr, size_t count = npos);
    StringView(const std::string&  cstr, size_t pos = 0, size_t count = npos);
    StringView(const std::string&& cstr, size_t pos = 0, size_t count = npos);

    StringView(const StringView&) = delete;
    StringView& operator=(const StringView&) = delete;
    StringView(StringView&&) noexcept = default;
    StringView& operator=(StringView&&) noexcept = default;

    ~StringView();

    const char& operator[](size_t index) const;
    const char* Data() const;

    const char& Front() const;
    const char& Back() const;

    bool Empty() const;
    size_t Size() const;
    size_t Length() const;
    
    void RemovePrefix(size_t n);
    void RemoveSuffix(size_t n);

    StringView Substr(size_t pos = 0, size_t count = npos) const;
    
    size_t Find(char ch, size_t pos = 0) const;
    size_t Find(const StringView& substring, size_t pos = 0) const;

    std::string ToString() const;

private:
    StringView(const char* cstr, size_t count, bool owner);

public:
    inline static constexpr size_t npos = -1;

private:
    /**
     * @brief Флаг, указывающий, владеет ли StringView данными строки
     * 
     * - @c true , если StringView владеет данными (например, создан из временной строки);
     * - @c false , если StringView не владеет данными (например, создан из существующей строки).
     *
     * @note Модификация поведения std::string_view.
     */
    bool owner_ = false; 

    size_t size_ = 0;
    const char* data_ = nullptr;
};

StringView::StringView(const char* cstr, size_t count)
    : StringView(cstr, count, false)
{

}


StringView::StringView(const std::string& cstr, size_t pos, size_t count)
    : StringView(pos >= cstr.size() ? nullptr : cstr.c_str() + pos, count, false)
{

}

StringView::StringView(const std::string&& cstr, size_t pos, size_t count)
    : StringView(pos >= cstr.size() ? nullptr : cstr.c_str() + pos, count, true)
{

}

StringView::~StringView() 
{ 
    if (owner_ && data_ != nullptr) 
        delete[] data_; 
}

const char& StringView::operator[](size_t index) const
{
    return data_[index];
}

const char* StringView::Data() const
{
    return data_;
}

bool StringView::Empty() const
{
    return size_ == 0;
}

size_t StringView::Size() const
{
    return size_;
}

size_t StringView::Length() const
{
    return Size();
}

void StringView::RemovePrefix(size_t n)
{
    if (n > size_)
        n = size_;

    data_ += n;
    size_ -= n;
}

void StringView::RemoveSuffix(size_t n)
{
    if (n > size_)
        n = size_;

    size_ -= n;
}

const char& StringView::Front() const
{
    return Data()[0];
}
const char& StringView::Back() const
{
    return Data()[Size() - 1];
}

size_t StringView::Find(char ch, size_t pos) const
{
    for (size_t i = pos; i < Size(); ++i) {
        if (Data()[i] == ch)
            return i;
    }
    return npos;
}

size_t StringView::Find(const StringView& substring, size_t pos) const
{
    // Пустая подстрока 
    if (substring.Empty()) 
        return Empty() ? npos : pos; 
    
    // Подстрока длиннее строки 
    if (substring.Size() > Size() || pos > Size() - substring.Size()) 
        return npos;

    for (size_t i = pos; i <= Size() - substring.Size(); ++i)
        if (std::strncmp(Data() + i, substring.Data(), substring.Size()) == 0)
            return i;

    return npos;
}

StringView StringView::Substr(size_t pos, size_t count) const
{
    if (Data() == nullptr || pos >= Size())
        return StringView();

    const size_t new_size = (count > Size() - pos) ? Size() - pos : count;
    return StringView(Data() + pos, new_size, false);
}

std::string StringView::ToString() const
{
    return std::string(Data(), Size());
}

// private constructor

StringView::StringView(const char* cstr, size_t count, bool owner)
    : owner_(owner)
{
    if (cstr == nullptr)
        return;

    const size_t cstr_len = std::strlen(cstr);
    size_ = count > cstr_len ? cstr_len : count; 

    if (owner_) { // Владеет данными, необходимо создать копию 
        char* new_data = new char[size_ + 1]; 
        std::strncpy(new_data, cstr, size_); 
        new_data[size_] = '\0'; // Гарантировать нуль-терминатор 
        data_ = new_data; 
    } else { // Не владеет данными, просто ссылается на существующую строку 
        data_ = cstr; 
    }
}