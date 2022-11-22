/*----------META-----------*/
// Author: Balea Andrei - Petru
// UPT Info AC 3 - 2022
// Limbaje Formale si Tehnici de Compilare

// This is the main program.
/*-------------------------*/

#include "lftc/lftc.h"

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

	printf("Reading file...\n");
	readFile(filename);
	printf("\nFile read!\n");
	printf("############################\n");
	printf("\nLexical analysis...\n");
	printProgram(); 
	printf("\nLexical analysis done!\n");
	printf("############################\n");

	printf("Syntax analysis...\n");
	initAsin();

	if(program())
	{
		printf("Programul este corect sintactic!\n");
	}
	  
	printf("############################\n");

	unallocAtoms();

	return 0;
}