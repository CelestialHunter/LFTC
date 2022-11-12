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
        RETURN, TYPE_INT, TYPE_REAL, TYPE_STR, // cuvinte cheie
    COMMA, COLON, SEMICOLON, LPAR, RPAR, FINISH, // delimitatori
    ADD, SUB, MUL, DIV, AND, OR, NOT, 
        ASSIGN, EQUAL, NOTEQ, LESS} LEXIC; // operatori	

char* lexems[] = {"ID", 
	"INT", "REAL", "STR", // constante
	"VAR", "FUNCTION", "IF", "ELSE", "WHILE", "END", 
		"RETURN", "TYPE_INT", "TYPE_REAL", "TYPE_STR", // cuvinte cheie
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
	printf("%s", lexems[a.code]);
	if (a.code == INT) printf(":%ld", a.value.i);
	if (a.code == REAL) printf(":%lf", a.value.r);
	if (a.code == STR || a.code == ID) printf(":%s", a.value.s);

	printf(" ");
}

Atom atomi[MAX_ATOM_SIZE];
int atomi_len = 0;
int line_count = 1;

int consume = 1;
char lastChar = ' ';

void addAtom(LEXIC code, int line, char* word) {
	Atom a;
	a.line = line;
	a.code = code;
	if (code == INT) a.value.i = atol(word);
	if (code == REAL) a.value.r = atof(word);

	if (code == ID)
	{
		char *s = (char*)malloc(strlen(word) + 1);
		strcpy(s, word);
		for(int i = 0; i < strlen(s); i++)
			s[i] = toupper(s[i]);
		for(int i = (int)VAR; i <= (int)TYPE_STR; i++)
		{
			if(strcmp(s, lexems[i]) == 0)
			{
				a.code = code = (LEXIC)i;
				break;
			}
		}
		if(strcmp(s, "INT") == 0)
			a.code = code = TYPE_INT;
		if(strcmp(s, "REAL") == 0)
			a.code = code = TYPE_REAL;
		if(strcmp(s, "STR") == 0)
			a.code = code = TYPE_STR;

		free(s);
	}

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
	char ch = lastChar;
	Atom* atom;

	int firstCall = 1;

	while(1)
	{
		if(consume)
		{
			ch = fgetc(fin);
		} 
		else
		{
			ch = lastChar;
		}

		if(firstCall)
		{
			firstCall = 0;
			consume = 1;
		}

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
				else if(ch == ',') stare = 8, consume = 0;
				else if(ch == ':') stare = 9, consume = 0;
				else if(ch == ';') stare = 10, consume = 0;
				else if(ch == '(') stare = 11, consume = 0;
				else if(ch == ')') stare = 12, consume = 0;
				else if(ch == EOF || ch == '\0') stare = 13, consume = 0;
				else if(ch == '+') stare = 14, consume = 0;
				else if(ch == '-') stare = 15, consume = 0;
				else if(ch == '*') stare = 16, consume = 0;
				else if(ch == '/') stare = 17, consume = 0;
				else if(ch == '&') stare = 18;
				else if(ch == '|') stare = 20;
				else if(ch == '!') stare = 22;
				else if(ch == '=') stare = 25, consume = 0;
				else if(ch == '<') stare = 28, consume = 0;
				else if(ch =='\"') stare = 29;
				else if((ch == '\n' && lastChar != '\r') || ch == '\r') stare = 0, ++line_count; // windows file format... \r\n
				else if(ch == '#') stare = 31;
				else if(ch == ' ') consume = 1;
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
					consume = 0;
				}
				break;
			case 2:
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
					consume = 0;
				}
				break;
			case 5:
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
					consume = 0;
				}
				break;
			case 7:
				word[wordLen] = 0;
				addAtom(ID, line_count, word);
				return ID;
				break;
			case 8:
				addAtom(COMMA, line_count, word);
				consume = 1;
				return COMMA;
				break;
			case 9:
				addAtom(COLON, line_count, word);
				consume = 1;
				return COLON;
				break;
			case 10:
				addAtom(SEMICOLON, line_count, word);
				consume = 1;
				return SEMICOLON;
				break;
			case 11:
				addAtom(LPAR, line_count, word);
				consume = 1;
				return LPAR;
				break;
			case 12:
				addAtom(RPAR, line_count, word);
				consume = 1;
				return RPAR;
				break;
			case 13:
				addAtom(FINISH, ++line_count, word);
				consume = 1;
				return FINISH;
				break;
			case 14:
				addAtom(ADD, line_count, word);
				consume = 1;
				return ADD;
				break;
			case 15:
				addAtom(SUB, line_count, word);
				consume = 1;
				return SUB;
				break;
			case 16:
				addAtom(MUL, line_count, word);
				consume = 1;
				return MUL;
				break;
			case 17:
				addAtom(DIV, line_count, word);
				consume = 1;
				return DIV;
				break;
			case 18:
				if(ch == '&')
				{
					word[wordLen++] = ch;
					stare = 19;
					consume = 0;
				}
				else
				{
					// ERROR - & expected
				}
				break;
			case 19:
				addAtom(AND, line_count, word);
				consume = 1;
				return AND;
				break;
			case 20:
				if(ch == '|')
				{
					word[wordLen++] = ch;
					stare = 21;
					consume = 0;
				}
				else
				{
					// ERROR - | expected
				}
				break;
			case 21:
				addAtom(OR, line_count, word);
				consume = 1;
				return OR;
				break;
			case 22:
				if(ch == '=')
				{
					word[wordLen++] = ch;
					stare = 24;
					consume = 0;
				}
				else
				{
					stare = 23;
					consume = 0;
				}
				break;
			case 23:
				addAtom(NOT, line_count, word);
				consume = 1;
				return NOT;
				break;
			case 24:
				addAtom(NOTEQ, line_count, word);
				consume = 1;
				return NOTEQ;
				break;
			case 25:
				if(ch == '=')
				{
					word[wordLen++] = ch;
					stare = 27;
					consume = 0;
				}
				else
				{
					stare = 26;
					consume = 0;
				}
				break;
			case 26:
				addAtom(ASSIGN, line_count, word);
				consume = 1;
				return ASSIGN;
				break;
			case 27:
				addAtom(EQUAL, line_count, word);
				consume = 1;
				return EQUAL;
				break;
			case 28:
				addAtom(LESS, line_count, word);
				consume = 1;
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
					consume = 0;
				}
				else
				{
					// ERROR - " expected
				}
				break;			
			case 30:
				addAtom(STR, line_count, word);
				consume = 1;
				return STR;
				break;
			case 31:
				if(ch != '\n' && ch != '\r' && ch != '\t' && ch != '\0')
				{
					stare = 31;
				}
				else if(ch == '\0')
				{
					return FINISH;
				}
				else 
				{
					stare = 0;
					consume = 1;
					line_count++;
				}
				break;
		}
		lastChar = ch;
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

int main(int argc, char* argv[]) {
	char filename[100];
	if(argc == 2)
	{
		strcpy(filename, argv[1]);
	}
	else
	{
		printf("Introduceti numele fisierului: ");
		scanf("%s", filename);
	}
	FILE* f = open_file(filename);
	int i;
	int last_line = 0;
	while(getNextAtom(f) != FINISH)
	{
	}

	for(i = 0; i < atomi_len; i++)
	{
		if(atomi[i].line != last_line)
		{
			printf("\nLine #%d: ", atomi[i].line);
			last_line = atomi[i].line;
		}
		printAtom(atomi[i]);
		
	}

	printf("\n");

	unallocAtoms();
}