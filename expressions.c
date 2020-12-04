/**
* @file expressions.h
* @author xkrukh00
* @date 03.11.2020
* @brief Implementation of Precedence analise
*/

#include "precstack.h"
#include "expressions.h"

tPS *precStack;

const char precedence_table[7][7] = {
/*         / *  + -  rel  id    (    )    $   */
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
        default: // DOLLAR
            return I_DOLLAR;
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

tPrecRules getRule(){

    if (precStack->top->next->precItem == REDUCE) {
        // E -> i
        switch (precStack->top->precItem) {
            case IDENTIFIER:
            case INT_NUMBER:
            case DOUBLE_NUMBER:
            case STRING:
                return E_OPERAND;
                break;
        
            default:
                return NOT_E_RULE;
                break;
        }

    } else if (precStack->top->next->next->next->precItem == REDUCE) {
       // E -> (E) 
       if (precStack->top->precItem == RIGHT_BRACKET && precStack->top->next->precItem == NON_TERM && precStack->top->next->next->precItem == LEFT_BRACKET) {
           return LBRCT_E_RBRCT;

        } else if (precStack->top->precItem == NON_TERM && precStack->top->next->next->precItem == NON_TERM) {
            // rules : E -> E + E  ...  E -> E != E
            if(precStack->top->next->precItem > 4 && precStack->top->next->precItem < 15) {
                return precStack->top->next->precItem - 2;
            } else {
                return NOT_E_RULE;
            }
        } else {
           return NOT_E_RULE;
        }
    } else {
        return NOT_E_RULE;
    }
}


int sematic(tPrecRules rule, tDataType *expType) {

    /* Checking operands definitions */
    
    // E -> i
    if (rule == E_OPERAND) {

        if (precStack->top->dataType == UNDEFINED_TYPE) {
            return SEM_ERR_TYPE_COMPAT;
        }

    // E -> (E)
    } else if (rule == LBRCT_E_RBRCT) {

        if (precStack->top->next->dataType == UNDEFINED_TYPE) {
            return SEM_ERR_UNDEFINED_VAR;
        }

    } else if ( rule > 2 ) {
        // Rules with operators. If one of operands have to type -> Error.
        if (precStack->top->dataType == UNDEFINED_TYPE || precStack->top->next->next->dataType == UNDEFINED_TYPE ) {
            return SEM_ERR_UNDEFINED_VAR;
        }
    }


    // Assing data type of expression result
    switch (rule) {
        case E_OPERAND:
            *expType = precStack->top->dataType;
            break;
        case LBRCT_E_RBRCT:
            *expType = precStack->top->next->dataType;
            break;

        case E_PLUS_E:
            *expType = FLOAT_TYPE;
            if ( precStack->top->next->next->dataType == STRING_TYPE && precStack->top->dataType == STRING_TYPE && rule == E_PLUS_E ) {
                *expType = STRING_TYPE;
                break;
            }  else if ( precStack->top->next->next->dataType == INT_TYPE && precStack->top->dataType == INT_TYPE ) {
                *expType = INT_TYPE;
                break;
            } else if ( precStack->top->next->next->dataType == FLOAT_TYPE && precStack->top->dataType == INT_TYPE ) {
                
            }
        break;

        default:
            break;
        }
    return OK;

}


int reduce() {

    int result = SYNTAX_ERR;
    tDataType expType = UNDEFINED_TYPE;
    tPrecRules rule = getRule();

    if (rule == NOT_E_RULE) {
        return SYNTAX_ERR;
    }

    result = sematic(rule, &expType);
    if(result){
        return result;
    }

    return OK;
}


char precedence(tPrecTabIndex topTerm, tPrecTabIndex inTerm) {
    return precedence_table[topTerm][inTerm];
}




int expessions (){

    // precedence stack initialization
    precStack = initPS();
    if (!pushPS(DOLLAR, UNDEFINED_TYPE)) {
        return ERR_INTERNAL;
    }

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
                pushPS(REDUCE, UNDEFINED_TYPE);
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
                    fprintf(stderr, "Syntax error: expression error : %s\n",getTokenName(token.token_type));
                    freePS();
                    return SYNTAX_ERR;
                }
                    
                break;
        }
    }

    return OK;
}