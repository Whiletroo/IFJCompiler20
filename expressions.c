/**
* @file expressions.h
* @author xkrukh00
* @date 03.11.2020
* @brief Implementation of Precedence analise
*/

#include "expressions.h"
#include "scanner.h"
#include "error.h"
#include "precstack.h"
#include "gena.h"
#include "parser.h"
#include "symtable.h"


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
        case IDENTIFIER: case INT_NUMBER: case FLOAT64_NUMBER: case STRING: case NIL:
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
/*
    if ( token.token_type > 3 || token.token_type < 24) {
        return token.token_type - 4;
    } else if (token.token_type == TOKEN_EOL || token.token_type == TOKEN_RCURLY_BRACKET ) {
        return DOLLAR;
    } else {    
        return NON_TERM;
    }
    */
   switch (token.token_type){
        case TOKEN_IDENTIFIER:
            return IDENTIFIER;
        case TOKEN_INT:
            return INT_NUMBER;
        case TOKEN_FLOAT64:
            return FLOAT64_NUMBER;
        case TOKEN_STRING:
            return STRING;
        case TOKEN_BOOL:
            return BOOLEAN;
        case TOKEN_NIL:
            return NIL;
        case TOKEN_PLUS:
            return ADD;
        case TOKEN_MINUS:
            return SUB;
        case TOKEN_MUL:
            return MUL;
        case TOKEN_DIV:
            return DIV;
        case TOKEN_EQUALS:
            return EQ;
        case TOKEN_HIGHER_OR_EQUAL:
            return HEQ;
        case TOKEN_HIGHER:
            return HTN;
        case TOKEN_LESS_OR_EQUAL:
            return LEQ;
        case TOKEN_LESS:
            return LTN;
        case TOKEN_NOT_EQUAL:
            return NEQ;
        case TOKEN_LEFT_BRACKET:
            return LEFT_BRACKET;
        case TOKEN_RIGHT_BRACKET:
            return RIGHT_BRACKET;
        case TOKEN_LCURLY_BRACKET: case TOKEN_EOL: case TOKEN_SEMICOLON:
            return DOLLAR;
        default:
            return NON_TERM;
            break;
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
        case TOKEN_IDENTIFIER:
            if (symTableSearch(localTable, token.attribute.value_string->str) ) {
                return symTableGetItem(localTable, token.attribute.value_string->str)->dataType[0];
            } else {
                return UNDEFINED_TYPE;
            }
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
            case FLOAT64_NUMBER:
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
                // E -> E idiv E
                if (precStack->top->next->precItem == DIV && precStack->top->dataType == INT_TYPE && precStack->top->next->next->dataType == INT_TYPE) {
                    return E_IDIV_E;
                }
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

        case E_PLUS_E: case E_MINUS_E: case E_MUL_E:
            *expType = FLOAT_TYPE;
            // str +- str

            if ( precStack->top->next->next->dataType == STRING_TYPE || precStack->top->dataType == STRING_TYPE ) {
                if ( precStack->top->next->next->dataType == STRING_TYPE && precStack->top->dataType == STRING_TYPE ) {
                    // str + str
                    if (rule == E_PLUS_E) {
                        *expType = STRING_TYPE;
                        break;
                    }
                }
                return SEM_ERR_TYPE_COMPAT;
            }

            // int +- int
            if ( precStack->top->next->next->dataType == INT_TYPE && precStack->top->dataType == INT_TYPE ) {
                *expType = INT_TYPE;
                break;
            // float64 +- int
            } else if ( precStack->top->next->next->dataType == FLOAT_TYPE && precStack->top->dataType == INT_TYPE ) {

                break;
            // int +- float64
            } else if ( precStack->top->next->next->dataType == INT_TYPE && precStack->top->dataType == FLOAT_TYPE ) {
                break;
            }
            break;
        
        case E_DIV_E:
            *expType = FLOAT_TYPE;
            if ( precStack->top->next->next->dataType == INT_TYPE || precStack->top->dataType == INT_TYPE ) {
                return SEM_ERR_TYPE_COMPAT;
            }

            if (precStack->top->dataType == STRING_TYPE || precStack->top->next->next->dataType == STRING_TYPE) {
                return SEM_ERR_TYPE_COMPAT;
            }
            break;

        case E_IDIV_E:
            *expType = INT_TYPE;
            break;

        case E_EQ_E: case E_HEQ_E: case E_HTN_E:
        case E_LEQ_E: case E_LTN_E: case E_NEQ_E:
            *expType = BOOLEAN_TYPE;
            if (precStack->top->dataType != precStack->top->next->next->dataType) {
                return SEM_ERR_EXP_COMPAT;
            }
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

    int i;
    if (rule < 3) {
        i = 2;
    } else {
        i = 4;
    }

    for(int it = 0; it < i; it++){
        popPS();
    }
    pushPS(NON_TERM, expType);

    if (tmparg != NULL) {
        tmparg->dataType[0] = expType;
    } else {
        return SEM_ERR_UNDEFINED_VAR;
    }

    return OK;
}


char precedence(tPrecTabIndex topTerm, tPrecTabIndex inTerm) {
    return precedence_table[topTerm][inTerm];
}




int expessions (){

    int result;
    // precedence stack initialization
    precStack = initPS();
    if (pushPS(DOLLAR, UNDEFINED_TYPE)) {
        return ERR_INTERNAL;
    }

    // 3 registers for processing expression
    //genCreDefVar(2, "R1");
    //genCreDefVar(2, "R2");
    //genCreDefVar(2, "R3");

    tPrecTabItem topTerm;   // top terminal in precedence stack
    tPrecTabItem inTerm;    // terminal on input

    while(true) {
        
        // init both terminals
        if (token.token_type == TOKEN_IDENTIFIER) {
            
        }
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
                insertReducePS();
                pushPS(tkn2precItem(), tokenType2DataType());
                getToken(&token);
                break;

            case '>':
                // check by rule
                result = reduce();
                if ( result ) {
                    fprintf(stderr, "Error: %d\n", result);
                    freePS();
                    return result;
                }
                break;

            // empty cell in precedence table
            default:
                if (topTerm == DOLLAR && inTerm == DOLLAR) {
                    freePS();
                    return OK;
                } else {
                    fprintf(stderr, "Syntax error: expression error : %s\n",getTokenName(token.token_type));
                    freePS();
                    return SYNTAX_ERR;
                }
                    
                break;
        }
    }

    tmparg->defined = true;

    return OK;
}
