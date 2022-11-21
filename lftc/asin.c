#include "asin.h"

/*----------DATA-----------*/
Atom* currentAtom;
Atom* consumedAtom;

char error[100];
/*-------------------------*/

/*--------FUNCTIONS--------*/
int consumeAtom(LEXIC code) {
    if (currentAtom->code == code) {
        consumedAtom = currentAtom++;
        return 1;
    }
    return 0;
}

void initAsin() {
    currentAtom = getAtomi();
    consumedAtom = NULL;
}

int program() 
{
    while(1)
    {
        if(defVar())
        {
            continue;
        }
        if(defFunc())
        {
            continue;
        }
        if(block())
        {
            continue;
        }
        break;
    }
    if(consumeAtom(FINISH))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int defVar()
{
    if(consumeAtom(VAR))
    {
        if(consumeAtom(ID))
        {
            if(consumeAtom(COLON))
            {
                if(baseType())
                {
                    if(consumeAtom(SEMICOLON))
                    {
                        return 1;
                    }
                    else 
                    {
                        // error: SEMICOLON expected
                        printf("Error: SEMICOLON [ \" ; \" ] expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else 
                {
                    // error: baseType expected
                    printf("Error: valid baseType expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else 
            {
                // error: COLON expected
                printf("Error: COLON [ \" : \" ] expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else 
        {
            // error: ID expected
            printf("Error: ID expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

int baseType()
{
    if(consumeAtom(TYPE_INT))
    {
        return 1;
    }
    if(consumeAtom(TYPE_REAL))
    {
        return 1;
    }
    if(consumeAtom(TYPE_STR))
    {
        return 1;
    }
    // error: baseType expected
    printf("Error: valid baseType expected at line %d\n", currentAtom->line);
    return 0;
}

int defFunc()
{
    if(consumeAtom(FUNCTION))
    {
        if(consumeAtom(ID))
        {
            if(consumeAtom(LPAR))
            {
                if(funcParams())
                {
                    if(consumeAtom(RPAR))
                    {
                        if(consumeAtom(COLON))
                        {
                            if(baseType())
                            {
                                while(1)
                                {
                                    if(defVar())
                                    {
                                        continue;
                                    }
                                    break;
                                }
                                if(block())
                                {
                                    if(consumeAtom(END))
                                    {
                                        return 1;
                                    }
                                    else 
                                    {
                                        // error: END expected
                                        printf("Error: END expected at line %d\n", currentAtom->line);
                                        return 0;
                                    }
                                }
                                else 
                                {
                                    // error: block expected
                                    printf("Error: block expected at line %d\n", currentAtom->line);
                                    return 0;
                                }
                            }
                            else 
                            {
                                // error: baseType expected
                                printf("Error: valid baseType expected at line %d\n", currentAtom->line);
                                return 0;
                            }
                        }
                        else 
                        {
                            // error: COLON expected
                            printf("Error: COLON [ \" : \" ] expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    else 
                    {
                        // error: RPAR expected
                        printf("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else 
                {
                    // error: funcParams expected
                    printf("Error: valid funcParams expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else 
            {
                // error: LPAR expected
                printf("Error: LPAR [ \" ( \" ] expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else 
        {
            // error: ID expected
            printf("Error: ID expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    return 0;
}

int block()
{
    if(instr())
    {
        while(1)
        {
            if(instr())
            {
                continue;
            }
            break;
        }
        return 1;
    }
    printf("Error: valid instruction expected at line %d\n", currentAtom->line);
    return 0;
}

int funcParams()
{
    funcParam();
    while(1)
    {
        if(consumeAtom(COMMA))
        {
            if(funcParam())
            {
                continue;
            }
            else
            {
                // error: funcParam expected
                printf("Error: valid funcParam expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        break;
    }
    return 1;
}

int funcParam()
{
    if(consumeAtom(ID))
    {
        if(consumeAtom(COLON))
        {
            if(baseType())
            {
                return 1;
            }
            else 
            {
                // error: baseType expected
                printf("Error: valid baseType expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else 
        {
            // error: COLON expected
            printf("Error: COLON [ \" : \" ] expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    printf("Error: ID expected at line %d\n", currentAtom->line);
    return 0;
}

int instr()
{
    if (expr())
        if(!consumeAtom(SEMICOLON))
        {
            printf("Error: SEMICOLON [ \" ; \" ] expected at line %d\n", currentAtom->line);
            return 0;
        }
        else return 1;
    if(consumeAtom(IF))
    {
        if(consumeAtom(LPAR))
        {
            if(expr())
            {
                if(consumeAtom(RPAR))
                {
                    if(block())
                    {
                        if(consumeAtom(ELSE))
                        {
                            if(block())
                            {
                                if(consumeAtom(END))
                                {
                                    return 1;
                                }
                                else 
                                {
                                    // error: END expected
                                    printf("Error: END expected at line %d\n", currentAtom->line);
                                    return 0;
                                }
                            }
                            else 
                            {
                                // error: block expected
                                printf("Error: valid block expected at line %d\n", currentAtom->line);
                                return 0;
                            }
                        }
                        if(consumeAtom(END))
                        {
                            return 1;
                        }
                        else 
                        {
                            // error: END expected
                            printf("Error: END expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    else 
                    {
                        // error: block expected
                        printf("Error: valid block expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else 
                {
                    // error: RPAR expected
                    printf("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else 
            {
                // error: expr expected
                printf("Error: valid expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else 
        {
            // error: LPAR expected
            printf("Error: LPAR [ \" ( \" ] expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    if(consumeAtom(RETURN))
    {
        if(expr())
        {
            if(consumeAtom(SEMICOLON))
            {
                return 1;
            }
            else 
            {
                // error: SEMICOLON expected
                printf("Error: SEMICOLON [ \" ; \" ] expected at line %d\n", currentAtom->line);
            }
        }
        else 
        {
            // error: expr expected
            printf("Error: valid expression expected at line %d\n", currentAtom->line);
        }
    }
    if(consumeAtom(WHILE))
    {
        if(consumeAtom(LPAR))
        {
            if(expr())
            {
                if(consumeAtom(RPAR))
                {
                    if(block())
                    {
                        if(consumeAtom(END))
                        {
                            return 1;
                        }
                        else 
                        {
                            // error: END expected
                            printf("Error: END expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    else 
                    {
                        // error: block expected
                        printf("Error: valid block expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else 
                {
                    // error: RPAR expected
                    printf("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else 
            {
                // error: expr expected
                printf("Error: valid expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else 
        {
            // error: LPAR expected
            printf("Error: LPAR [ \" ( \" ] expected at line %d\n", currentAtom->line);
        }
    }
    printf("Error: an instruction was expected at line %d\n", currentAtom->line);
    return 0; // instr expected
}

int expr()
{
    return exprLogic();
}

int exprLogic()
{
    if(exprAssign())
    {
        while(1)
        {
            if(consumeAtom(AND) | consumeAtom(OR))
            {
                if(exprAssign())
                {
                    continue;
                }
                else 
                {
                    // error: exprLogic expected
                    printf("Error: valid logic expression expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            break;
        }
    }
}

int exprAssign()
{
    if(consumeAtom(ID))
    {
        if(consumeAtom(ASSIGN))
        {
            if(exprComp())
            {
                return 1;
            }
            else 
            {
                // error: exprComp expected
                printf("Error: valid comparison expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
    }
    if (exprComp())
    {
        return 1;
    }
    printf("Error: valid assignment expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprComp()
{
    if(exprAdd())
    {
        if(consumeAtom(LESS))
        {
            if(exprAdd()) 
            {
                return 1;
            }
            else 
            {
                // error: exprAdd expected
                printf("Error: valid addition expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        if(consumeAtom(EQUAL))
        {
            if(exprAdd()) 
            {
                return 1;
            }
            else 
            {
                // error: exprAdd expected
                printf("Error: valid addition expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }

        return 1;
    }
    printf("Error: valid addition expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprAdd()
{
    if(exprMul())
    {
        if(consumeAtom(ADD))
        {
            if(exprMul()) 
            {
                return 1;
            }
            else 
            {
                // error: exprMul expected
                printf("Error: valid multiplication expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        if(consumeAtom(SUB))
        {
            if(exprMul()) 
            {
                return 1;
            }
            else 
            {
                // error: exprMul expected
                printf("Error: valid multiplication expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }

        return 1;
    }
    printf("Error: valid multiplication expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprMul()
{
    if(exprPrefix())
    {
        if(consumeAtom(MUL))
        {
            if(exprPrefix()) 
            {
                return 1;
            }
            else 
            {
                // error: exprPrefix expected
                printf("Error: valid prefix expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        if(consumeAtom(DIV))
        {
            if(exprPrefix()) 
            {
                return 1;
            }
            else 
            {
                // error: exprPrefix expected
                printf("Error: valid prefix expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        return 1;
    }
    // error: exprPrefix expected
    printf("Error: valid prefix expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprPrefix()
{
    (consumeAtom(SUB) | consumeAtom(NOT));
    if(factor())
    {
        return 1;
    }
    printf("Error: valid factor expected at line %d\n", currentAtom->line);
    return 0;
}

int factor()
{
    if(consumeAtom(INT))
    {
        return 1;
    }
    if(consumeAtom(REAL))
    {
        return 1;
    }
    if(consumeAtom(STR))
    {
        return 1;
    }
    if(consumeAtom(LPAR))
    {
        if(expr())
        {
            if(consumeAtom(RPAR))
            {
                return 1;
            }
            else 
            {
                // error: RPAR expected
                printf("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else 
        {
            // error: expr expected
            printf("Error: valid expression expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    if(consumeAtom(ID))
    {
        if(consumeAtom(LPAR))
        {
            if(expr())
            {
                while(1)
                {
                    if(consumeAtom(COMMA))
                    {
                        if(expr) continue;
                        else 
                        {
                            // error: expr expected
                            printf("Error: valid expression expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    break;
                }
                if(consumeAtom(RPAR)) {}      
                else 
                {
                    // error: RPAR expected
                    printf("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else 
            {
                // error: expr expected
                printf("Error: valid expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        return 1;
    }
    printf("Error: factor expected at line %d\n", currentAtom->line);
    return 0; 
}


/*-------------------------*/