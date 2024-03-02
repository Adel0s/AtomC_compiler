//
// Created by adel_ on 3/2/2024.
//

#ifndef ATOMC_COPY_LEXER_H
#define ATOMC_COPY_LEXER_H
#pragma once

enum{
    ID
    /// keywords
    , TYPE_INT, TYPE_CHAR, TYPE_DOUBLE, ELSE, IF, RETURN, STRUCT, VOID, WHILE
    /// delimiters
    ,COMMA, SEMICOLON, LPAR, RPAR, LBRACKET, RBRACKET, LACC, RACC, END
    /// operators
    ,ADD, MUL, DIV, DOT, AND, OR, NOT, NOTEQ, LESS, LESSEQ, GREATER, GREATEREQ, ASSIGN, EQUAL, SUB
};

typedef struct Token{
    int code;		// ID, TYPE_CHAR, ...
    int line;		// the line from the input file

    /// vom folosi unul dintre aceste campuri la un moment dat
    union{
        char *text;		// the chars for ID, STRING (dynamically allocated)
        int i;		// the value for INT
        char c;		// the value for CHAR
        double d;		// the value for DOUBLE
    };
    struct Token *next;		// next token in a simple linked list
}Token;

Token *tokenize(const char *pch);
void showTokens(const Token *tokens);

#endif //ATOMC_COPY_LEXER_H
