cmake_minimum_required(VERSION 3.31)
project(json_parser C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_ASM_STANDARD_REQUIRED ON)

include_directories(headers)


add_executable(json_parser main.c)
