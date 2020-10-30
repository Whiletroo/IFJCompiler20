/**
* @file symtable.h
* @author xkrukh00
* @date 21.10.2020
* @brief The header file for symtable.c
*/

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "scanner.h"


typedef enum { variable, function, label } TIdType;


typedef struct {

	char *id;
	TIdType iDtype;			// type of id: variable, function, label(for jumps)
    TOKENS dataType;        // type of data
	void *valuePtr;			// for variables
	unsigned int pos;       // not sure
	bool defined;
	bool found; 		    // for label. was the target found?

} TData;



typedef struct symtabitem {
	char *key;
	TData *data;
	struct symtabitem *nextItem;
} TListItem;


typedef struct {
	TListItem **items;
} TSymTable;



unsigned long int hashf(const char *key);

TSymTable *symTableInit();

void symTableInsert(TSymTable *symtable, char *key, TData *data);

bool symTableSearch(TSymTable *symtable, char *key);

void symTabDeleteItem(TSymTable *symtab, char *key);

TData *symTableGetItem(TSymTable *symtab, char *key);

void symTableDestoy(TSymTable *symtab);

#endif