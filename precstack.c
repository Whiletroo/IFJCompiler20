/**
* @file precstack.c
* @author xkrukh00
* @date 03.12.2020
* @brief The implementation of stack for precedence analise
*/

#include "precstack.h"

#define CHECKPTR_RETNULL(ptr)\
    if (ptr == NULL)\
        return NULL;

#define CHECKPTR_RETERR(ptr)\
    if (ptr == NULL)\
        return ERR_INTERNAL;



tPS *initPS(){

    tPS *tmpptr = (tPS *)malloc(sizeof(tPS));
    if( tmpptr == NULL) {
        return NULL;
    }
    tmpptr->top = NULL;
    return tmpptr;
}


int pushPS(tPrecTabItem precItem, tDataType dataType) {

    tPSItem *newPSItem = malloc(sizeof(tPSItem));
    CHECKPTR_RETERR(newPSItem);
/*  
    if (token.id != NULL) {
        newPSItem->id = malloc(sizeof(char));
        CHECKPTR_RETERR(newPSItem->id);
        strcpy(newPSItem->id,token.id);
    } else {
        newPSItem->id = NULL;
    }
*/
    newPSItem->next = precStack->top;
    newPSItem->precItem = precItem;
    newPSItem->dataType = dataType;
    precStack->top = newPSItem;
    return OK;
}


tPSItem *topPS() {
    CHECKPTR_RETNULL(precStack);
    return precStack->top;
}


tPSItem *topTermPS() {

    CHECKPTR_RETNULL(precStack);
    CHECKPTR_RETNULL(precStack->top);

    tPSItem *tmpptr = precStack->top;

    while( tmpptr->precItem != DOLLAR ) {

        if ( tmpptr->precItem < 17) {
            break;
        }

        tmpptr = tmpptr->next;
    }

    return tmpptr;
}


void insertReducePS() {    

    tPSItem *act;
    tPSItem *next = topPS();

    if (next->precItem != NON_TERM ) {
        pushPS(REDUCE, UNDEFINED_TYPE);
        return;
    }

    while ( next->precItem == NON_TERM ) {
        act = next;
        next = next->next;
    }

    act->next = malloc(sizeof(tPSItem));
    act->next->next = next;
    act->next->precItem = REDUCE;
    act->next->dataType = UNDEFINED_TYPE;

}



bool emptyPS() {
    return (precStack->top->precItem == DOLLAR) ? true : false;
}


void popPS() {
    
    if(precStack == NULL){
        return;
    } else if (precStack->top == NULL) {
        return;
    }
    
    if (precStack->top->precItem != DOLLAR) {
        tPSItem *tmpptr = precStack->top;
        precStack->top = precStack->top->next;
    //    if (tmpptr->id != NULL)
    //        free(tmpptr->id);
        free(tmpptr);
    }
}

void disposePS() {

    while(!emptyPS()) {
        popPS();
    }
}

void freePS(){
    disposePS();
    free(precStack->top);
    free(precStack);
}