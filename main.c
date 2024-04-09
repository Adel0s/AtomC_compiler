#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "ad.h"

int main()
{
    char *input_buffer=loadFile("tests/testparser.c");
    puts(input_buffer);

    // va returna lista atomilor extrasi din fisierul incarcat in input_buffer
    Token *tokens = tokenize(input_buffer);
    showTokens(tokens);

    // integrare analizor de domeniu
    // pushDomain();

    // integrare analizor sintactic
    parse(tokens);

    // afisare domeniu
    // showDomain(symTable, "global");
    // dropDomain();

    free(input_buffer);



    return 0;
}
