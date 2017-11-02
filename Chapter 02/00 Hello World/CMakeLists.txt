cmake_minimum_required(VERSION 3.6)
project(00_Hello_World)

set(CMAKE_C_STANDARD 11)

add_executable(00_Hello_World_KandR_C main_KandR_C.c)
add_executable(00_Hello_World_C11 main_C11.c)
add_executable(00_Hello_World_Main_with_Args main_with_args.c)