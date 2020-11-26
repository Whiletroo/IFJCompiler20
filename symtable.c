#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "symtable.h"
#include "error.h"

#define MAX_ST_SIZE 100 // size of hash table. set MAX_ST_SIZE to 4 for test(uncomment main func)

// TODO add comments


/* Hash function
* TODO probably change it
*/
unsigned long int hashf(const char *key) {

 	unsigned long int  value = 0;
	unsigned int  i = 0;
 	unsigned int key_len = strlen(key);

 	for (; i < key_len; i++) {
 		value = value * 37 + key[i];
 	}
 
	return value % MAX_ST_SIZE;
}



/*
* Functiond allocates memory
* for symbol table.
*/
int symTableInit(TSymTable *symtab) {

	// memory allocatoin for table
	symtab = malloc(sizeof(TSymTable));
	if ( symtab == NULL ){
		return ERR_INTERNAL;
	}

	// memory allocatoin for pointer to items
	symtab->items = malloc(sizeof(TListItem*) * MAX_ST_SIZE);
	if ( symtab->items == NULL) {
		free(symtab);
		return ERR_INTERNAL;
	}

	// set each NULL
	for(int i = 0; i < MAX_ST_SIZE; i++){
		symtab->items[i] = NULL;
	}

	return OK;
}



/*
* Function indicates if еhe symbol 
* with the entered key is in table.
* Returns true if found, or false
* if item istn found.
*/
bool symTableSearch(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		exit(EXIT_FAILURE);
	}

	bool found = false;
	unsigned long int index = hashf(key);
	TListItem* tmpitem = symtab->items[index];

	while(!found && tmpitem != NULL)  {
		if (!strcmp(tmpitem->key, key)) {
			found = true;
		}
		tmpitem = tmpitem->nextItem;
	}
	return found;
}



/*
* Function insert new item with entered
* key and data. If item(s) with enered key exists
* insetr a new item to end of list of items
*/
void symTableInsert(TSymTable *symtab, char *key, TData *data){
	
	if (symtab == NULL) {
		exit(EXIT_FAILURE);
	}

	unsigned long int index = hashf(key);

	if (symtab->items[index] == NULL) { // if entry is empty
		
		symtab->items[index] = malloc(sizeof(TListItem));
		if (symtab->items[index] == NULL) {
			exit(EXIT_FAILURE);
		}
		symtab->items[index]->nextItem = NULL;
		symtab->items[index]->key = key;
		symtab->items[index]->data = data;

	} else {							// if entry is not empty

		TListItem *tmpitem = symtab->items[index];

		while(tmpitem->nextItem != NULL ) { 			// set tmpitem to last position or
			if (!strcmp(tmpitem->nextItem->key, key)){	// position before a cell with same key
				break;
			}
			tmpitem = tmpitem->nextItem;
		}

		if (tmpitem->nextItem == NULL) {  				// if position is the last position

			tmpitem->nextItem = malloc(sizeof(TListItem));
			tmpitem->nextItem->key = key;
			tmpitem->nextItem->data = data;
			tmpitem->nextItem->nextItem = NULL;

		} else {										// if position is position before item with a same key

			TListItem *newitem = malloc(sizeof(TListItem));
			newitem->key = key;
			newitem->data = data;
			newitem->nextItem = tmpitem->nextItem;
			tmpitem->nextItem = newitem;

		}
	}
}

void symTableActualize(TSymTable *symtable, char *key, TData *data) {
	
}

/*
* Function delete the last symbol
* with entered key.
*/
void symTabDeleteItem(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		exit(EXIT_FAILURE);
	}

	unsigned long int index = hashf(key);

	TListItem *actitem, *previtem;
	actitem = symtab->items[index];
	previtem = NULL;

	while(actitem != NULL){
		if (!strcmp(actitem->key, key)){
			break;
		}
		previtem = actitem;
		actitem = actitem->nextItem;
	}
	
	if (actitem != NULL){
		
		if(previtem == NULL) {
			symtab->items[index] = NULL;
		} else {
			previtem->nextItem = actitem->nextItem;
		}

		free(actitem->data);
		free(actitem);
	}

}


/*
*Function returns a pointer to last item with entered key
* Returns NULL of item isnt found
*/
TData *symTableGetItem(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		exit(EXIT_FAILURE);
	} else {

		TData *data = NULL;
		unsigned long int index = hashf(key);
		TListItem* tmpitem = symtab->items[index];

		if (tmpitem != NULL) {			
			while( tmpitem != NULL )  {
				if (!strcmp(tmpitem->key, key)) {
					data = tmpitem->data;
					return data;
				}
				tmpitem = tmpitem->nextItem;
			}
		}
		return data;
	}
}



/*
* Functiond prints all symtable items
*/
void symTableDump(TSymTable *symtab) {
    
	for (int index = 0; index < MAX_ST_SIZE; index++) {
        
		TListItem *tmpitem = symtab->items[index];

        if (tmpitem == NULL) {
            continue;
        }

        printf("slot[%4d]: ", index);

        while (tmpitem != NULL) {
            printf(" [%s]\t ", tmpitem->key);
            tmpitem = tmpitem->nextItem;
        }

        printf("\n");
    }
	printf("\n");
}



/*
* Frees allocated memory for symbol table
*/
void symTableDestroy(TSymTable *symtab) {

	if (symtab != NULL) {
		
		TListItem *tmpitem ,*nextitem;

		for( unsigned long int index = 0; index < MAX_ST_SIZE; index++) {

			for( tmpitem = symtab->items[index]; tmpitem != NULL; tmpitem = nextitem) {

				nextitem = tmpitem->nextItem;
				free(tmpitem->data);
				free(tmpitem);
			}
		}
		
		free(symtab->items);
		free(symtab);
	}
}