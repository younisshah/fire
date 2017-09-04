cmake_minimum_required(VERSION 3.6)
project(fire)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c client.c fire.h)
add_executable(fire ${SOURCE_FILES})