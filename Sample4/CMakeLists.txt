cmake_minimum_required(VERSION 3.17)
project(Sample4 C)

set(CMAKE_C_STANDARD 11)

add_executable(Sample4 main.c dynlst.h)

target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/libDynLib.dll.a)