cmake_minimum_required(VERSION 3.17)
project(Sample2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Sample2 main.c dynarr.h dynvar.h)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/libDynLib.dll.a)