#include "expressions.h"

const char precedence_table[7][7] = {
/*         /*   +-   rel  id    (    )    $   */
/* / * */ {'>', '>', '>', '<', '<', '>', '>'},
/* + - */ {'<', '>', '>', '<', '<', '>', '>'},
/* rel */ {'<', '<', ' ', '<', '<', '>', '>'},
/* id  */ {'>', '>', '>', ' ', ' ', '>', '>'},
/*  (  */ {'<', '<', '<', '<', '<', '=', '>'},
/*  )  */ {'>', '>', '>', ' ', ' ', '>', '>'},
/*  $  */ {'<', '<', '<', '<', '<', '<', ' '}
};


tPrecTabIndex getPrecTabIndex(tPrecTabItem precItem){
    switch (precItem) {
        case IDENTIFIER: case INT_NUMBER: case DOUBLE_NUMBER: case STRING: case NIL:
            return I_DATA;
            break;
        case ADD: case SUB:
            return I_ADD_SUB;
            break;
        case MUL: case DIV:
            return I_MUL_DIV;
            break;
        case EQ: case NEQ: case LEQ: case LTN: case HEQ: case HTN:
            return I_REL;
            break;
        case LEFT_BRACKET:
            return I_LEFT_BRACKET;
            break;
        case RIGHT_BRACKET:
            return RIGHT_BRACKET;
            break;
        case DOLLAR:
            return I_DOLLAR;
            break;
        default:
            break;
    }
}


tPrecTabItem tkn2precItem() {

    if ( token.token_type > 3 || token.token_type < 23) {
        return token.token_type - 4;
    } else if (token.token_type == TOKEN_EOL || token.token_type == TOKEN_RCURLY_BRACKET ) {
        return DOLLAR;
    } else {    
        return NON_TERM;
    }
}


tDataType tokenType2DataType() {
    switch (token.token_type) {
        case TOKEN_INT:
            return INT_TYPE;
            break;
        case TOKEN_FLOAT64:
            return FLOAT_TYPE;
            break;
        case TOKEN_STRING:
            return STRING_TYPE;
            break;
        case TOKEN_NIL:
            return NIL_TYPE;
            break;
        default:
            return UNDEFINED_TYPE;
            break;
    }
}


char precedence(tPrecTabIndex topTerm, tPrecTabIndex inTerm) {
    return precedence_table[topTerm][inTerm];
}


int reduce() {

    

}


int expessions (){

    // return value
    int result = SYNTAX_ERR;

    // precedence stack initialization
    precStack = initPS;
    pushPS(DOLLAR, UNDEFINED_TYPE);

    tPrecTabItem topTerm;   // top terminal in precedence stack
    tPrecTabItem inTerm;    // terminal on input

    while(!emptyPS()) {
        
        // init both terminals
        topTerm = topTermPS()->precItem;
        inTerm = tkn2precItem();

        switch (precedence( getPrecTabIndex(topTerm), getPrecTabIndex(inTerm) )) {
            case '=':
                // 1. push new terminal
                // 2. get next terminal
                pushPS(tkn2precItem(), tokenType2DataType());
                getToken(&token);
                break;

            case '<':
                // 1. insert reduce item after top terminal
                // 2. push new terminal
                // 3. get next terminal
                pusPS(REDUCE, UNDEFINED_TYPE);
                pushPS(tkn2precItem(), tokenType2DataType());
                getToken(&token);
                break;

            case '>':
                // check by rule
                if (reduce()) {
                    fprintf(stderr, "Error: \n");
                    return ERR_INTERNAL;
                }
                break;

            // empty cell in precedence table
            default:
                if (topTerm == DOLLAR && inTerm == DOLLAR) {
                    return OK;
                } else {
                    fprintf(stderr, "Syntax error: expression error : %s\n",token.token_type);
                    freePS();
                    return SYNTAX_ERR;
                }
                    
                break;
        }
    }

    return OK;
}