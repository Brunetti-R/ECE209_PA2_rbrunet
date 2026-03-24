cmake_minimum_required(VERSION 3.30)
project(scopone C)

set(CMAKE_C_STANDARD 17)

add_executable(scopone main.c
        scopone.c
        scopone.h)