#include <iostream>
#include <memory>


struct Logger {
    Logger() { std::cerr << "default ctor" << std::endl; }
    Logger(const Logger&) { std::cerr << "copy ctor" << std::endl; }
    Logger(Logger&&) { std::cerr << "move ctor" << std::endl; }
    Logger& operator=(const Logger&) { std::cerr << "copy assignment" << std::endl; return *this; }
    Logger& operator=(Logger&&) { std::cerr << "move assignment" << std::endl; return *this; }
    ~Logger() { std::cerr << "~dctor" << std::endl; }
};


int main() {
    std::unique_ptr<Logger> ptr{new Logger()};
    {
        std::unique_ptr<Logger> ptr2{new Logger()};
    }
}