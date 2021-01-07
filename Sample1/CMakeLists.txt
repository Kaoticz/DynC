cmake_minimum_required(VERSION 3.17)
project(Sample1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Sample1 main.c)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/libDynLib.dll.a)