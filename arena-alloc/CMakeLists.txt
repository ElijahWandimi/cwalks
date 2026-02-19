cmake_minimum_required(VERSION 3.31)
project(arena_alloc C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_ASM_STANDARD_REQUIRED ON)

add_executable(arena_alloc main.c)
