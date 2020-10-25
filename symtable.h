/**
* @file symtable.h
* @author xkrukh00
* @date 21.10.2020
* @brief The header file for symtable.c
*/

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "scanner.h"


/* Identifier types*/
typedef enum { variable, function, label } TIdType;


/* 
* Type for containing data about identifier
*/
typedef struct {

	char *key;			    // name of identifier
	TIdType iDtype;			// type of id: variable, function, label(for jumps)
    TOKENS dataType;        // type of data
	void *valuePtr;			// for variables
	unsigned int pos;       // not sure
	bool defined;
	bool found; 		    // for label. was the target found?

} TData;

/*
* 
*/
typedef struct symtabitem {
	char *key;
	TData *data;
	struct symtabitem *nextItem;
} TListItem;


/* Hash table type
* pointer to array of po
*/
typedef struct {
	TListItem **items;
} TSymTable;

unsigned long int hashf(const char *key);

TSymTable *symTableInit();

void symTableInsert(TSymTable *symtable, char *key, TData *data);

bool symTableSearch(TSymTable *symtable, char *key);

void symTabDeleteItem(TSymTable *symtab, char *key);

void symTableCopy();

void symTableDestoy(TSymTable *symtab);

#endif