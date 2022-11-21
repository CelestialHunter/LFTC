/*----------META-----------*/
// Author: Balea Andrei - Petru
// UPT Info AC 3 - 2022
// Limbaje Formale si Tehnici de Compilare

// This module implements the lexical analyzer (ALEX).
/*-------------------------*/

#ifndef ALEX_H
#define ALEX_H

/*---------INCLUDES--------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*-------------------------*/



/*---------DEFINES---------*/

#define MAX_ATOM_SIZE 10000

/*-------------------------*/



/*--------DATA TYPES-------*/

typedef enum {ID, 
    INT, REAL, STR, // constante
    VAR, FUNCTION, IF, ELSE, WHILE, END, 
        RETURN, TYPE_INT, TYPE_REAL, TYPE_STR, // cuvinte cheie
    COMMA, COLON, SEMICOLON, LPAR, RPAR, FINISH, // delimitatori
    ADD, SUB, MUL, DIV, AND, OR, NOT, 
        ASSIGN, EQUAL, NOTEQ, LESS} LEXIC; // operatori	

#ifndef LEXEMS
#define LEXEMS
extern const char* lexems[];
#endif

typedef struct {
	LEXIC code;
	union {
		long i;
		double r;
		char *s;
	} value;
	int line;
} Atom;

/*-------------------------*/



/*--------FUNCTIONS--------*/

extern void readFile(char*);

void printProgram();

void printAtom(Atom);

void addAtom(LEXIC, int, char*);

LEXIC getNextAtom(FILE*);

void unallocAtoms();

extern FILE* openFile(char*);

Atom* getAtomi();

int getAtomiLen();

/*-------------------------*/
#endif