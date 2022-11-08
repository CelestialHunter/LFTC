// Balea Andrei - Petru
// Informatica 3, AC UPT 2022

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_ATOM_SIZE 10000

typedef enum {ID, 
    INT, REAL, STR, // constante
    VAR, FUNCTION, IF, ELSE, WHILE, END, 
        RETURN, TYPE_INT, TYPE_REAL, TYLE_STR, // cuvinte cheie
    COMMA, COLON, SEMICOLON, LPAR, RPAR, FINISH, // delimitatori
    ADD, SUB, MUL, DIV, AND, OR, NOT, 
        ASSIGN, EQUAL, NOTEQ, LESS} LEXIC; // operatori	

char* lexems[] = {"ID", 
	"INT", "REAL", "STR", // constante
	"VAR", "FUNCTION", "IF", "ELSE", "WHILE", "END", 
		"RETURN", "TYPE_INT", "TYPE_REAL", "TYLE_STR", // cuvinte cheie
	"COMMA", "COLON", "SEMICOLON", "LPAR", "RPAR", "FINISH", // delimitatori
	"ADD", "SUB", "MUL", "DIV", "AND", "OR", "NOT", 
		"ASSIGN", "EQUAL", "NOTEQ", "LESS"}; // operatori

typedef struct {
	LEXIC code;
	union {
		long i;
		double r;
		char *s;
	} value;
	int line;
} Atom;

void printAtom(Atom a) {
	printf("Line #%d: %s", a.line, lexems[a.code]);
	if (a.code == INT) printf(":%ld", a.value.i);
	if (a.code == REAL) printf(":%lf", a.value.r);
	if (a.code == STR || a.code == ID) printf(":%s", a.value.s);
	printf("\n");
}

Atom atomi[MAX_ATOM_SIZE];
int atomi_len = 0;
int line_count = 0;

void addAtom(LEXIC code, int line, char* word) {
	Atom a;
	a.code = code;
	a.line = line;
	if (code == INT) a.value.i = atol(word);
	if (code == REAL) a.value.r = atof(word);
	if (code == STR || code == ID)
	{
		a.value.s = (char*)malloc(strlen(word) + 1);
		strcpy(a.value.s, word);
		a.value.s[strlen(word)] = '\0';
	}
	atomi[atomi_len++] = a;
}

