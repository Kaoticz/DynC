cmake_minimum_required(VERSION 3.17)
project(DynLib C)

set(CMAKE_C_STANDARD 11)

add_library(DynLib SHARED Headers/dynvar.h Source/dynvar.c Headers/dynarr.h Source/dynarr.c Source/dynlst.c Headers/dynlst.h)