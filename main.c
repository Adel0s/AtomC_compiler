#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "lexer.h"

int main()
{
    char *input_buffer=loadFile("tests/testlex.c");
    puts(input_buffer);

    // va returna lista atomilor extrasi din fisierul incarcat in input_buffer
    Token *tokens = tokenize(input_buffer);
    showTokens(tokens);

    free(input_buffer);

    return 0;
}
