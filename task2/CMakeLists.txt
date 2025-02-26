cmake_minimum_required(VERSION 3.15)

project(main)

include_directories(src/headers)

file(GLOB SOURCES "src/cpp/*.cpp")

add_executable(main main.cpp 
                    src/cpp/generator.cpp
                    src/cpp/math_func.cpp)