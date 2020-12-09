/**
* @file symtable.h
* @author xkrukh00
* @date 21.10.2020
* @brief The header file for symtable.c
*/

#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include <stdbool.h>


#define MAX_ST_SIZE 10 //size of symbol table
#define MAX_RETURN_TYPES 10
#define MAX_PARAMETERS 10

typedef enum {UNDEF, variable, function} TIdType;

typedef enum {UNDEFINED_TYPE, INT_TYPE, FLOAT_TYPE, STRING_TYPE, BOOLEAN_TYPE,NIL_TYPE} tDataType;


typedef struct {

	char *identifier;							// name of the sumbol
	TIdType idType;								// type of id: variable, function, label
	bool defined;

    tDataType dataType[MAX_RETURN_TYPES];     	// type of data if it is variable, array of returns type if it is function
	tDataType funcParams[MAX_PARAMETERS];		// array of types for function parameters
	
	struct symTable *localTable;

	// maybe uneeded
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


typedef struct symTable {
	TSymbolItem *items[MAX_ST_SIZE];
} TSymTable;


/**
 * Hash function. Hash is used as index in symbol table
 * 
 * @param key string to be hashed
 * @return hash code
*/
int hashf(const char *key);


/**
 * Symbol table initialization. 
 * Allocates memory for symtab and initialize it.
 * 
 * @return pointer to intialized symtable or NULL if error.
*/
TSymTable *symTableInit();


/**
 * Initialize a structue TData
 * 
 * @param key pointer to char, name of identifier
 * @param painter to TData
 * @return exit code
*/
int symTableInitData(TData *data, char *key);


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
 * @return exit code. OK if it is ok, else ERR_INTERNAL.
*/
int symTabDeleteItem(TSymTable *symtab, char *key);

/**
 * Getting a pointer to data of symbol with entered key.
 * Use it for inserting, or changing data.
 * 
 * @param symtab pointer to TSymTable.
 * @param key key of the symbol table item. (Name of identifier).
 * @return TData* pointer to data. NULL if symbol doesn't exist.
*/
TData *symTableGetItem(TSymTable *symtab, char *key);


/**
 * Fully destroy a symbol table. Free allocated memory
 * 
 * @param symtab pointer to TSymTable.
*/
void symTableDestroy(TSymTable *symtab);



/******* DEBUG FUNCTIONS *********/


/**
 * Print to stdout FULL symbol table
 * 
 * @param symtab pointer to symbol table
*/
void printTable(TSymTable *symtab);


/**
 * Print to stdout data of symbol (symbol table item)
 * 
 * @param data pointer to data of symbol
*/
void printData(TData *data);


#endif
