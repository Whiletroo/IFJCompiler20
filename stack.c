/**
 * @file stack.c
 * @author xpimen00
 * @date 8.10.2020
 * @brief
*/

#include "stack.h"

//TODO Implement stack

void stackInit (Stack* s)
{

    s->TopCounter = -1;
    s->topItem = NULL;
}

int stackEmpty (Stack* s)
{

    return s->topItem == NULL;
}

void stackPush (Stack* s, tToken t, tPrecTabItem type)
{

    StackItem* newTop = (StackItem*) malloc(sizeof(StackItem));

    if(newTop == NULL)
    {
        fprintf(stderr, "Internal Error: Failed Allocation\n");
        exit(-1);
    }

    s->TopCounter += 1;

    newTop->token = t;
    newTop->isTerminal = true;
    newTop->nextItem = s->topItem;
    newTop->type = type;

    s->topItem = newTop;
}

StackItem* stackPop (Stack* s)
{

    if(stackEmpty(s))
    {
        return NULL;
    }

    StackItem* tmp = s->topItem;
    s->topItem = tmp->nextItem;

    s->TopCounter -= 1;

    free(tmp);

    return tmp;
}

StackItem* stackTop(Stack* s)
{

    if(stackEmpty(s))
    {
        return NULL;
    }

    StackItem* tmp = s->topItem;

    return tmp;
}

tToken* getStackTopToken(Stack* s)
{

    if(stackEmpty(s))
    {
        return NULL;
    }

    token = s->topItem->token;

    return &token;
}

int insertStopper(Stack* s)
{
    StackItem* terminal = NULL;

    for(StackItem* tmpTop = s->topItem; tmpTop != NULL; tmpTop = tmpTop->nextItem)
    {
        if(tmpTop->symbol < STOP)
        {
            StackItem* stopper = (StackItem*) malloc(sizeof(StackItem));

            if(stopper == NULL)
            {
                return 0;
            }

            stopper->symbol = STOP;
            stopper->type = TOKEN_NIL;

            if(terminal == NULL)
            {
                stopper->nextItem = s->topItem;
                s->topItem = stopper;
            } else {
                stopper->nextItem = terminal->nextItem;
                terminal->nextItem = stopper;
            }

            return 1;
        }

        terminal = tmpTop;
    }

    return 0;
}

int getStackItemCounterBeforeStopper(Stack *s)
{

    int counter = 0;
    StackItem* item = stackTop(s);

    while(item != NULL) {
        if(item->symbol != STOP) {
            counter++;
        }
        else {
            break;
        }

        item = item->nextItem;
    }

    return counter;
}

void stackDestroy (Stack* s)
{

    if(stackEmpty(s))
    {
        return;
    }

    for(int top = s->TopCounter; top >= 0; top--)
    {
        stackPop(s);
    }
}

