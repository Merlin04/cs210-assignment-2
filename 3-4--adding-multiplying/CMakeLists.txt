cmake_minimum_required(VERSION 3.23)
project(3_adding_2 C)

set(CMAKE_C_STANDARD 23)

add_executable(3_adding_2 main.c)
target_link_libraries(3_adding_2 m)