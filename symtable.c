#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "symtable.h"
#include "error.h"


unsigned long int hashf(const char *key) {

 	unsigned long int  value = 0;
	unsigned int  i = 0;
 	unsigned int key_len = strlen(key);

 	for (; i < key_len; i++) {
 		value = value * 37 + key[i];
 	}
 
	return value % MAX_ST_SIZE;
}



int symTableInit(TSymTable *symtab) {

	// memory allocatoin for table
	symtab = malloc(sizeof(TSymTable));
	if ( symtab == NULL ){
		return ERR_INTERNAL;
	}

	// memory allocatoin for pointer to items
	symtab->items = malloc(sizeof(TSymbolItem*) * MAX_ST_SIZE);
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



bool symTableSearch(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		exit(EXIT_FAILURE);
	}

	bool found = false;
	unsigned long int index = hashf(key);
	TSymbolItem* tmpitem = symtab->items[index];

	while(!found && tmpitem != NULL)  {
		if (!strcmp(tmpitem->key, key)) {
			found = true;
		}
		tmpitem = tmpitem->nextItem;
	}
	return found;
}



int symTableInsert(TSymTable *symtab, char *key, TData *data){
	
	if (symtab == NULL) {
		return ERR_INTERNAL;
	}

	unsigned long int index = hashf(key);

	if (symtab->items[index] == NULL) { // if entry is empty
		
		symtab->items[index] = malloc(sizeof(TSymbolItem));
		if (symtab->items[index] == NULL) {
			return ERR_INTERNAL;
		}

		symtab->items[index]->nextItem = NULL;
		strcpy(symtab->items[index]->key ,key);

		// setting data values
		strcpy(symtab->items[index]->data.identifier, key);
		symtab->items[index]->data.dataType = NIL;
		symtab->items[index]->data.idType = UNDEF;
		symtab->items[index]->data.defined = false;
		symtab->items[index]->data.string_val = NULL ; // setting only of value of union

		return OK;

	} else {							// if entry is not empty

		TSymbolItem *tmpitem = symtab->items[index];

		while(tmpitem->nextItem != NULL ) { 			// set tmpitem to last position or
			if (!strcmp(tmpitem->nextItem->key, key)){	// position before a cell with same key
				break;
			}
			tmpitem = tmpitem->nextItem;
		}

		if (tmpitem->nextItem == NULL) {  				// if position is the last position

			tmpitem->nextItem = malloc(sizeof(TSymbolItem));
			if (tmpitem->nextItem == NULL){
				return ERR_INTERNAL;
			}

			strcpy(tmpitem->nextItem->key, key);
			strcpy(tmpitem->nextItem->data.identifier, key);
			tmpitem->nextItem->data.dataType = NIL;
			tmpitem->nextItem->data.idType = UNDEF;
			tmpitem->nextItem->data.defined = false;
			tmpitem->nextItem->data.string_val = NULL;

			return OK;

		} else {										// if position is position before item with a same key


			TSymbolItem *newitem = malloc(sizeof(TSymbolItem));
			if (newitem == NULL) {
				return ERR_INTERNAL;
			}

			strcpy(newitem->key, key);
			strcpy(newitem->data.identifier, key);
			newitem->data.dataType = NIL;
			newitem->data.idType = UNDEF;
			newitem->data.defined = false;
			newitem->data.string_val = NULL;

			newitem->nextItem = tmpitem->nextItem;
			tmpitem->nextItem = newitem;

			return OK;

		}
	}
}



TData *symTableGetItem(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		return ERR_INTERNAL;
	} else {

		TData *data = NULL;
		unsigned long int index = hashf(key);
		TSymbolItem* tmpitem = symtab->items[index];

		if (tmpitem != NULL) {			
			while( tmpitem != NULL )  {
				if (!strcmp(tmpitem->key, key)) {
					data = &tmpitem->data;
					return data;
				}
				tmpitem = tmpitem->nextItem;
			}
		}
		return data;
	}
}



void symTabDeleteItem(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		return ERR_INTERNAL;
	}

	unsigned long int index = hashf(key);

	TSymbolItem *actitem, *previtem;
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

		free(actitem);
	}

}



void symTableDump(TSymTable *symtab) {
    
	for (int index = 0; index < MAX_ST_SIZE; index++) {
        
		TSymbolItem *tmpitem = symtab->items[index];

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



void symTableDestroy(TSymTable *symtab) {

	if (symtab != NULL) {
		
		TSymbolItem *tmpitem ,*nextitem;

		for( unsigned long int index = 0; index < MAX_ST_SIZE; index++) {

			for( tmpitem = symtab->items[index]; tmpitem != NULL; tmpitem = nextitem) {

				nextitem = tmpitem->nextItem;
				free(tmpitem);
			}
		}
		
		free(symtab->items);
		free(symtab);
	}
}