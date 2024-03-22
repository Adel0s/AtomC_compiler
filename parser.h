#pragma once

#include <stdbool.h>
#include "lexer.h"

bool unit();
bool strucDef();
bool fnDef();
bool varDef();
bool typeBase();
bool arrayDecl();
bool fnParam();
bool stm();
bool stmCompound();
bool expr();
bool exprAssign();
bool exprOr();
bool exprOrPrim();
bool exprAnd();
bool exprAndPrim();
bool exprEq();
bool exprRel();
bool exprAdd();
bool exprMul();
bool exprCast();
bool exprUnary();
bool exprPostfix();
bool exprPrimary();
void parse(Token *tokens);
