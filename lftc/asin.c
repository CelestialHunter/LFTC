#include "asin.h"

/*----------DATA-----------*/
Atom* currentAtom;
Atom* consumedAtom;
/*-------------------------*/

/*--------FUNCTIONS--------*/
int consumeAtom(LEXIC code) {
    if (currentAtom->code == code) {
        consumedAtom = currentAtom++;
        return 1;
    }
    return 0;
}
/*-------------------------*/