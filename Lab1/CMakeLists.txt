cmake_minimum_required(VERSION 3.17)
project(Lab1 C)

set(CMAKE_C_STANDARD 99)

add_executable(Lab1 main.c bmp_serializers.c rotate.c bmp_serializers.h bmp_struct.h rotate.h)