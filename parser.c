#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "parser.h"

Token *iTk;		// the iterator in the tokens list
Token *consumedTk;		// the last consumed token

// afiseaza error in line... : textul de eroare si la sfarsit iese din program
void tkerr(const char *fmt,...){
	fprintf(stderr,"error in line %d: ",iTk->line);
	va_list va;
	va_start(va,fmt);
	vfprintf(stderr,fmt,va);
	va_end(va);
	fprintf(stderr,"\n");
	exit(EXIT_FAILURE);
	}

bool consume(int code){
	if(iTk->code==code){
		consumedTk=iTk;
		iTk=iTk->next;
		return true;
		}
	return false;
	}

// typeBase: TYPE_INT | TYPE_DOUBLE | TYPE_CHAR | STRUCT ID
// ATENTIE: totul sau nimic(daca functia consuma toata regula atunci va returna true)
// nu avem voie ca o functie sa cosume doar o parte din atomi
bool typeBase(){
    Token *start = iTk;
	if(consume(TYPE_INT)){
		return true;
		}
	if(consume(TYPE_DOUBLE)){
		return true;
		}
	if(consume(TYPE_CHAR)){
		return true;
		}
	if(consume(STRUCT)){
		if(consume(ID)){
			return true;
			}
		}
    iTk=start; //refacere pozitie initiala -> necesar cand e posibil sa nu se consume totul
	return false;
	}


bool structDef(){
    return true;
}

bool fnDef(){
    return true;
}

// arrayDecl: LBRACKET INT? RBRACKET
bool arrayDecl(){
    if(consume(LBRACKET)){
        if(consume(INT)){
            if(consume(RBRACKET)){
                return true;
            }
        }
        if(consume(RBRACKET)){
            return true;
        }
    }
    return false;
}

// varDef: typeBase ID arrayDecl? SEMICOLON
bool varDef(){
    if(typeBase()){
        if(consume(ID)){
            if(arrayDecl()){
                if(consume(SEMICOLON)){
                    return true;
                }
            }
            if(consume(SEMICOLON)){
                return true;
            }
        }
    }
    return false;
}
// daca e cu litere mare facem cosume fiind unn terminal
// unit: ( structDef | fnDef | varDef )* END
// fiecare regula va fi implementata cu cate o functie separata(una pt structDef, una pt fnDef, etc...), care nu vor avea parametrii si returneaza un bool
// true -> daca regula e indeplinita si false in caz contrar
bool unit(){
	for(;;){
		if(structDef()){}
		else if(fnDef()){}
		else if(varDef()){}
		else break;
		}
	if(consume(END)){
		return true;
		}
	return false;
	}

// recursivitate stanga => recursivitate infinita
// itk - iterator token
void parse(Token *tokens){
	iTk=tokens;
	if(!unit())tkerr("syntax error");
	}
