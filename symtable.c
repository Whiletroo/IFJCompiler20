#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "symtable.h"

#define MAX_ST_SIZE 100 // size of hash table

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
TSymTable *symTableInit() {

	// memory allocatoin for table
	TSymTable *symtab = malloc(sizeof(TSymTable));
	if ( symtab == NULL ){
		exit (EXIT_FAILURE);
	}

	// memory allocatoin for pointer to items
	symtab->items = malloc(sizeof(TListItem*) * MAX_ST_SIZE);
	if ( symtab->items == NULL) {
		free(symtab);
		exit (EXIT_FAILURE);
	}

	// set each NULL
	for(int i = 0; i < MAX_ST_SIZE; i++){
		symtab->items[i] = NULL;
	}

	return symtab;
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
		if (tmpitem->key == key) {
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

	if (symtab->items[index] == NULL) {
		
		symtab->items[index] = malloc(sizeof(TListItem));
		if (symtab->items[index] == NULL) {
			exit(EXIT_FAILURE);
		}
		symtab->items[index]->nextItem = NULL;
		symtab->items[index]->key = key;
		symtab->items[index]->data = data;

	} else {

		TListItem *tmpitem = symtab->items[index];
		while(tmpitem->nextItem != NULL){
			tmpitem = tmpitem->nextItem;
		}

		tmpitem->nextItem = malloc(sizeof(TListItem));

		if (tmpitem->nextItem == NULL){
			exit(EXIT_FAILURE);
		}

		tmpitem->nextItem->key = key;
		tmpitem->nextItem->data = data;
		tmpitem->nextItem->nextItem = NULL;

	}
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

	if (symtab->items[index] != NULL) {
		
		if (symtab->items[index]->nextItem == NULL) {

			free(symtab->items[index]->data);
			free(symtab->items[index]);
			symtab->items[index] = NULL;

		} else {

			TListItem *previtem = symtab->items[index];
			TListItem *nextitem = symtab->items[index]->nextItem;

			while(nextitem->nextItem != NULL) {

				previtem = nextitem;
				nextitem = nextitem->nextItem;
			}

			free(nextitem->data);
			free(nextitem);
			previtem->nextItem = NULL;
		}
	}
}


/*
*Function returns a pointer to last item with entered key
*/
TData *symTableGetItem(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		exit(EXIT_FAILURE);
	} else {

		TData *data;
		unsigned long int index = hashf(key);
		TListItem* tmpitem = symtab->items[index];

		if (tmpitem != NULL) {
			while( tmpitem->nextItem != NULL && strcmp(tmpitem->key, key))  {
				tmpitem = tmpitem->nextItem;
			}
		}

		data = tmpitem->data;
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
}



/*
* Frees allocated memory for symbol table
*/
void symTableDestoy(TSymTable *symtab) {

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
