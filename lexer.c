#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

Token *tokens;	// single linked list of tokens
Token *lastTk;	// the last token in list

int line=1;		// the current line in the input file

// adds a token to the end of the tokens list and returns it
// sets its code and line
Token *addTk(int code)
{
    Token *tk=safeAlloc(sizeof(Token));
    tk->code=code;
    tk->line=line;
    tk->next=NULL;
    if(lastTk)
    {
        lastTk->next=tk;
    }
    else
    {
        tokens=tk;
    }
    lastTk=tk;
    return tk;
}

///de completat
/// primeste 2 pointeri si extrage intr-o zona alocata dinamic subsirul dintre cei doi pointeri [begin, end)
char *extract(const char *begin,const char *end)
{
    if (begin == NULL || end == NULL || begin >= end)
    {
        err("invalid pointers");
        return NULL;
    }

    size_t length = end - begin;

    /// alocam memorie dinamic pentru sirul extras
    char *extracted_text = (char*)safeAlloc((size_t)length+1);

    /// copiem carcterele din intervalul [begin, end)
    size_t i = 0;
    for(i = 0; i < length; i++)
    {
        extracted_text[i] = begin[i];
    }

    /// adaugam terminatorul de sir de caracter
    extracted_text[length] = '\0';

    return extracted_text;
}

void *remove_quotes(char *text)
{
    int i, j;
    size_t len = strlen(text);

    /// vezi de ce e necesar sa pui -1 ca altfel afiseaza un caracter in plus...
    for(i = 0, j = 0; i < len-1; i++)
    {
        if(text[i] != '"')
        {
            text[j++] = text[i];
        }
    }

    text[j] = '\0';
}

/// de completat
Token *tokenize(const char *pch)  /// pch = pointer to current char
{
    const char *start;
    Token *tk;
    for(;;)
    {
        switch(*pch)
        {
            case ' ':
            case '\t':
                pch++;
                break;
            case '\r':		// handles different kinds of newlines (Windows: \r\n, Linux: \n, MacOS, OS X: \r or \n)
                if(pch[1]=='\n')pch++;
                // fallthrough to \n
            case '\n':
                line++;
                pch++;
                break;
            case '\0':
                addTk(END);
                return tokens;
            case ',':
                addTk(COMMA);
                pch++;
                break;
            case ';':
                addTk(SEMICOLON);
                pch++;
                break;
            case '(':
                addTk(LPAR);
                pch++;
                break;
            case ')':
                addTk(RPAR);
                pch++;
                break;
            case '{':
                addTk(LACC);
                pch++;
                break;
            case '}':
                addTk(RACC);
                pch++;
                break;
            case '[':
                addTk(LBRACKET);
                pch++;
                break;
            case ']':
                addTk(RBRACKET);
                pch++;
                break;
            case '+':
                addTk(ADD);
                pch++;
                break;
            case '-':
                addTk(SUB);
                pch++;
                break;
            case '*':
                addTk(MUL);
                pch++;
                break;
            case '.':
                if(isdigit(pch[1]) == 0)
                {
                    addTk(DOT);
                    pch++;
                }
                break;
                /// to implement for AND &&, OR ||, DIV /, LINECOMMENT //
            case '=':
                if(pch[1]=='=')
                {
                    addTk(EQUAL);
                    pch+=2;
                }
                else
                {
                    addTk(ASSIGN);
                    pch++;
                }
                break;
            case '!':
                if(pch[1]=='=')
                {
                    addTk(NOTEQ);
                    pch+=2;
                }
                else
                {
                    addTk(NOT);
                    pch++;
                }
                break;
            case '<':
                if(pch[1]=='=')
                {
                    addTk(LESSEQ);
                    pch+=2;
                }
                else
                {
                    addTk(LESS);
                    pch++;
                }
                break;
            case '>':
                if(pch[1]=='=')
                {
                    addTk(GREATEREQ);
                    pch+=2;
                }
                else
                {
                    addTk(GREATER);
                    pch++;
                }
                break;
            case '&':
                if(pch[1] == '&')
                {
                    addTk(AND);
                    pch += 2;
                }
                else
                {
                    err("invalid symbol: %c (%d)",*pch,*pch);
                }
                break;
            case '|':
                if(pch[1]== '|')
                {
                    addTk(OR);
                    pch += 2;
                }
                else
                {
                    err("invalid symbol: %c (%d)",*pch,*pch);
                }
                break;
            default:
                if(isalpha(*pch)||*pch=='_')
                {
                    for(start=pch++; isalnum(*pch)||*pch=='_'; pch++) {}
                    char *text=extract(start,pch); ///[start, pch) deoarece pch este pe urmatorul caracter -> exemplu: tmp; -> pch se va afla pe carcaterul ;
                    /// trebuie sa introducem toate cuvintele cheie
                    if(strcmp(text,"char")==0)addTk(TYPE_CHAR);
                    else if(strcmp(text, "double") == 0) addTk(TYPE_DOUBLE);
                    else if(strcmp(text, "int") == 0) addTk(TYPE_INT);
                    else if(strcmp(text, "if") == 0) addTk(IF);
                    else if(strcmp(text, "else") == 0) addTk(ELSE);
                    else if(strcmp(text, "struct") == 0) addTk(STRUCT);
                    else if(strcmp(text, "void") == 0) addTk(VOID);
                    else if(strcmp(text, "while") == 0) addTk(WHILE);
                    else if(strcmp(text, "return") == 0) addTk(RETURN);
                    else
                    {
                        tk=addTk(ID);
                        tk->text=text;
                    }
                }
                /// de adaugat pentru numere
                /*
                    INT: [0-9]+
                    DOUBLE: [0-9]+ ( '.' [0-9]+ ( [eE] [+-]? [0-9]+ )? | ( '.' [0-9]+ )? [eE] [+-]? [0-9]+ )
                    de completat pentru double sa permita si scrierea cu e sau E
                 */
                else if(isdigit(*pch) || *pch=='.')
                {
                    for (start = pch++; isdigit(*pch) || *pch=='.' ; pch++) {}
                    char *text = extract(start, pch);
                    char *text_to_double = NULL;

                    // convert text to double
                    double value = strtod(text, &text_to_double);

                    if(strchr(text, '.') != NULL)
                    {
                        tk=addTk(DOUBLE);
                        tk->d=value;
                    }
                    else
                    {
                        tk= addTk(INT);
                        tk->i=(int)value; //casting double to int value
                    }
                }
                /// si clase de caractere
                /// STRING: ["] [^"]* ["]
                else if(*pch == '\"')
                {
                    for (start = pch++; *pch!='\"'; pch++) {}

                    // Extract string. Example puts("abc"); -> text value = "abc"
                    char *text = extract(start, pch);
                    text[strlen(text)]='"';
                    // Add string terminator
                    text[strlen(text)+1]='\0';
                    // Set current pointer on the last character from string "text"
                    pch++;

                    // We should remove " characters
                    remove_quotes(text);
                    tk=addTk(STRING);
                    tk->text=text;
                }
                else err("invalid char: %c (%d)",*pch,*pch);
        }
    }
}

