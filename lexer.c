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

char *tkCodeName(int code) {
    switch(code) {
        case ID:
            return "ID";
        case TYPE_INT:
            return "TYPE_INT";
        case TYPE_CHAR:
            return "TYPE_CHAR";
        case TYPE_DOUBLE:
            return "TYPE_DOUBLE";
        case ELSE:
            return "ELSE";
        case IF:
            return "IF";
        case RETURN:
            return "RETURN";
        case STRUCT:
            return "STRUCT";
        case VOID:
            return "VOID";
        case WHILE:
            return "WHILE";
        case COMMA:
            return "COMMA";
        case SEMICOLON:
            return "SEMICOLON";
        case LPAR:
            return "LPAR";
        case RPAR:
            return "RPAR";
        case LBRACKET:
            return "LBRACKET";
        case RBRACKET:
            return "RBRACKET";
        case LACC:
            return "LACC";
        case RACC:
            return "RACC";
        case END:
            return "END";
        case ADD:
            return "ADD";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case DOT:
            return "DOT";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case NOT:
            return "NOT";
        case NOTEQ:
            return "NOTEQ";
        case LESS:
            return "LESS";
        case LESSEQ:
            return "LESSEQ";
        case GREATER:
            return "GREATER";
        case GREATEREQ:
            return "GREATEREQ";
        case ASSIGN:
            return "ASSIGN";
        case EQUAL:
            return "EQUAL";
        case SUB:
            return "SUB";
        case INT:
            return "INT";
        case DOUBLE:
            return "DOUBLE";
        case CHAR:
            return "CHAR";
        case STRING:
            return "STRING";
        default:
            return "N\\A";
    }
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

    // printf("%s\n", extracted_text);

    return extracted_text;
}

void *remove_quotes(char *text)
{
    int i, j;
    size_t len = strlen(text);


    for(i = 0, j = 0; i < len-1; i++)
    {
        if(text[i] != '"')
        {
            text[j++] = text[i];
        }
    }

    text[j] = '\0';
}

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
            case '/':
                if(pch[1] == '/')  ///vezi cum implemenetezi pentru linecomment sa mearga pe mai multe platforme
                {
                    pch++;
                    for(start=pch++; *pch!='\n'; pch++) {}
                    break;
                }
                else{
                    addTk(DIV);
                    pch++;
                    break;
                }
            case '.':
                if(isdigit(pch[1]) == 0)
                {
                    addTk(DOT);
                    pch++;
                }
                break;
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
            case '\'':
                if(pch[1] != '\'' && pch[2] == '\'')
                {
                    tk=addTk(CHAR);
                    tk->c = pch[1];
                    pch += 3;
                }
                else
                {
                    err("invalid char: %c (%d)",*pch,*pch);
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
                // de completat pentru double sa permita si scrierea cu e sau E
                // DOUBLE: [0-9]+ ( '.' [0-9]+ ( [eE] [+-]? [0-9]+ )? | ( '.' [0-9]+ )? [eE] [+-]? [0-9]+ )
                else if(isdigit(*pch))
                {
                    for(start=pch++; isdigit(*pch) || *pch=='.' ||
                                     ((*pch=='e' || *pch=='E') && (*(pch+1)=='-' || (*(pch+1)=='+' || isdigit(*(pch+1)) ||
                                             isdigit(*(pch-1))) )) ||
                                     ((*(pch-1)=='e' || *(pch-1)=='E') && (*pch=='-' || (*pch=='+' || isdigit(*pch) ) )); pch++) {}
                    char *text = extract(start, pch);
                    char *text_to_double = NULL;
                    // convert text to double
                    double value = strtod(text, &text_to_double);

                    if(strchr(text, '.') || strchr(text, 'e') || strchr(text, 'E') || strchr(text, '-') || strchr(text, '+'))
                    {
                        tk=addTk(DOUBLE);
                        tk->d=value;
                    }
                    // INT: [0-9]+
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
                    pch++;
                    char *text = extract(start, pch);
                    printf("%s %d\n\n", text, strlen(text));

                    // We should remove " characters
                    remove_quotes(text);
                    tk=addTk(STRING);
                    tk->text=text;
                }
                else err("invalid char: %c (%d)",*pch,*pch);
        }
    }
}

