#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "lexer.h"
#include "parser.h"

int main()
{
    char *input_buffer=loadFile("tests/testparser.c");
    puts(input_buffer);

    // va returna lista atomilor extrasi din fisierul incarcat in input_buffer
    Token *tokens = tokenize(input_buffer);
    showTokens(tokens);

    // integrare analizor sintactic
    parse(tokens);

    free(input_buffer);

    return 0;
}