/// de completat
/// afisarea liniei iar in loc de cod va trebui sa afiseze numele atomului si daca e cazul -> caracterele din care e format identificatorul.
void showTokens(const Token *tokens)
{
    const Token *tk;
    char code[12];

    char *text;
    int *type_int_value;
    double *type_double_value;

    for(tk=tokens; tk; tk=tk->next)
    {
        size_t length = 0;

        text = NULL;
        type_int_value = NULL;
        type_double_value = NULL;

        switch(tk->code)
        {
            case 0:
                strcpy(code, "ID");
                length = strlen(tk->text);
                text = (char*)safeAlloc((size_t)length+1);
                strcpy(text, tk->text);
                break;
            case 1:
                strcpy(code, "TYPE_INT");
                break;
            case 2:
                strcpy(code, "TYPE_CHAR");
                break;
            case 3:
                strcpy(code, "TYPE_DOUBLE");
                break;
            case 4:
                strcpy(code, "ELSE");
                break;
            case 5:
                strcpy(code, "IF");
                break;
            case 6:
                strcpy(code, "RETURN");
                break;
            case 7:
                strcpy(code, "STRUCT");
                break;
            case 8:
                strcpy(code, "VOID");
                break;
            case 9:
                strcpy(code, "WHILE");
                break;
            case 10:
                strcpy(code, "COMMA");
                break;
            case 11:
                strcpy(code, "SEMICOLON");
                break;
            case 12:
                strcpy(code, "LPAR");
                break;
            case 13:
                strcpy(code, "RPAR");
                break;
            case 14:
                strcpy(code, "LBRACKET");
                break;
            case 15:
                strcpy(code, "RBRACKET");
                break;
            case 16:
                strcpy(code, "LACC");
                break;
            case 17:
                strcpy(code, "RACC");
                break;
            case 18:
                strcpy(code, "END");
                break;
            case 19:
                strcpy(code, "ADD");
                break;
            case 20:
                strcpy(code, "MUL");
                break;
            case 21:
                strcpy(code, "DIV");
                break;
            case 22:
                strcpy(code, "DOT");
                break;
            case 23:
                strcpy(code, "AND");
                break;
            case 24:
                strcpy(code, "OR");
                break;
            case 25:
                strcpy(code, "NOT");
                break;
            case 26:
                strcpy(code, "NOTEQ");
                break;
            case 27:
                strcpy(code, "LESS");
                break;
            case 28:
                strcpy(code, "LESSEQ");
                break;
            case 29:
                strcpy(code, "GREATER");
                break;
            case 30:
                strcpy(code, "GREATEREQ");
                break;
            case 31:
                strcpy(code, "ASSIGN");
                break;
            case 32:
                strcpy(code, "EQUAL");
                break;
            case 33:
                strcpy(code, "SUB");
                break;
            case 34:
                strcpy(code, "INT");
                type_int_value = (int*)safeAlloc((size_t)sizeof(int));
                *type_int_value = tk->i;
                break;
            case 35:
                strcpy(code, "DOUBLE");
                type_double_value = (double*)safeAlloc((size_t)sizeof(double));
                *type_double_value = tk->d;
                break;
            case 36:
                strcpy(code, "CHAR");
                break;
            case 37:
                strcpy(code, "STRING");
                length = strlen(tk->text);
                text = (char*)safeAlloc((size_t)length+1);
                strcpy(text, tk->text);
                break;
            default:
                strcpy(code, "N\\A");
        }

        if(text)
        {
            printf("%d %s: %s\n", tk->line, code, text);
        }
        else if(type_int_value)
        {
            printf("%d %s: %d\n", tk->line, code, *type_int_value);
        }
        else if(type_double_value)
        {
            printf("%d %s: %f\n", tk->line, code, *type_double_value);
        }
        else
        {
            printf("%d %s\n", tk->line, code);
        }
    }
    free(text);
    free(type_double_value);
    free(type_int_value);
}
