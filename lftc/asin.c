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
        if(defProc())
        {
            continue;
        }
        break;
    }
    if(finish())
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
                }
            }
        }
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
                                if(consumeAtom(SEMICOLON))
                                {
                                    if(block())
                                    {
                                        if(consumeAtom(END))
                                        {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
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
            return 0;
        }
    }
    return 0;
}

int instr()
{
    expr();
    if(consumeAtom(SEMICOLON))
    {
        return 1;
    }
    return 0;
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
                                return 0; // END expected
                            }
                            return 0; // block expected
                        }
                        if(consumeAtom(END))
                        {
                            return 1;
                        }
                        return 0; // END expected
                    }
                    return 0; // block expected
                }
                return 0; // ) expected
            }
            return 0; // expr expected
        }
        return 0; // ( expected
    }
    if(consumeAtom(RETURN))
    {
        if(expr())
        {
            if(consumeAtom(SEMICOLON))
            {
                return 1;
            }
            return 0; // ; expected
        }
        return 0; // expr expected
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
                        return 0; // END expected
                    }
                    return 0; // block expected
                }
                return 0; // ) expected
            }
            return 0; // expr expected
        }
        return 0; // ( expected
    }
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
                if(exprLogic())
                {
                    continue;
                }
                return 0; // exprLogic expected
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
            return 0; // exprComp expected
        }
    }
    if (exprComp())
    {
        return 1;
    }
    return 0; // exprComp expected
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
            return 0; // exprAdd expected
        }
        if(consumeAtom(EQUAL))
        {
            if(exprAdd()) 
            {
                return 1;
            }
            return 0; // exprAdd expected
        }

        return 1;
    }
    return 0; // exprAdd expected
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
            return 0; // exprMul expected
        }
        if(consumeAtom(SUB))
        {
            if(exprMul()) 
            {
                return 1;
            }
            return 0; // exprMul expected
        }

        return 1;
    }
    return 0; // exprMul expected
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
            return 0; // exprPrefix expected
        }
        if(consumeAtom(DIV))
        {
            if(exprPrefix()) 
            {
                return 1;
            }
            return 0; // exprPrefix expected
        }
        return 1;
    }
    return 0; // exprPrefix expected
}

int exprPrefix()
{
    (consumeAtom(SUB) | consumeAtom(NOT));
    if(factor())
    {
        return 1;
    }
    return 0; // factor expected
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
            return 0; // ) expected
        }
        return 0; // expr expected
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
                        else return 0; // expr expected
                    }
                    break;
                }
                if(consumeAtom(RPAR)) {}             
                else return 0; // ) expected
            }
            return 0; // expr expected
        }
        return 1;
    }
    return 0; 
}


/*-------------------------*/