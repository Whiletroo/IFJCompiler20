/**
* @file symtable.h
* @author xkrukh00
* @date 21.10.2020
* @brief The header file for symtable.c
*/

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "scanner.h"

typedef enum { variable, function, UNDEF } TIdType;

typedef enum { INT, DOUBLE, BOOL, STRING, UNDEF} tDataType;


typedef struct {

	tAttribute *attribute;	// pointer to value (attribute)
	TIdType iDtype;			// type of id: variable, function, label
    tDataType dataType;     // type of data if it is variable

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

int symTableInsert(TSymTable *symtable, char *key, TData *data);


/**
 * Allocates memry for TData in symbol table item. Use it Before calling symTableInstert.
 * 
 * @param data pointer to which will be assigned the value of the pointer to initialized TData
 * @return Exit code. OK (0) if it is ok.
*/
int symTableDataInit(TData *data)


void symTableActualize(TSymTable *symtable, char *key, TData *data); //TODO

bool symTableSearch(TSymTable *symtable, char *key);

void symTabDeleteItem(TSymTable *symtab, char *key);

TData *symTableGetItem(TSymTable *symtab, char *key);

void symTableDestroy(TSymTable *symtab);

#endif