LEXIC getNextAtom(FILE* fin)
{
	int stare = 0;
	char word[100] = {0};
	int wordLen = 0;
	char ch;
	Atom* atom;

	while(1)
	{
		ch = fgetc(fin);
		switch(stare)
		{
			case 0:
				if(isdigit(ch))
				{
					word[wordLen++] = ch;
					stare = 1;
				}
				else if(isalpha(ch) || ch == '_')
				{
					word[wordLen++] = ch;
					stare = 6;
				}
				else if(ch == ',') stare = 8;
				else if(ch == ':') stare = 9;
				else if(ch == ';') stare = 10;
				else if(ch == '(') stare = 11;
				else if(ch == ')') stare = 12;
				else if(ch == EOF || ch == '\0') stare = 13;
				else if(ch == '+') stare = 14;
				else if(ch == '-') stare = 15;
				else if(ch == '*') stare = 16;
				else if(ch == '/') stare = 17;
				else if(ch == '&') stare = 18;
				else if(ch == '|') stare = 20;
				else if(ch == '!') stare = 22;
				else if(ch == '=') stare = 25;
				else if(ch == '<') stare = 28;
				else if(ch =='\"') stare = 29;
				else if(ch == '\n' || ch == '\t') stare = 0, line_count++;
				else if(ch == '#') stare = 31;
				break;
			case 1:
				if(isdigit(ch))
				{
					word[wordLen++] = ch;
					stare = 1;
				}
				else if(ch == '.')
				{
					word[wordLen++] = ch;
					stare = 3;
				}
				else
				{
					stare = 2;
				}
				break;
			case 2:
				ungetc(ch, fin);
				word[wordLen] = 0;
				addAtom(INT, line_count, word);
				return INT;
				break;
			case 3:
				if(isdigit(ch))
				{
					word[wordLen++] = ch;
					stare = 4;
				}
				else
				{
					// ERROR - Digit expected
				}
				break;
			case 4:
				if(isdigit(ch))
				{
					word[wordLen++] = ch;
					stare = 4;
				}
				else
				{
					stare = 5;
				}
				break;
			case 5:
				ungetc(ch, fin);
				word[wordLen] = 0;
				addAtom(REAL, line_count, word);
				return REAL;
				break;
			case 6:
				if(isalpha(ch) || isdigit(ch) || ch == '_')
				{
					word[wordLen++] = ch;
					stare = 6;
				}
				else
				{
					stare = 7;
				}
				break;
			case 7:
				ungetc(ch, fin);
				word[wordLen] = 0;
				addAtom(ID, line_count, word);
				return ID;
				break;
			case 8:
				ungetc(ch, fin);
				addAtom(COMMA, line_count, word);
				return COMMA;
				break;
			case 9:
				ungetc(ch, fin);
				addAtom(COLON, line_count, word);
				return COLON;
				break;
			case 10:
				ungetc(ch, fin);
				addAtom(SEMICOLON, line_count, word);
				return SEMICOLON;
				break;
			case 11:
				ungetc(ch, fin);
				addAtom(LPAR, line_count, word);
				return LPAR;
				break;
			case 12:
				ungetc(ch, fin);
				addAtom(RPAR, line_count, word);
				return RPAR;
				break;
			case 13:
				ungetc(ch, fin);
				addAtom(FINISH, line_count, word);
				return FINISH;
				break;
			case 14:
				ungetc(ch, fin);
				addAtom(ADD, line_count, word);
				return ADD;
				break;
			case 15:
				ungetc(ch, fin);
				addAtom(SUB, line_count, word);
				return SUB;
				break;
			case 16:
				ungetc(ch, fin);
				addAtom(MUL, line_count, word);
				return MUL;
				break;
			case 17:
				ungetc(ch, fin);
				addAtom(DIV, line_count, word);
				return DIV;
				break;
			case 18:
				if(ch == '&')
				{
					word[wordLen++] = ch;
					stare = 19;
				}
				else
				{
					// ERROR - & expected
				}
				break;
			case 19:
				ungetc(ch, fin);
				addAtom(AND, line_count, word);
				return AND;
				break;
			case 20:
				if(ch == '|')
				{
					word[wordLen++] = ch;
					stare = 21;
				}
				else
				{
					// ERROR - | expected
				}
				break;
			case 21:
				ungetc(ch, fin);
				addAtom(OR, line_count, word);
				return OR;
				break;
			case 22:
				if(ch == '=')
				{
					word[wordLen++] = ch;
					stare = 24;
				}
				else
				{
					stare = 23;
				}
				break;
			case 23:
				ungetc(ch, fin);
				addAtom(NOT, line_count, word);
				return NOT;
				break;
			case 24:
				ungetc(ch, fin);
				addAtom(NOTEQ, line_count, word);
				return NOTEQ;
				break;
			case 25:
				if(ch == '=')
				{
					word[wordLen++] = ch;
					stare = 27;
				}
				else
				{
					stare = 26;
				}
				break;
			case 26:
				ungetc(ch, fin);
				addAtom(ASSIGN, line_count, word);
				return ASSIGN;
				break;
			case 27:
				ungetc(ch, fin);
				addAtom(EQUAL, line_count, word);
				return EQUAL;
				break;
			case 28:
				ungetc(ch, fin);
				addAtom(LESS, line_count, word);
				return LESS;
				break;
			case 29:
				if(ch != '\"')
				{					
					word[wordLen++] = ch;					
					stare = 29;
				}
				else if (ch == '\"')
				{
					stare = 30;
				}
				else
				{
					// ERROR - " expected
				}
				break;			
			case 30:
				ungetc(ch, fin);
				addAtom(STR, line_count, word);
				return STR;
				break;
			case 31:
				if(ch != '\n' && ch != '\0')
				{
					stare = 31;
				}
				else if(stare == '\0')
				{
					return FINISH;
				}
				else 
				{
					stare = 0;
				}
		}
	}
}

void unallocAtoms()
{
	int i;
	for(i = 0; i < atomi_len; i++)
	{
		if(atomi[i].code == STR || atomi[i].code == ID)
		{
			free(atomi[i].value.s);
		}
	}
}

FILE* open_file(char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		printf("Eroare la deschiderea fisierului %s\n", filename);
		exit(1);
	}
	return f;
}

int main() {
	FILE* f = open_file("1.q");
	int i;
	while(getNextAtom(f) != FINISH)
	{
	}

	for(i = 0; i < atomi_len; i++)
	{
		printAtom(atomi[i]);
	}

	unallocAtoms();
}