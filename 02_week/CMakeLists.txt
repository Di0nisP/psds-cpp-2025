cmake_minimum_required(VERSION 3.14) # Минимальная требуемая версия CMake
project(02_week) # Необходим для инициализации cmake

set(CMAKE_CXX_STANDARD 20) # Версия стандарта C++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Гарантирует использование указанной версии стандарта

set(EXAMPLES_DIR examples) # Определим переменную с именем директории
set(TASKS_DIR tasks)

add_subdirectory(tasks)

# Создать исполняемый файл для каждого примера
if (BUILD_EXAMPLES_02_WEEK)
    add_example(const_cast ${EXAMPLES_DIR}/const_cast.cpp)
    add_example(ref ${EXAMPLES_DIR}/ref.cpp)
    add_example(ptr_address ${EXAMPLES_DIR}/ptr_address.cpp)
    add_example(ptr_access ${EXAMPLES_DIR}/ptr_access.cpp)
    add_example(ptr_arithmetic ${EXAMPLES_DIR}/ptr_arithmetic.cpp)
    add_example(reinterpret ${EXAMPLES_DIR}/reinterpret.cpp)
endif()