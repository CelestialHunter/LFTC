#include "asin.h"

/*----------DATA-----------*/
Atom *currentAtom;
Atom *consumedAtom;

/*-------------------------*/

/*--------FUNCTIONS--------*/
int consumeAtom(LEXIC code)
{
    if (currentAtom->code == code)
    {
        consumedAtom = currentAtom++;
        return 1;
    }
    return 0;
}

void initAsin()
{
    currentAtom = getAtomi();
    consumedAtom = NULL;
}

void printError(char *error, int line)
{
    printf(error, line);
    exit(0);
}

int program()
{
    adaugaDomeniu();

    adaugaFnPredefinite();

    while (1)
    {
        if (defVar() || defFunc() || block())
        {
            continue;
        }
        break;
    }
    if (consumeAtom(FINISH))
    {
        stergeDomeniu();

        return 1;
    }
    printError("Eroare la linia %d: Asteptam un atom final\n", consumedAtom->line + 1);
}

int defVar()
{
    if (consumeAtom(VAR))
    {
        if (consumeAtom(ID))
        {
            const char *nume = consumedAtom->value.s;
            Simbol *s = cautaInDomeniulCurent(nume);
            if (s)
            {
                char error[100];
                sprintf(error, "Eroare la linia %d: Simbolul %s este deja definit\n", consumedAtom->line, nume);
                printError(error, consumedAtom->line);
            }
            s = adaugaSimbol(nume, FEL_VAR);
            s->local = crtFn != NULL;

            if (consumeAtom(COLON))
            {
                if (baseType())
                {
                    s->tip = ret.tip;
                    if (consumeAtom(SEMICOLON))
                    {
                        return 1;
                    }
                    else
                    {
                        // error: SEMICOLON expected
                        printError("Error: SEMICOLON [ \" ; \" ] expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else
                {
                    // error: baseType expected
                    printError("Error: variable baseType expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else
            {
                // error: COLON expected
                printError("Error: COLON [ \" : \" ] expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else
        {
            // error: ID expected
            printError("Error: ID expected at line %d\n", currentAtom->line);
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
    if (consumeAtom(TYPE_INT))
    {
        ret.tip = TYPE_INT;
        return 1;
    }
    if (consumeAtom(TYPE_REAL))
    {
        ret.tip = TYPE_REAL;
        return 1;
    }
    if (consumeAtom(TYPE_STR))
    {
        ret.tip = TYPE_STR;
        return 1;
    }
    // error: baseType expected
    // printError( "Error: valid baseType expected at line %d\n", currentAtom->line);
    return 0;
}

int defFunc()
{
    if (consumeAtom(FUNCTION))
    {
        if (consumeAtom(ID))
        {

            const char *nume = consumedAtom->value.s;
            Simbol *s = cautaInDomeniulCurent(nume);
            if (s)
            {
                char error[100];
                sprintf(error, "Eroare la linia %d: Simbolul %s este deja definit\n", consumedAtom->line, nume);
                printError(error, consumedAtom->line);
            }
            crtFn = adaugaSimbol(nume, FEL_FN);
            crtFn->args = NULL;
            adaugaDomeniu();

            if (consumeAtom(LPAR))
            {
                if (funcParams())
                {
                    if (consumeAtom(RPAR))
                    {
                        if (consumeAtom(COLON))
                        {
                            if (baseType())
                            {
                                crtFn->tip = ret.tip;
                                while (1)
                                {
                                    if (defVar())
                                    {
                                        continue;
                                    }
                                    break;
                                }
                                if (block())
                                {
                                    if (consumeAtom(END))
                                    {
                                        stergeDomeniu();
                                        crtFn = NULL;
                                        return 1;
                                    }
                                    else
                                    {
                                        // error: END expected
                                        printError("Error: END expected after block at line %d\n", currentAtom->line);
                                        return 0;
                                    }
                                }
                                else
                                {
                                    // error: block expected
                                    printError("Error: function block expected at line %d\n", currentAtom->line);
                                    return 0;
                                }
                            }
                            else
                            {
                                // error: baseType expected
                                printError("Error: valid function baseType expected at line %d\n", currentAtom->line);
                                return 0;
                            }
                        }
                        else
                        {
                            // error: COLON expected
                            printError("Error: COLON [ \" : \" ] expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    else
                    {
                        // error: RPAR expected
                        printError("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else
                {
                    // error: funcParams expected
                    printError("Error: valid funcParams expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else
            {
                // error: LPAR expected
                printError("Error: LPAR [ \" ( \" ] expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else
        {
            // error: ID expected
            printError("Error: ID expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    return 0;
}

int block()
{
    if (instr())
    {
        while (1)
        {
            if (instr())
            {
                continue;
            }
            break;
        }
        return 1;
    }
    // currentAtom = consumedAtom;
    // printError( "Error: valid instruction expected at line %d\n", currentAtom->line);
    return 0;
}

int funcParams()
{
    funcParam();
    while (1)
    {
        if (consumeAtom(COMMA))
        {
            if (funcParam())
            {
                continue;
            }
            else
            {
                // error: funcParam expected
                printError("Error: valid funcParam expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        break;
    }
    return 1;
}

int funcParam()
{
    if (consumeAtom(ID))
    {
        const char *nume = consumedAtom->value.s;
        Simbol *s = cautaInDomeniulCurent(nume);
        if (s)
        {
            char error[100];
            sprintf(error, "Eroare la linia %d: Simbolul %s este deja definit\n", consumedAtom->line, nume);
            printError(error, consumedAtom->line);
        }
        s = adaugaSimbol(nume, FEL_ARG);
        Simbol *argFn = adaugaArgFn(crtFn, nume);

        if (consumeAtom(COLON))
        {
            if (baseType())
            {
                s->tip = ret.tip;
                argFn->tip = ret.tip;
                return 1;
            }
            else
            {
                // error: baseType expected
                printError("Error: valid baseType expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else
        {
            // error: COLON expected
            printError("Error: COLON [ \" : \" ] expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    // currentAtom = consumedAtom;
    // printError( "Error: ID expected at line %d\n", currentAtom->line);
    return 0;
}

int instr()
{
    if (expr())
        if (!consumeAtom(SEMICOLON))
        {
            printError("Error: SEMICOLON [ \" ; \" ] expected at line %d\n", currentAtom->line);
            return 0;
        }
        else
            return 1;
    if (consumeAtom(IF))
    {
        if (consumeAtom(LPAR))
        {
            if (expr())
            {
                if (ret.tip == TYPE_STR)
                    printError("IF condition must be INT or REAL at line %d\n", currentAtom->line);
                if (consumeAtom(RPAR))
                {
                    if (block())
                    {
                        if (consumeAtom(ELSE))
                        {
                            if (block())
                            {
                                if (consumeAtom(END))
                                {
                                    return 1;
                                }
                                else
                                {
                                    // error: END expected
                                    printError("Error: END expected at line %d\n", currentAtom->line);
                                    return 0;
                                }
                            }
                            else
                            {
                                // error: block expected
                                printError("Error: valid block expected at line %d\n", currentAtom->line);
                                return 0;
                            }
                        }
                        if (consumeAtom(END))
                        {
                            return 1;
                        }
                        else
                        {
                            // error: END expected
                            printError("Error: END expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    else
                    {
                        // error: block expected
                        printError("Error: valid block expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else
                {
                    // error: RPAR expected
                    printError("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else
            {
                // error: expr expected
                printError("Error: valid expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else
        {
            // error: LPAR expected
            printError("Error: LPAR [ \" ( \" ] expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    if (consumeAtom(RETURN))
    {
        if (expr())
        {
            if(!crtFn)
                printError("Error: RETURN can only occur within a function. Line %d\n", currentAtom->line);
            if(ret.tip!=crtFn->tip)
                printError("Return value does not match function type at line %d\n", currentAtom->line);

            if (consumeAtom(SEMICOLON))
            {
                return 1;
            }
            else
            {
                // error: SEMICOLON expected
                printError("Error: SEMICOLON [ \" ; \" ] expected at line %d\n", currentAtom->line);
            }
        }
        else
        {
            // error: expr expected
            printError("Error: valid expression expected at line %d\n", currentAtom->line);
        }
    }
    if (consumeAtom(WHILE))
    {
        if (consumeAtom(LPAR))
        {
            if (ret.tip == TYPE_STR)
                    printError("WHILE condition must be INT or REAL at line %d\n", currentAtom->line);
            if (expr())
            {
                if (consumeAtom(RPAR))
                {
                    if (block())
                    {
                        if (consumeAtom(END))
                        {
                            return 1;
                        }
                        else
                        {
                            // error: END expected
                            printError("Error: END expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    else
                    {
                        // error: block expected
                        printError("Error: valid block expected at line %d\n", currentAtom->line);
                        return 0;
                    }
                }
                else
                {
                    // error: RPAR expected
                    printError("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            else
            {
                // error: expr expected
                printError("Error: valid expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else
        {
            // error: LPAR expected
            printError("Error: LPAR [ \" ( \" ] expected at line %d\n", currentAtom->line);
        }
    }
    // printError( "Error: an instruction was expected at line %d\n", currentAtom->line);
    return 0; // instr expected
}

int expr()
{
    return exprLogic();
}

int exprLogic()
{
    if (exprAssign())
    {
        while (1)
        {
            if (consumeAtom(AND) | consumeAtom(OR))
            {
                Ret tipStanga = ret;
                if(tipStanga.tip == TYPE_STR)
                    printError("Error: Left operand of AND/OR must be INT or REAL at line %d\n", currentAtom->line);

                if (exprAssign())
                {
                    Ret tipDreapta = ret;
                    if(tipDreapta.tip == TYPE_STR)
                        printError("Error: Right operand of AND/OR must be INT or REAL at line %d\n", currentAtom->line);
                    setRet(TYPE_INT, false);
                    continue;
                }
                else
                {
                    // error: exprLogic expected
                    printError("Error: valid logic expression expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            break;
        }
        return 1;
    }
}

int exprAssign()
{
    if (consumeAtom(ID))
    {
        const char* nume = consumedAtom->value.s;
        if (consumeAtom(ASSIGN))
        {
            if (exprComp())
            {
                Simbol* s = cautaSimbol(nume);
                const char* error[100];
                
                if(!s) 
                {                    
                    sprintf(error, "Error: Undeclared variable %s at line \%d\n", nume);
                    printError(error, currentAtom->line);
                }

                if(s->tip == FEL_FN)
                {
                    sprintf(error, "Error: %s is a function at line \%d\n", nume);
                    printError(error, currentAtom->line);
                }

                if(s->tip != ret.tip)
                {
                    printError("Error: Type mismatch at line %d\n", currentAtom->line);
                }

                ret.lval = false;
                return 1;
            }
            else
            {
                // error: exprComp expected
                printError("Error: valid comparison expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else
        {
            currentAtom = consumedAtom;
        }
    }
    if (exprComp())
    {
        return 1;
    }
    // printError( "Error: valid assignment expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprComp()
{
    if (exprAdd())
    {
        if (consumeAtom(LESS))
        {
            Ret tipStanga = ret;

            if (exprAdd())
            {
                if(tipStanga.tip != ret.tip)
                {
                    printError("Error: Type mismatch for < operands at line %d\n", currentAtom->line);
                }
                setRet(TYPE_INT, false);
                return 1;
            }
            else
            {
                // error: exprAdd expected
                printError("Error: valid comparing expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        if (consumeAtom(EQUAL))
        {
            Ret tipStanga = ret;
            if (exprAdd())
            {
                if(tipStanga.tip != ret.tip)
                {
                    printError("Error: Type mismatch for == operands at line %d\n", currentAtom->line);
                }
                setRet(TYPE_INT, false);
                return 1;
                return 1;
            }
            else
            {
                // error: exprAdd expected
                printError("Error: valid assignment expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }

        return 1;
    }
    // printError( "Error: valid addition expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprAdd()
{
    if (exprMul())
    {
        if (consumeAtom(ADD))
        {
            Ret tipStanga = ret;
            if(tipStanga.tip == TYPE_STR)
                printError("Error: Left operand of + must be INT or REAL at line %d\n", currentAtom->line);
            if (exprMul())
            {
                if(tipStanga.tip != ret.tip)
                {
                    printError("Error: Type mismatch for + operands (must be same type) at line %d\n", currentAtom->line);
                }
                ret.lval = false;
                return 1;
            }
            else
            {
                // error: exprMul expected
                printError("Error: valid addition expression expected at line %d\n", currentAtom->line);
            }
        }
        if (consumeAtom(SUB))
        {
            Ret tipStanga = ret;
            if(tipStanga.tip == TYPE_STR)
                printError("Error: Left operand of - must be INT or REAL at line %d\n", currentAtom->line);
            if (exprMul())
            {
                if(tipStanga.tip != ret.tip)
                {
                    printError("Error: Type mismatch for - operands (must be same type) at line %d\n", currentAtom->line);
                }
                ret.lval = false;
                return 1;
            }
            else
            {
                // error: exprMul expected
                printError("Error: valid subtraction expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }

        return 1;
    }
    // printError( "Error: valid multiplication expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprMul()
{
    if (exprPrefix())
    {
        if (consumeAtom(MUL))
        {
            Ret tipStanga = ret;
            if(tipStanga.tip == TYPE_STR)
                printError("Error: Left operand of * must be INT or REAL at line %d\n", currentAtom->line);

            if (exprPrefix())
            {
                if(tipStanga.tip != ret.tip)
                    printError("Error: Type mismatch for * operands (must be same type) at line %d\n", currentAtom->line);

                ret.lval = false;
                return 1;
            }
            else
            {
                // error: exprPrefix expected
                printError("Error: valid multiplication expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        if (consumeAtom(DIV))
        {
            Ret tipStanga = ret;
            if(tipStanga.tip == TYPE_STR)
                printError("Error: Left operand of / must be INT or REAL at line %d\n", currentAtom->line);

            if (exprPrefix())
            {
                if(tipStanga.tip != ret.tip)
                    printError("Error: Type mismatch for / operands (must be same type) at line %d\n", currentAtom->line);
                ret.lval = false;
                return 1;
            }
            else
            {
                // error: exprPrefix expected
                printError("Error: valid prefix expected after / at line %d\n", currentAtom->line);
                return 0;
            }
        }
        return 1;
    }
    // error: exprPrefix expected
    // printError( "Error: valid prefix expression expected at line %d\n", currentAtom->line);
    return 0;
}

int exprPrefix()
{
    if(consumeAtom(SUB))
    {
        if(ret.tip==TYPE_STR)
            printError("Error: - cannot be applied to STR at line %d\n", currentAtom->line);
        
        if(factor())
        {
            ret.lval = false;   
            return 1;
        }
    }
    else        
    if(consumeAtom(NOT))
    {
        if(ret.tip==TYPE_STR)
            printError("Error: ! cannot be applied to STR at line %d\n", currentAtom->line);
        if(factor())
        {
            ret.lval = false;  
            return 1;
        }
    }
    else 
    if(factor())
    {
        return 1;
    }
    // printError( "Error: valid factor expected at line %d\n", currentAtom->line);
    return 0;
}

int factor()
{
    if (consumeAtom(INT))
    {
        setRet(TYPE_INT, false);
        return 1;
    }
    if (consumeAtom(REAL))
    {
        setRet(TYPE_REAL, false);
        return 1;
    }
    if (consumeAtom(STR))
    {
        setRet(TYPE_STR, false);
        return 1;
    }
    if (consumeAtom(LPAR))
    {
        if (expr())
        {
            if (consumeAtom(RPAR))
            {
                return 1;
            }
            else
            {
                // error: RPAR expected
                printError("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        else
        {
            // error: expr expected
            printError("Error: valid expression expected at line %d\n", currentAtom->line);
            return 0;
        }
    }
    if (consumeAtom(ID))
    {
        Simbol* s = cautaSimbol(consumedAtom->value.s);
        if(!s) 
        {
            const char error[100];
            sprintf(error, "Error: undeclared variable %s at line \%d\n", consumedAtom->value.s);
            printError(error, consumedAtom->line);
        }
        if (consumeAtom(LPAR))
        {
            if(s->fel != FEL_FN)
            {
                const char error[100];
                sprintf(error, "Error: %s is not a function at line \%d\n", consumedAtom->value.s);
                printError(error, consumedAtom->line);
            }
            Simbol* argDef = s->args;
            if (expr())
            {
                if(!argDef)
                {
                    const char error[100];
                    sprintf(error, "Error: too many arguments for function %s at line \%d\n", consumedAtom->value.s);
                    printError(error, consumedAtom->line);
                }
                else
                {
                    if(argDef->tip != ret.tip)
                    {
                        const char error[100];
                        sprintf(error, "Error: type mismatch for argument %s of function %s at line \%d\n", argDef->nume, consumedAtom->value.s);
                        printError(error, consumedAtom->line);
                    }
                    argDef = argDef->urm;
                }

                while (1)
                {
                    if (consumeAtom(COMMA))
                    {
                        if (expr())
                        {
                            if(!argDef)
                            {
                                const char error[100];
                                sprintf(error, "Error: too many arguments for function %s at line \%d\n", consumedAtom->value.s);
                                printError(error, consumedAtom->line);
                            }
                            else
                            {
                                if(argDef->tip != ret.tip)
                                {
                                    const char error[100];
                                    sprintf(error, "Error: type mismatch for argument %s of function %s at line \%d\n", argDef->nume, consumedAtom->value.s);
                                    printError(error, consumedAtom->line);
                                }
                                argDef = argDef->urm;
                            }
                            continue;
                        }
                        else
                        {
                            // error: expr expected
                            printError("Error: valid expression expected at line %d\n", currentAtom->line);
                            return 0;
                        }
                    }
                    break;
                }
                if (consumeAtom(RPAR))
                {
                    if(argDef)
                    {
                        const char error[100];
                        sprintf(error, "Error: too few arguments for function %s at line \%d\n", consumedAtom->value.s);
                        printError(error, consumedAtom->line);
                    }
                    setRet(s->tip, false);
                    return 1;
                }
                else
                {
                    // error: RPAR expected
                    printError("Error: RPAR [ \" ) \" ] expected at line %d\n", currentAtom->line);
                    return 0;
                }
            }
            if(s->fel == FEL_FN)
            {
                const char error[100];
                sprintf(error, "Error: too many arguments for function %s at line \%d\n", consumedAtom->value.s);
                printError(error, consumedAtom->line);
            }
            else
            {                
                // error: expr expected
                printError("Error: valid expression expected at line %d\n", currentAtom->line);
                return 0;
            }
        }
        return 1;
    }
    // printError( "Error: factor expected at line %d\n", currentAtom->line);
    return 0;
}

/*-------------------------*/