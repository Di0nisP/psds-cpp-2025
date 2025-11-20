cmake_minimum_required(VERSION 3.14)
project(psds_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_EXAMPLES_01_WEEK "Build examples for 01_week" OFF)
option(BUILD_EXAMPLES_02_WEEK "Build examples for 02_week" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
find_package(GoogleTest REQUIRED)

set(CMAKE_BINARY_TASKS_DIR ${CMAKE_BINARY_DIR}/tasks)

include(cmake/TestSolution.cmake)
include(cmake/AsanFlags.cmake)

function(add_week NAME)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${NAME})
        add_subdirectory(${NAME})
    endif()
endfunction()

add_executable(psds_cpp main.cpp)

add_week(01_week)
add_week(02_week)
add_week(03_week)
add_week(04_week)
add_week(05_week)
add_week(06_week)
add_week(07_week)
add_week(08_week)
add_week(09_week)
add_week(10_week)