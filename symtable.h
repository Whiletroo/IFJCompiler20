/**
* @file symtable.h
* @author xkrukh00
* @date 21.10.2020
* @brief The header file for symtable.c
*/

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "scanner.h"

#define MAX_ST_SIZE 100 // size of symbol table

typedef enum { variable, function, UNDEF } TIdType;

typedef enum { INT, DOUBLE, BOOL, STRING, UNDEF} tDataType;



/**
 * @struct 
*/
typedef struct {

    tDataType dataType;     // type of data if it is variable

	tAttribute *attribute;	// pointer to value (attribute)

	TIdType iDtype;			// type of id: variable, function, label



} TData;

/**
 * @struct TODO
*/
typedef struct symtabitem {
	char *key;
	TData *data;
	struct symtabitem *nextItem;
} TListItem;


typedef struct symTable{
	TListItem **items;
} TSymTable;



/**
 * Hash function.
 * 
 * @param key is a string that will be hashed.
 * @return hash value that is using as index in symbol table.
*/
unsigned long int hashf(const char *key);


/**
 * Initialize a symbol table.
 * 
 * @param symtab poiner to TSymTable.
 * @return Exit code.
*/
int symTableInit(TSymTable *symtab);


/**
 * Allocates memry for TData in symbol table item. Use it Before calling symTableInstert.
 * 
 * @param data pointer to which will be assigned the value of the pointer to initialized TData.
 * @return Exit code. OK (0) if it is ok.
*/
int symTableDataInit(TData *data);


/**
 * Insert a new symbol into table.
 * 
 * @param symtable pointer to symbol table.
 * @param key is key, a string that contains a name
 * @param data is pointer to TData.
*/
int symTableInsert(TSymTable *symtable, char *key, TData *data);


/**
 * Looking for symbol.
 * 
 * @param symtable pointer to symbol table.
 * @param key is key, a string that contains a name 
 * @return true if symbol exists, else false.
*/
bool symTableSearch(TSymTable *symtable, char *key);


/**
 * Changing data of symbol. If symbol with entered key
 * does not exits - nothing to do.
 * 
 * @param symtable pointer to symbol table.
 * @param key is key, a string that contains a name.
 * @param data a new data. Pointer to TData.
*/
void symTableActualize(TSymTable *symtable, char *key, TData *data); //TODO


/**
 * Delete symbol by a key. If symbol with entered key
 * does not exits - nothing to do.
 * 
 * @param symtable pointer to symbol table.
 * @param key is key, a string that contains a name.
*/
void symTabDeleteItem(TSymTable *symtab, char *key);


/**
 * Get pointer to data of symbol with entered key
 * 
 * @param symtable pointer to symbol table.
 * @param key is key, a string that contains a name.
 * @return pointer to TData.
*/
TData *symTableGetItem(TSymTable *symtab, char *key);


/**
 * Fully destroy symbol table. Free allocated memory.
 * 
 * @param symtable pointer to symbol table.
*/
void symTableDestroy(TSymTable *symtab);



#endif