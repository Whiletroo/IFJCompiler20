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

    while( tmpptr->precItem == NON_TERM ) {
        tmpptr = tmpptr->next;
    }
    return tmpptr;
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