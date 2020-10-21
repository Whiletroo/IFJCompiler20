#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"

#define MAX 100 // size of hash table


/* Key for symbol table*/
typedef unsigned char T_Key;


/* Identifier types*/
typedef enum { variable, function, label } TIdType;


/* 
* Symbol table cell type.
* Data that is not needed by the
* identifier type is not filled.
*/
typedef struct TidCell {

	T_Key key;
	char *Identifier;			// pointer to name of identifier
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
typedef struct ThashTable {
	T_Cell **Cells;
} T_hashTable;


/* Hash function
* TODO probably change it
*/
 unsigned int hash(const char *key) {
 	unsigned long int  value = 0;
	unsigned int  i = 0;
 	unsigned int key_len = strlen(key);

 	for (; i < key_len; i++) {
 		value = value * 37 + key[i];
 	}
 
	return value % MAX;
 }


T_hashTable *hashTableInit(void) {

	// memory allocatoin for table
	T_hashTable *hashTablePtr = malloc(sizeof(T_hashTable));
	if ( hashTablePtr == NULL){
		exit (EXIT_FAILURE);
	}

	// memory allocatoin for pointer to cells
	hashTablePtr->Cells = malloc(sizeof(T_Cell*) * MAX);
	if ( hashTablePtr->Cells == NULL) {
		free(hashTablePtr);
		exit (EXIT_FAILURE);
	}

	// set each NULL
	for(int i = 0; i < MAX; i++){
		hashTablePtr->Cells[i] = NULL;
	}

	return hashTablePtr;
}

void hashTableInsert();

void hashTableDestoy();

bool hashTableSearch();

int main() {

	return 0;
}
