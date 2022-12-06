/*----------META-----------*/
// Author: Balea Andrei - Petru
// UPT Info AC 3 - 2022
// Limbaje Formale si Tehnici de Compilare

// This module implements the syntax analyzer (ASIN).
/*-------------------------*/

#ifndef ASIN_H
#define ASIN_H

/*---------INCLUDES--------*/

#include "alex.h"
#include "ad.h"
#include "at.h"

/*-------------------------*/



/*--------DATA TYPES-------*/
/*-------------------------*/



/*--------FUNCTIONS--------*/
int consumeAtom(LEXIC);
extern void initAsin();
extern int program();
extern void printError(char*, int);
int defVar();
int baseType();
int defFunc();
int block();
int funcParams();
int funcParam();
int instr();
int expr();
int exprLogic();
int exprAssign();
int exprComp();
int exprAdd();
int exprMul();
int exprPrefix();
int factor();

/*-------------------------*/

#endif