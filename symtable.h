/**
* @file symtable.h
* @author xkrukh00
* @date 21.10.2020
* @brief The header file for symtable.c
*/

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include <stdbool.h>
#include "scanner.h"

#define MAX_ST_SIZE 1000 //size of symbol table

typedef enum { variable, function, UNDEF } TIdType;

typedef enum { INT, FLOAT, BOOL, STRING, NIL} tDataType;


typedef struct {

	char *identifier;		// name of the sumbol
    tDataType dataType;     // type of data if it is variable
	TIdType idType;			// type of id: variable, function, label
	bool defined;
	union {
		int int_val;
		double double_val;
		char *string_val;
	};

} TData;


typedef struct symtabitem {
	char *key;
	TData data;
	struct symtabitem *nextItem;
} TSymbolItem;


typedef struct symTable{
	TSymbolItem **items;
} TSymTable;


/**
 * Hash function. Hash is used as index in symbol table
 * 
 * @param key string to be hashed
 * @return hash code
*/
unsigned long int hashf(const char *key);


/**
 * Symbol table initialization. 
 * Allocates memory for symtab and initialize it.
 * 
 * @param symtab pointer to TSymTable.
 * @return exit code. OK if it is ok, else ERR_INTERNAL.
*/
int symTableInit(TSymTable *symtab);


/**
 * Inserts new symbol into symbol table. If symbol with same key 
 * does exists, will insert it before existing symbol. Data 
 * of symbol are initialized to default values.
 * 
 * @param symtab pointer to TSymTable.
 * @param key key of the symbol table item. (Name of identifier).
 * @return exit code. OK if it is ok, else ERR_INTERNAL.
*/
int symTableInsert(TSymTable *symtab, char *key);


/**
 * Check for existence a symbol with entered key.
 * 
 * @param symtab pointer to TSymTable.
 * @param key key of the symbol table item. (Name of identifier).
 * @return true if symbol is found, else false
*/
bool symTableSearch(TSymTable *symtab, char *key);

/**
 * Delete item with by key.
 * 
 * @param symtab pointer to TSymTable.
 * @param key key of the symbol table item. (Name of identifier).
*/
void symTabDeleteItem(TSymTable *symtab, char *key);

/**
 * Getting a pointer to data of symbol with entered key.
 * Use it for inserting, or changing data.
 * 
 * @param symtab pointer to TSymTable.
 * @param key key of the symbol table item. (Name of identifier).
 * @return TData* pointer to data.
*/
TData *symTableGetItem(TSymTable *symtab, char *key);

/**
 * Fully destroy a symbol table. Free allocated memory
 * 
 * @param symtab pointer to TSymTable.
*/
void symTableDestroy(TSymTable *symtab);

#endif