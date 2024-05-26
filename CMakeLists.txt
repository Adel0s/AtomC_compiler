cmake_minimum_required(VERSION 3.27)
project(AtomC_copy C)

set(CMAKE_C_STANDARD 11)

add_executable(AtomC_copy src/main.c
        src/lexer.c
        src/headers/lexer.h
        src/utils.c
        src/headers/utils.h
        src/parser.c
        src/headers/parser.h
        src/ad.c
        src/headers/ad.h
        src/vm.c
        src/headers/vm.h
        src/at.c
        src/headers/at.h
        src/gc.c
        src/headers/gc.h)
