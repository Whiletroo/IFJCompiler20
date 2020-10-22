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
* Symbol table cell type.
* Data that is not needed by the
* identifier type is not filled.
*/
typedef struct TidCell {

	char *key;			// pointer to name of identifier
	TIdType type;				// type of id: variable, function, label(for jumps)

	void *valuePtr;				// for variables
	unsigned int pos;

	TOKENS **inputTypes;		// for functions. TODO change it to dynamic
	TOKENS **returnTypes;				
	bool defined;

	bool found; 				// for label. was the target found?

	struct TidCell *next;

} T_Cell;


/* Hash table type
* pointer to array of po
*/
typedef struct {
	T_Cell **Cells;
} T_symTable;


#endif