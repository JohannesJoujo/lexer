cmake_minimum_required(VERSION 3.24)
project(lexer)

set(CMAKE_CXX_STANDARD 17)

add_executable(lexer main.cpp lexer.cpp lexer.h expr.cpp expr.h)
