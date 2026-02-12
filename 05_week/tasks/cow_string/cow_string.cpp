#include <cstring>
#include <string>
#include <stdexcept>

class CowString {
    struct StringData {
        size_t size;
        char* data;
        size_t ref_count;

        StringData(const char* cstr);
        StringData(const std::string& str);
        ~StringData();
    };

public:
    CowString();
    CowString(const char* cstr);
    CowString(const std::string& str);

    /// @name Копирование / перемещение
    /// @{
    CowString(const CowString& other);
    CowString& operator=(const CowString& other);

    CowString(CowString&& other) noexcept;
    CowString& operator=(CowString&& other) noexcept;
    /// @}

    ~CowString();
    
    /// @name Методы, не вызывающие копирования
    /// @{
    size_t Size() const;
    bool Empty() const;
    
    const char* ToCstr() const;
    std::string ToString() const;

    size_t Find(const char* substring) const;
    size_t Find(const std::string& substring) const;
    size_t Find(char ch) const;

    const char& operator[](size_t index) const;
    operator const char*() const;
    /// @}

    /// @name Методы, обеспечивающие модификацию на собственной копии данных 
    /// @{
    char& operator[](size_t index);

    CowString& Append(const char* cstr);
    CowString& Append(const std::string& str);

    CowString  Substr(size_t pos = 0, size_t count = npos) const;

    CowString& Clear();
    /// @}

private:
    /**
     * @brief Отсоединяет эту строку от общих данных
     * и создает независимую копию данных для этого экземпляра.
     * 
     * Этот метод реализует шаблон копирования при записи (CoW) 
     * путем создания независимой копии строковых данных, 
     * когда требуются изменения. 
     *
     * - Если эта строка является единственной ссылкой,
     * с данными не выполняется никаких действий. 
     * - Если существует несколько ссылок, создается новая копия 
     * и количество ссылок обновляется соответствующим образом.
     *
     * @note Обычно это вызывается внутри системы 
     * перед любой неконстантной операцией, которая изменяет данные строки.
     */
    void Detach();

public:
    inline static constexpr size_t npos = -1;

private:
    StringData* string_data_;
};

// -------------------------------------------------------------------------
// CowString::StringData implementation
// -------------------------------------------------------------------------

CowString::StringData::StringData(const char* cstr)
{
    /// @warning UB при nullptr

    size = std::strlen(cstr);
    data = new char[size + 1]; // +1 для нуль-терминатора
    std::strcpy(data, cstr);
    ref_count = 1;
}

CowString::StringData::StringData(const std::string& str)
    : StringData(str.c_str())
{

}

CowString::StringData::~StringData() 
{
    if (data != nullptr)
        delete[] data;
}

// -------------------------------------------------------------------------
// CowString implementation
// -------------------------------------------------------------------------

CowString::CowString()
    : string_data_(new StringData(""))
{

}

CowString::CowString(const char* cstr)
    : string_data_(new StringData(cstr))
{

}

CowString::CowString(const std::string& str)
    : CowString(str.c_str())
{

}

CowString::CowString(const CowString& other)
    : string_data_(other.string_data_)
{
    ++string_data_->ref_count;
}

CowString& CowString::operator=(const CowString& other) 
{
    if (this != &other) { 
        // Отпускаем свои данные 
        if (--string_data_->ref_count == 0) 
            delete string_data_; 
        
        // Присоединяемся к чужим 
        string_data_ = other.string_data_; 
        ++string_data_->ref_count;
    }
    return *this;
}

CowString::CowString(CowString&& other) noexcept
    : string_data_(other.string_data_)
{
    other.string_data_ = new StringData("");
}

CowString& CowString::operator=(CowString&& other) noexcept 
{
    if (this != &other) {
        // Отпускаем свои данные 
        if (--string_data_->ref_count == 0) 
            delete string_data_; 
        
        string_data_ = other.string_data_;
        other.string_data_ = new StringData("");
    }
    return *this;
}

CowString::~CowString() 
{
    --string_data_->ref_count;
    if (string_data_->ref_count == 0 && string_data_ != nullptr) {
        delete string_data_;
    }
}

// Методы, не вызывающие копирования

auto CowString::Size() const -> std::size_t 
{
    return string_data_->size;
}

auto CowString::Empty() const -> bool 
{
    return string_data_->size == 0;
}

auto CowString::ToCstr() const -> const char*
{
    return string_data_->data;
}

auto CowString::ToString() const -> std::string
{
    return std::string(string_data_->data);
}

auto CowString::Find(const char* substring) const -> size_t
{
    const char* found = std::strstr(string_data_->data, substring);
    return found ? static_cast<size_t>(found - string_data_->data) : npos;
}

auto CowString::Find(const std::string& substring) const -> size_t
{
    return Find(substring.c_str());
}

auto CowString::Find(char ch) const -> size_t
{
    const char* found = std::strchr(string_data_->data, ch); 
    return found ? static_cast<size_t>(found - string_data_->data) : npos;
}

auto CowString::operator[](size_t index) const -> const char&
{
    if (index >= string_data_->size)
        throw std::out_of_range("Index out of bounds");

    return string_data_->data[index];
}

CowString::operator const char*() const 
{ 
    return string_data_->data; 
}

// Методы, обеспечивающие модификацию на собственной копии данных

auto CowString::operator[](size_t index) -> char&
{
    if (index >= string_data_->size)
        throw std::out_of_range("Index out of bounds");

    Detach();

    return string_data_->data[index];
}

auto CowString::Append(const char* cstr) -> CowString&
{
    if (std::strlen(cstr) == 0)
        return *this; // Нет необходимости в копировании

    Detach();

    const auto new_size = string_data_->size + std::strlen(cstr);
    char* new_data = new char[new_size + 1];

    std::strcpy(new_data, string_data_->data);
    std::strcat(new_data, cstr);

    delete[] string_data_->data;
    string_data_->data = new_data;
    string_data_->size = new_size;

    return *this;
}

auto CowString::Append(const std::string& str) -> CowString&
{
    return Append(str.c_str());
}

auto CowString::Substr(size_t pos, size_t count) const -> CowString
{
    if (pos >= string_data_->size)
        return CowString();

    const auto new_size = (count > string_data_->size - pos) ? string_data_->size - pos : count;
    char* new_data = new char[new_size + 1]; // +1 для нуль-терминатора

    std::strncpy(new_data, string_data_->data + pos, new_size);
    new_data[new_size] = '\0'; // Гарантировать нуль-терминатор

    CowString result(new_data);
    delete[] new_data; // Удалить временные данные return result

    return result;
}

auto CowString::Clear() -> CowString&
{
    Detach();

    delete string_data_;
    string_data_ = new StringData("");
    
    return *this;
}

// private

void CowString::Detach() 
{
    if (string_data_->ref_count > 1) {
        --string_data_->ref_count;
        string_data_ = new StringData(string_data_->data);
    }
}