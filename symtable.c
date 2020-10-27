#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "symtable.h"

#define MAX_ST_SIZE 1000 // size of hash table

// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY
// ALMOST READY ALMOST READY ALMOST READY


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
		if (symtab->items[index]->key == tmpitem->key) {
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
		
		TListItem *tmpitem = symtab->items[index];
		while(tmpitem != NULL) {
		
			if (tmpitem->nextItem == NULL){
				free(tmpitem);
				break;
			} else {
				tmpitem = tmpitem->nextItem;
			}
		}
	}
}



/*
* Function accesses/copy a table element
* returns
*/
void symTableCopy();



void symTableDump(TSymTable *symtab) {
    
	for (int i = 0; i < MAX_ST_SIZE; ++i) {
        
		TListItem *tmpitem = symtab->items[i];

        if (tmpitem == NULL) {
            continue;
        }

        printf("slot[%4d]: ", i);

        for(;;) {
            printf(" [%s]\t ", tmpitem->key);

            if (tmpitem->nextItem == NULL) {
                break;
            }

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



int main() {

	TSymTable *symtab = symTableInit();
	
	TData *data1 = malloc(sizeof(TData));
	TData *data2 = malloc(sizeof(TData));
	TData *data3 = malloc(sizeof(TData));
	TData *data4 = malloc(sizeof(TData));
	TData *data5 = malloc(sizeof(TData));
	TData *data6 = malloc(sizeof(TData));


	char *key1 = "guyb";
	char *key2 = "llplj8";
	char *key3 = "mnkb4";
	char *key4 = "ppoqa";
	char *key5 = "ttkjnh";
	char *key6 = "Identifier_6";

	symTableInsert(symtab, key1, data1);
	symTableInsert(symtab, key2, data2);
	symTableInsert(symtab, key3, data3);
	symTableInsert(symtab, key4, data4);
	symTableInsert(symtab, key5, data5);
	symTableInsert(symtab, key6, data6);

	symTableDump(symtab);

	symTableDestoy(symtab);

	return 0;
}



//	data1->dataType = TOKEN_INT;
//	data2->dataType = TOKEN_DOUBLE;
//	data3->dataType = TOKEN_STRING;
//	data4->dataType = TOKEN_IDENTIFIER;
//	data5->dataType = TOKEN_KEYWORD;
//	data6->dataType = TOKEN_MINUS;

