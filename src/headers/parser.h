#pragma once

#include <stdbool.h>
#include "lexer.h"
#include "ad.h"
#include "at.h"

bool unit();
bool structDef();
bool fnDef();
bool varDef();
bool typeBase(Type *);
bool arrayDecl(Type *);
bool fnParam();
bool stm();
bool stmCompound(bool);
bool expr(Ret *);
bool exprAssign(Ret *);
bool exprOr(Ret *);
bool exprOrPrim(Ret *);
bool exprAnd(Ret *);
bool exprAndPrim(Ret *);
bool exprEq(Ret *);
bool exprEqPrim(Ret *);
bool exprRel(Ret *);
bool exprRelPrim(Ret *);
bool exprAdd(Ret *);
bool exprAddPrim(Ret *);
bool exprMul(Ret *);
bool exprMulPrim(Ret *);
bool exprCast(Ret *);
bool exprUnary(Ret *);
bool exprPostfix(Ret *);
bool exprPostfixPrim(Ret *);
bool exprPrimary(Ret *);
void parse(Token *tokens);
