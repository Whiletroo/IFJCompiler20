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

typedef enum { INT, DOUBLE, BOOL, STRING, UNDEF} tDataType;


typedef struct {

	tAttribute *attribute;	// pointer to value (attribute)
	TIdType iDtype;			// type of id: variable, function, label
    tDataType dataType;     // type of data if it is variable
	bool label_found; 		// flag for labels (is it found?)

} TData;


typedef struct symtabitem {
	char *key;
	TData *data;
	struct symtabitem *nextItem;
} TListItem;


typedef struct symTable{
	TListItem **items;
} TSymTable;


unsigned long int hashf(const char *key);

int symTableInit(TSymTable *symtab);

void symTableInsert(TSymTable *symtable, char *key, TData *data);

void symTableActualize(TSymTable *symtable, char *key, TData *data); //TODO

bool symTableSearch(TSymTable *symtable, char *key);

void symTabDeleteItem(TSymTable *symtab, char *key);

TData *symTableGetItem(TSymTable *symtab, char *key);

void symTableDestroy(TSymTable *symtab);

#endif