#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "parser.h"

Token *iTk;		// the iterator in the tokens list
Token *consumedTk;		// the last consumed token

// afiseaza error in line... : textul de eroare si la sfarsit iese din program
// mesaj de eroare cat mai specific: specificam ce lipseste si de unde lipseste. Ex: lipsa paranteza DESCHISA dupa IF
// if(<b) -> conditie if invalida
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
    printf("#typeBase: %d\n", iTk->code);
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
			}else tkerr("lipseste numele structurii!");
		}
    iTk=start; //refacere pozitie initiala -> necesar cand e posibil sa nu se consume totul
	return false;
}

// arrayDecl: LBRACKET INT? RBRACKET
bool arrayDecl(){
    printf("#arrayDecl: %d\n", iTk->code);
    if(consume(LBRACKET)){
        if(consume(INT)){
            if(consume(RBRACKET)){
                return true;
            }else(tkerr("lipseste ] din declararea array-ului"));
        }
        if(consume(RBRACKET)){
            return true;
        }else(tkerr("lipseste ] din declararea array-ului"));
    }else(tkerr("lipseste [ din declararea array-ului"));
    return false;
}

// varDef: typeBase ID arrayDecl? SEMICOLON
bool varDef(){
    printf("#varDef: %d\n", iTk->code);
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

// STRUCT ID LACC varDef* RACC SEMICOLON
bool structDef(){
    printf("#structDef: %d\n", iTk->code);
    if(consume(STRUCT)) {
        if(consume(ID)) {
            if(consume(LACC)) {
                for(;;) {
                    if(varDef()) {
                        if(consume(RACC)) {
                            if(consume(SEMICOLON)) {
                                return true;
                            }
                        }
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }

    return false;
}

// fnParam: typeBase ID arrayDecl?
bool fnParam() {
    printf("#fnParam: %d\n", iTk->code);
    if(typeBase()) {
        if(consume(ID)){
            if(arrayDecl()) {
                return true;
            }
            return true;
        }
    }
    return false;
}

// exprOr: exprOr OR exprAnd | exprAnd
// rezolvare recursivitate la stanga
// alfa1 = OR exprAnd     beta1 = exprAnd
// exprOr: exprAnd exprOrPrim
// exprOrPrim: OR exprAnd exprOrPrim | epsilon

bool exprOrPrim() {
    printf("#exprOrPrim: %d\n", iTk->code);
    if(consume(OR)) {
        if(exprAnd()) {
            if(exprOrPrim()) {
                return true;
            }
        }
    }
    return true; // epsilon
}

bool exprOr() {
    printf("#exprOr: %d\n", iTk->code);
    if(exprAnd()) {
        if(exprOrPrim()) {
            return true;
        }
    }
    return false;
}

// exprAnd: exprAnd AND exprEq | exprEq
// exprAnd -> exprEq exprAnd'
//exprAnd' -> AND exprEq exprAnd' | ϵ

bool exprAnd() {
    printf("#exprAnd: %d\n", iTk->code);
    if(exprEq()) {
        if(exprAndPrim()) {
            return true;
        }
    }
    return false;
}

bool exprAndPrim() {
    printf("#exprAndPrim: %d\n", iTk->code);
    if(consume(AND)) {
        if(exprEq()) {
            if(exprAndPrim()) {
                return true;
            }
        }
    }
    return true; //epsilon
}

// exprEq: exprEq ( EQUAL | NOTEQ ) exprRel | exprRel
// alfa1 = ( EQUAL | NOTEQ ) exprRel     beta1 = exprRel
// exprEq = exprRel exprEqPrim
// exprEqPrim = ( EQUAL | NOTEQ ) exprRel exprEqPrim | epsilon
bool exprEq() {
    printf("#exprEq: %d\n", iTk->code);
    if(exprRel()) {
        if(exprEqPrim()) {
            return true;
        }
    }
    return false;
}

bool exprEqPrim() {
    printf("#exprEqPrim: %d\n", iTk->code);
    if(consume(EQUAL)) {
        if(exprRel()) {
            if(exprEqPrim()) {
                return true;
            }
        }
    }
    else if(consume(NOTEQ)) {
        if(exprRel()) {
            if(exprEqPrim()) {
                return true;
            }
        }
    }
    return true; //epsilon
}

// exprAssign: exprUnary ASSIGN exprAssign | exprOr
bool exprAssign() {
    printf("#exprAssign: %d\n", iTk->code);
    if(exprUnary()) {
        if(consume(ASSIGN)) {
            if(exprAssign()){
                return true;
            }
            else if(exprOr()) {
                return true;
            }
        }
    }
    return false;
}

// exprRel: exprRel ( LESS | LESSEQ | GREATER | GREATEREQ ) exprAdd | exprAdd
// exprRel: exprAdd exprRelPrim
// exprRelPrim: (LESS | LESSEQ | GREATER | GREATEREQ) exprAdd exprRelPrim | epsilon
bool exprRel() {
    printf("#exprRel: %d\n", iTk->code);
    if(exprAdd()) {
        if(exprRelPrim()) {
            return true;
        }
    }
    return false;
}

bool exprRelPrim() {
    printf("#exprRelPrim: %d\n", iTk->code);
    // merge oare asa? SAU trebuie if(consume(LESS)) else if(consume(LESSEQ) ) else if(consume(GREATER))...
    if(consume(LESS) || consume(LESSEQ) || consume(GREATER) || consume(GREATEREQ)) {
        if(exprAdd()) {
            if(exprRelPrim()) {
                return true;
            }
        }
    }
    return true; // epsilon
}

// exprAdd: exprAdd ( ADD | SUB ) exprMul | exprMul
// exprAdd: exprMul exprAddPrim
// exprAddPrim: (ADD | SUB) exprMul exprAddPrim | epsilon
bool exprAdd() {
    printf("#exprAdd: %d\n", iTk->code);
    if(exprMul()) {
        if(exprAddPrim()) {
            return true;
        }
    }
    return false;
}

bool exprAddPrim() {
    printf("#exprAddPrim: %d\n", iTk->code);
    if(consume(ADD) || consume(SUB)) {
        if(exprMul()) {
            if(exprAddPrim()) {
                return true;
            }
        }
    }
    return true; // epsilon
}

// exprMul: exprMul ( MUL | DIV ) exprCast | exprCast eliminam recursivitatea la stanga
// alfa1 = ( MUL | DIV ) exprCast    beta1 = exprCast
// exprMul = beta1 exprMulPrim   --->   exprMul = exprCast exprMulPrim
// exprMulPrim = alfa1 exprMulPrim | epsilon   --->   exprMulPrim = ( MUL | DIV ) exprCast exprMulPrim | epsilon
bool exprMul() {
    printf("#exprMul: %d\n", iTk->code);
    if(exprCast()) {
        if(exprMulPrim()) {
            return true;
        }
    }
    return false;
}

bool exprMulPrim() {
    printf("#exprMulPrim: %d\n", iTk->code);
    if(consume(MUL) || consume(DIV)) {
        if(exprCast()) {
            if(exprMulPrim()) {
                return true;
            }
        }
    }
    return true; // epsilon
}

// exprCast: LPAR typeBase arrayDecl? RPAR exprCast | exprUnary
bool exprCast() {
    printf("#exprCast: %d\n", iTk->code);
    if(consume(LPAR)) {
        if(typeBase()) {
            if(arrayDecl()) {
                if(consume(RPAR)) {
                    if(exprCast()) {
                        return true;
                    }
                }
            }
            // arrayDecl? - tratare caz optionalitate
            if(consume(RPAR)) {
                if(exprCast()) {
                    return true;
                }
            }
        }
    }
    else if(exprUnary()) {
        return true;
    }
    return false;
}

// exprUnary: ( SUB | NOT ) exprUnary | exprPostfix
bool exprUnary() {
    printf("#exprUnary: %d\n", iTk->code);
    if(consume(SUB) || consume(NOT)) {
        if(exprUnary()) {
            return true;
        }
    }
    if(exprPostfix()) {
        return true;
    }
    return false;
}

// exprPostfix: exprPostfix LBRACKET expr RBRACKET
//      | exprPostfix DOT ID
//      | exprPrimary
// alfa1 = LBRACKET expr RBRACKET    alfa2 = DOT ID   beta1 = exprPrimary
// exprPostfix = beta1 exprPostfixPrim   --->   exprPrimary exprPosfixPrim
// exprPostfixPrim = alfa1 exprPostfixPrim | alfa2 exprPostfixPrim | epsilon
// exprPostfixPrim   --->   LBRACKET expr RBRACKET exprPostfixPrim | DOT ID exprPostfixPrim | epsilon

bool exprPostfix() {
    printf("#exprPostfix: %d\n", iTk->code);
    if(exprPrimary()) {
        if(exprPostfixPrim()) {
            return true;
        }
    }
    return false;
}

bool exprPostfixPrim() {
    printf("#exprPostfixPrim: %d\n", iTk->code);
    if(consume(LBRACKET)) {
        if(expr()) {
            if(consume(RBRACKET)) {
                if(exprPostfixPrim()) {
                    return true;
                }
            }
        }
    }
    if(consume(DOT)) {
        if(consume(ID)) {
            if(exprPostfixPrim()) {
                return true;
            }
        }
    }
    return true;
}

// exprPrimary: ID ( LPAR ( expr ( COMMA expr )* )? RPAR )?
//      | INT | DOUBLE | CHAR | STRING | LPAR expr RPAR
//   !!!   trebuie verificata logica   !!!
bool exprPrimary() {
    printf("#exprPrimary %d\n", iTk->code);
    if(consume(ID)) {
        if(consume(LPAR)) {
            if(expr()) {
                for(;;) {
                    if(consume(COMMA)) {
                        if(expr()) {}
                    }
                    else break;
                }
            }
            if(consume(RPAR)) {}
            return true;
        }
        else if(consume(INT) || consume(DOUBLE) || consume(CHAR) || consume(STRING)){}
        else if(consume(LPAR)) {
            if(expr()) {
                if(consume(RPAR)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// expr: exprAssign
bool expr() {
    printf("#expr: %d\n", iTk->code);
    if(exprAssign()) {
        return true;
    }
    return false;
}

// stm: stmCompound
//      | IF LPAR expr RPAR stm ( ELSE stm )?
//      | WHILE LPAR expr RPAR stm
//      | RETURN expr? SEMICOLON
//      | expr? SEMICOLON
bool stm() {
    printf("#stm: %d\n", iTk->code);
    if(stmCompound()){
        return true;
    }
    // | IF LPAR expr RPAR stm ( ELSE stm )?
    if(consume(IF)) {
        if(consume(LPAR)) {
            if(expr()) {
                if(consume(RPAR)) {
                    if(stm()){
                        if(consume(ELSE)) {
                            if(stm()){
                                return true;
                            }
                        }
                        return true;
                    }
                }
            }
        }
    }
    // | WHILE LPAR expr RPAR stm
    else if(consume(WHILE)) {
        if (consume(LPAR)) {
            if(expr()){
                if(consume(RPAR)) {
                    if(stm()) {
                        return true;
                    }
                }
            }
        }
    }
    // | RETURN expr? SEMICOLON
    else if(consume(RETURN)) {
        if(expr()){
            if(consume(SEMICOLON)){
                return true;
            }
        }
        if(consume(SEMICOLON)) return true;
    }
    // | expr? SEMICOLON
    else if(expr()) {
        if(consume(SEMICOLON)){
            return true;
        }
    }

    return false;
}

// stmCompound: LACC ( varDef | stm )* RACC
bool stmCompound() {
    printf("#stmCompound: %d\n", iTk->code);
    if(consume(LACC)) {
        for(;;){
            if(varDef()){}
            else if(stm()){}
            else break;
        }
        if(consume(RACC)){
            return true;
        }
    }
    return false;
}

// fnDef: ( typeBase | VOID ) ID
//               LPAR ( fnParam ( COMMA fnParam )* )? RPAR
//               stmCompound
bool fnDef(){
    printf("#fnDef: %d\n", iTk->code);
    if(typeBase()) {

    }
    else if(consume(VOID)) {

    }
    if(consume(ID)) {
        if(consume(LPAR)) {
            if(fnParam()){
                for(;;) {
                    if(consume(COMMA)) {
                        if(fnParam()) {

                        }
                        else break;
                    }
                }
            }
            if(consume(RPAR)) {
                if(stmCompound()) {
                    return true;
                }
            }
        }
    }
    return false;
}


// daca e cu litere mari facem cosume fiind un terminal
// unit: ( structDef | fnDef | varDef )* END
// fiecare regula va fi implementata cu cate o functie separata(una pt structDef, una pt fnDef, etc...), care nu vor avea parametrii si vor returna un bool
// true -> daca regula e indeplinita si false in caz contrar
bool unit(){
    printf("#unit: %d\n", iTk->code);
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
