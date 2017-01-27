cmake_minimum_required(VERSION 3.6)
project(SystemSoftware)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES Lab1/main.c Lab1/area_of_rec.c Lab1/area_of_rec.h Lab1/area_of_triangle.c Lab1/area_of_triangle.h Lab1/area_of_circle.c Lab1/area_of_circle.h Lab1/roman_num.c Lab1/roman_num.h)
add_executable(SystemSoftware ${SOURCE_FILES})