#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "lexer.h"
#include "parser.h"
#include "ad.h"

int main()
{
    char *input_buffer=loadFile("tests/testad.c");
    puts(input_buffer);

    // va returna lista atomilor extrasi din fisierul incarcat in input_buffer
    // integrare analizor lexical
    Token *tokens = tokenize(input_buffer);
    showTokens(tokens); // afisare atomi lexicali (sfarsitul analizei lexicale)

    // integrare analizor de domeniu
    pushDomain(); // creaza domeniul global in tabela de simboluri

    // integrare analizor sintactic
    parse(tokens);

    // afisare domeniu
    showDomain(symTable,"global"); // afisare domeniu global
    dropDomain(); // sterge domeniul global

    free(input_buffer);



    return 0;
}