void showTokens(const Token *tokens)
{
    FILE* fout = fopen("lista-de-atomi-copy.txt", "w"); //opening file for write

    const Token *tk;
    char code[12];

    char *text;
    int *type_int_value;
    double *type_double_value;
    char c = '\0';

    for(tk=tokens; tk; tk=tk->next)
    {
        size_t length = 0;

        text = NULL;
        type_int_value = NULL;
        type_double_value = NULL;
        c = '\0';

        switch(tk->code)
        {
            case ID:
                strcpy(code, "ID");
                length = strlen(tk->text);
                text = (char*)safeAlloc((size_t)length+1);
                strcpy(text, tk->text);
                break;
            case TYPE_INT:
                strcpy(code, "TYPE_INT");
                break;
            case TYPE_CHAR:
                strcpy(code, "TYPE_CHAR");
                break;
            case TYPE_DOUBLE:
                strcpy(code, "TYPE_DOUBLE");
                break;
            case ELSE:
                strcpy(code, "ELSE");
                break;
            case IF:
                strcpy(code, "IF");
                break;
            case RETURN:
                strcpy(code, "RETURN");
                break;
            case STRUCT:
                strcpy(code, "STRUCT");
                break;
            case VOID:
                strcpy(code, "VOID");
                break;
            case WHILE:
                strcpy(code, "WHILE");
                break;
            case COMMA:
                strcpy(code, "COMMA");
                break;
            case SEMICOLON:
                strcpy(code, "SEMICOLON");
                break;
            case LPAR:
                strcpy(code, "LPAR");
                break;
            case RPAR:
                strcpy(code, "RPAR");
                break;
            case LBRACKET:
                strcpy(code, "LBRACKET");
                break;
            case RBRACKET:
                strcpy(code, "RBRACKET");
                break;
            case LACC:
                strcpy(code, "LACC");
                break;
            case RACC:
                strcpy(code, "RACC");
                break;
            case END:
                strcpy(code, "END");
                break;
            case ADD:
                strcpy(code, "ADD");
                break;
            case MUL:
                strcpy(code, "MUL");
                break;
            case DIV:
                strcpy(code, "DIV");
                break;
            case DOT:
                strcpy(code, "DOT");
                break;
            case AND:
                strcpy(code, "AND");
                break;
            case OR:
                strcpy(code, "OR");
                break;
            case NOT:
                strcpy(code, "NOT");
                break;
            case NOTEQ:
                strcpy(code, "NOTEQ");
                break;
            case LESS:
                strcpy(code, "LESS");
                break;
            case LESSEQ:
                strcpy(code, "LESSEQ");
                break;
            case GREATER:
                strcpy(code, "GREATER");
                break;
            case GREATEREQ:
                strcpy(code, "GREATEREQ");
                break;
            case ASSIGN:
                strcpy(code, "ASSIGN");
                break;
            case EQUAL:
                strcpy(code, "EQUAL");
                break;
            case SUB:
                strcpy(code, "SUB");
                break;
            case INT:
                strcpy(code, "INT");
                type_int_value = (int*)safeAlloc((size_t)sizeof(int));
                *type_int_value = tk->i;
                break;
            case DOUBLE:
                strcpy(code, "DOUBLE");
                type_double_value = (double*)safeAlloc((size_t)sizeof(double));
                *type_double_value = tk->d;
                break;
            case CHAR:
                strcpy(code, "CHAR");
                c = tk->c;
                break;
            case STRING:
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
            printf("%d \t%s: %s\n", tk->line, code, text);
            fprintf(fout, "%d \t%s: %s\n", tk->line, code, text);
        }
        else if(type_int_value)
        {
            printf("%d \t%s: %d\n", tk->line, code, *type_int_value);
            fprintf(fout, "%d \t%s: %d\n", tk->line, code, *type_int_value);
        }
        else if(type_double_value)
        {
            printf("%d \t%s: %f\n", tk->line, code, *type_double_value);
            fprintf(fout, "%d \t%s: %f\n", tk->line, code, *type_double_value);
        }
        else if(c != '\0')
        {
            printf("%d \t%s: %c\n", tk->line, code, c);
            fprintf(fout, "%d \t%s: %c\n", tk->line, code, c);
        }
        else
        {
            printf("%d \t%s\n", tk->line, code);
            fprintf(fout, "%d \t%s\n", tk->line, code);
        }
    }
    free(text);
    free(type_double_value);
    free(type_int_value);

    fclose(fout);
}
