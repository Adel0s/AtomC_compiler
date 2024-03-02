cmake_minimum_required(VERSION 3.27)
project(AtomC_copy C)

set(CMAKE_C_STANDARD 11)

add_executable(AtomC_copy main.c
        lexer.c
        lexer.h
        utils.c
        utils.h)
