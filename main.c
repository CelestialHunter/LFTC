/*----------META-----------*/
// Author: Balea Andrei - Petru
// UPT Info AC 3 - 2022
// Limbaje Formale si Tehnici de Compilare

// This is the main program.
/*-------------------------*/

#include "lftc/alex.h"

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
    
	FILE* f = openFile(filename);
	int i;
	int last_line = 0;
	while(getNextAtom(f) != FINISH) {}

    Atom* atomi = getAtomi();

	for(i = 0; i < getAtomiLen(); i++)
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