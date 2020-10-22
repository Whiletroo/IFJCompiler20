#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "symtable.h"

#define MAX_ST_SIZE 100 // size of hash table


/* Hash function
* TODO probably change it
*/
 unsigned int hashf(const char *key) {
 	unsigned long int  value = 0;
	unsigned int  i = 0;
 	unsigned int key_len = strlen(key);

 	for (; i < key_len; i++) {
 		value = value * 37 + key[i];
 	}
 
	return value % MAX_ST_SIZE;
 }


T_symTable *hashTableInit(void) {

	// memory allocatoin for table
	T_symTable *hashTablePtr = malloc(sizeof(T_symTable));
	if ( hashTablePtr == NULL){
		exit (EXIT_FAILURE);
	}

	// memory allocatoin for pointer to cells
	hashTablePtr->Cells = malloc(sizeof(T_Cell*) * MAX_ST_SIZE);
	if ( hashTablePtr->Cells == NULL) {
		free(hashTablePtr);
		exit (EXIT_FAILURE);
	}

	// set each NULL
	for(int i = 0; i < MAX_ST_SIZE; i++){
		hashTablePtr->Cells[i] = NULL;
	}

	return hashTablePtr;
}

void hashTableInsert();

void hashTableChange();

void hashTableDestoy();

bool hashTableSearch();

int main() {

	return 0;
}