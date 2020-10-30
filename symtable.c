#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "symtable.h"

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

		/*while(tmpitem->nextItem != NULL ){
			tmpitem = tmpitem->nextItem;
		}

		tmpitem->nextItem = malloc(sizeof(TListItem));

		if (tmpitem->nextItem == NULL){
			exit(EXIT_FAILURE);
		}

		tmpitem->nextItem->key = key;
		tmpitem->nextItem->data = data;
		tmpitem->nextItem->nextItem = NULL;*/
	}
}



/* TODO переделать

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

/*
// Simple test
int main(){

	TData *data1 = malloc(sizeof(TData));
	TData *data2 = malloc(sizeof(TData));
	TData *data3 = malloc(sizeof(TData));
	TData *data4 = malloc(sizeof(TData));
	TData *data5 = malloc(sizeof(TData));
	TData *data6 = malloc(sizeof(TData));
	TData *data7 = malloc(sizeof(TData));
	TData *data8 = malloc(sizeof(TData));
	TData *data9 = malloc(sizeof(TData));
	TData *data10 = malloc(sizeof(TData));
	TData *data11 = malloc(sizeof(TData));
	TData *data12 = malloc(sizeof(TData));

	TSymTable *symtab = symTableInit();

	data10->id = "first inserted name10";

	symTableInsert(symtab, "name1", data1);
	symTableInsert(symtab, "name2", data2);
	symTableInsert(symtab, "name3", data3);
	symTableInsert(symtab, "name4", data4);
	symTableInsert(symtab, "name5", data5);
	symTableInsert(symtab, "name6", data6);
	symTableInsert(symtab, "name7", data7);
	symTableInsert(symtab, "name8", data8);
	symTableInsert(symtab, "name9", data9);
	symTableInsert(symtab, "name10", data10);
	symTableInsert(symtab, "name11", data11);
	symTableInsert(symtab, "name12", data12);

	symTableDump(symtab);


	printf("Getting data from name 10\n");
	TData *tmpdata = symTableGetItem(symtab, "name10");
	printf("Data from name10 : \"%s\"\n", tmpdata->id);


	printf("Insert new name10\n");
	TData *mydata = malloc(sizeof(TData));
	mydata->id = "last inserted name 10";
	symTableInsert(symtab, "name10", mydata);
	symTableDump(symtab);


	printf("Getting data from name 10\n");
	tmpdata = symTableGetItem(symtab, "name10");
	printf("Data from name10 : \"%s\"\n", tmpdata->id);


	printf("\nCall delete functoin with arg \"name10\"\n");
	symTabDeleteItem(symtab, "name10");
	printf("Result :\n");
	symTableDump(symtab);


	printf("Getting data from name 10\n");
	tmpdata = symTableGetItem(symtab, "name10");
	printf("Data from name10 : \"%s\"\n", tmpdata->id);

	
	printf("\nCall delete functoin with arg \"name3\"\n");
	symTabDeleteItem(symtab, "name3");
	printf("Result :\n");
	symTableDump(symtab);


	printf("\nCall delete functoin with arg \"name8\"\n");
	symTabDeleteItem(symtab, "name8");
	printf("Result :\n");
	symTableDump(symtab);


	printf("\nCall delete functoin with arg \"name4\"\n");
	symTabDeleteItem(symtab, "name4");
	printf("Result :\n");
	symTableDump(symtab);


	printf("\nCall insert function, insert back name8\n");
	data8 = malloc(sizeof(TData));
	symTableInsert(symtab, "name8", data8);
	printf("Result :\n");
	symTableDump(symtab);


	symTableDestoy(symtab);

	return 0;
}
*/