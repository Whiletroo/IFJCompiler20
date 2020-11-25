#include "expressions.h"

/*
1. ADT стак для PrecTableItem
2. функция для ковертирования токена на PrecTabItem
3. функция для поучения precedence ( < > = )
*/

/*
    if ( token.token_type > 4 || token.token_type < 22) {
        return (tPrecTableIndex) token.token_type - 5;
    }

    return NULL;
*/


int exp (){

    // Navratova hodnota
    int result;

    tPrecStack *precStack = 

    pushItem(DOLLAR);

    while(!emptyPrecStack()) {
        tPrecTabItem topTerm = topTermPrecStack()->type;
        switch (precedence()) {
            case '=':
                pushPrecStack(tkn2precItem());
                getToken(&token);
                
                break;

            case '<':
                insertReducePrecStack();
                pushPrecStack(tkn2precItem());
                getToken(&token);

                break;

            case '>':
                if (/*TODO*/ 1) {

                } else {
                    fprintf(stderr, "Error: \n");
                }

                break;

            // empty cell in precedence table
            default:
                fprintf(stderr, "Syntax error: expression error : %s\n",token.token_type);
                break;
        }

    }

    return;
}