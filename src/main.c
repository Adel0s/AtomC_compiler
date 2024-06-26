#include <stdio.h>
#include <stdlib.h>
#include "headers/utils.h"
#include "headers/lexer.h"
#include "headers/parser.h"
#include "headers/ad.h"

int main()
{
    char *input_buffer=loadFile("tests/testgc.c");
    //puts(input_buffer);

    // va returna lista atomilor extrasi din fisierul incarcat in input_buffer
    // integrare analizor lexical
    Token *tokens = tokenize(input_buffer);
    //showTokens(tokens); // afisare atomi lexicali (sfarsitul analizei lexicale)

    // integrare analizor de domeniu
    pushDomain(); // creaza domeniul global in tabela de simboluri

    vmInit(); // initializare masina virtuala

    // integrare analizor sintactic
    parse(tokens);

    printf("\n");

    // generare de cod
    Symbol *symMain=findSymbolInDomain(symTable,"main");
    if(!symMain)err("missing main function");
    Instr *entryCode=NULL;
    addInstr(&entryCode,OP_CALL)->arg.instr=symMain->fn.instr;
    addInstr(&entryCode,OP_HALT);
    run(entryCode);

    // afisare domeniu
    showDomain(symTable,"global"); // afisare domeniu global

//    Instr *testCode = genTestProgramFloat(); // genereaza cod de test pentru masina virtuala
//    run(testCode); // executie cod masina virtual

    dropDomain(); // sterge domeniul global

    free(input_buffer);



    return 0;
}
