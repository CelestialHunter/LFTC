// Balea Andrei - Petru
// Informatica 3, AC UPT 2022

#include <stdio.h>
#include <string.h>

enum {ID, 
    INT, REAL, STR, // constante
    VAR, FUNCTION, IF, ELSE, WHILE, END, 
        RETURN, TYPE_INT, TYPE_REAL, TYLE_STR, // cuvinte cheie
    COMMA, COLON, SEMICOLON, LPAR, RPAR, FINISH, // delimitatori
    ADD, SUB, MUL, DIV, AND, OR, NOT, 
        ASSIGN, EQUAL, NOTEQ, LESS}; // operatori	

typedef struct{
	int cod;		// codul atomului ( ID, INT, ...)
	int linie;		// linia atomului respectiv
	union{
		char s[100];		// pentru ID-uri si stringuri
		double r;				// pentru real
		int i;						// pentru int
		};
	}Atom;
Atom atomi[10000];	// vectorul cu atomii extrasi din fisierul de intrare
int nAtomi;	// numarul de atomi din vectorul atomi

char bufin[30001];
char *pch;	// cursor la caracterul curent din bufin
int linie=1;		// linia curenta; adaugata automat la atom de addAtom

// la fiecare apel returneaza codul unui atom
int getNextTk()			// get next token (atom lexical)
{
	int state=0;		// starea curenta
	char buf[100];
	int n=0;		// nr caractere din buf
	for(;;){
		char ch=*pch;	// caracterul curent
		printf("#%d %c(%d)\n",state,ch,ch);	// pt debugging
		// cate un case pentru fiecare stare din diagrama
		switch(state){
			case 0:
				// testam toate tranzitiile posibile din acea stare
				if(isalpha(ch)||ch=='_'){state=1;pch++;buf[n++]=ch;}
				else if(isdigit(ch)){state=3;pch++;}
				else if(ch==' '||ch=='\r'||ch=='\n'||ch=='\t'){
						pch++;
						if(ch=='\n')linie++;
						}
				break;
			case 1:
				if(isalnum(ch)||ch=='_'){pch++;buf[n++]=ch;}
				else{state=2;}
				break;
			case 2:
				buf[n]='\0';	// incheiere sir
				if(strcmp(buf,"var")==0)addAtom(VAR);
				else if(strcmp(buf,"if`")==0)addAtom(IF);
				....	// toate cuvintele cheie
				else{
					addAtom(ID);	// id simplu
					strcpy(atomi[nAtomi-1].s,buf);
					}
				return ID;
			..........................
			default: printf("stare invalida %d\n",state);
			}
		}
	
}

int main()
{
	FILE *fis;
	fis=fopen("1.q","rb");
	if(fis==NULL){
		printf("nu s-a putut deschide fisierul");
		return -1;
		}
	int n=fread(bufin,1,30000,fis);	// returneaza nr de elemente citite integral
	bufin[n]='\0';
	fclose(fis);
	pch=bufin;	// initializare pch pe prima pozitie din bufin
	// extragere atomi
	while(getNextTk()!=FINISH){
		}
	// afisare atomi
	...
}