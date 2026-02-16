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
    StringView(const std::string&& cstr, size_t pos = 0, size_t count = npos) = delete;

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

public:
    inline static constexpr size_t npos = -1;

private:
    size_t size_ = 0;
    const char* data_ = nullptr;
};

StringView::StringView(const char* cstr, size_t count)
{
    if (cstr == nullptr)
        return;

    const size_t cstr_len = std::strlen(cstr);
    size_ = count > cstr_len ? cstr_len : count; 
    data_ = cstr; 
}


StringView::StringView(const std::string& cstr, size_t pos, size_t count)
    : StringView(pos >= cstr.size() ? nullptr : cstr.c_str() + pos, count)
{

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
        return 0; 
    
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
    return StringView(Data() + pos, new_size);
}

std::string StringView::ToString() const
{
    return std::string(Data(), Size());
}