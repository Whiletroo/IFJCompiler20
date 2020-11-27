#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"
#include "symtable.h"
#include "error.h"


int hashf(const char *key) {

 	int  value = 0;
	unsigned int  i = 0;
 	unsigned int key_len = strlen(key);

 	for (; i < key_len; i++) {
 		value = value * 37 + key[i];
 	}
 
	return value % MAX_ST_SIZE;
}



TSymTable *symTableInit() {

	TSymTable *symtab;
	symtab = malloc(sizeof(TSymTable));
	if (symtab == NULL ) {
		return NULL;
	}

	for (int i = 0; i < MAX_ST_SIZE; i++) {
		symtab->items[i] = NULL;
	}

	return symtab;
}



bool symTableSearch(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		exit(EXIT_FAILURE);
	}

	bool found = false;
	int index = hashf(key);
	TSymbolItem* tmpitem = symtab->items[index];

	while(!found && tmpitem != NULL)  {
		if (!strcmp(tmpitem->key, key)) {
			found = true;
		}
		tmpitem = tmpitem->nextItem;
	}
	return found;
}



int symTableInsert(TSymTable *symtab, char *key){
	
	if (symtab == NULL) {
		return ERR_INTERNAL;
	}

	int index = hashf(key);

	if (symtab->items[index] == NULL) { // if entry is empty
		
		symtab->items[index] = (TSymbolItem *)malloc(sizeof(TSymbolItem));
		if (symtab->items[index] == NULL) {
			return ERR_INTERNAL;
		}

		// setting symbol item values
		symtab->items[index]->nextItem = NULL;
		symtab->items[index]->key = malloc(sizeof(char));
		memcpy(symtab->items[index]->key, key, strlen(key));		

		// allocating memory for data
		symtab->items[index]->data = *(TData *)malloc(sizeof(TData));
		symtab->items[index]->data.identifier = malloc(sizeof(char));

		// setting data values
		memcpy(symtab->items[index]->data.identifier, key, strlen(key));
		symtab->items[index]->data.dataType = NIL_TYPE;
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
			tmpitem->nextItem->data.dataType = NIL_TYPE;
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
			newitem->data.dataType = NIL_TYPE;
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
		return NULL;
	} else {

		TData *data = NULL;
		int index = hashf(key);
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



int symTabDeleteItem(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		return ERR_INTERNAL;
	}

	int index = hashf(key);

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
	return OK;

}



void symTableDump(TSymTable *symtab) {
    
	for (int index = 0; index < MAX_ST_SIZE; index++) {
        
		TSymbolItem *tmpitem = symtab->items[index];

        if (tmpitem == NULL) {
            continue;
        }

        printf("slot[%d]: ", index);

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

		// iteration over table
		for( int index = 0; index < MAX_ST_SIZE; index++) {

			// iteration over list
			for( tmpitem = symtab->items[index]; tmpitem != NULL; tmpitem = nextitem) {
				
				if (tmpitem->nextItem != NULL) {
					nextitem = tmpitem->nextItem;
				}
				free(tmpitem->key);
				free(tmpitem->data.identifier);
				free(&tmpitem->data);
				free(tmpitem);
			}
		}
		
		free(symtab->items);
		free(symtab);
	}
}

void printData(TData *data) {

	printf("Identifier  	:\t%s\n",data->identifier);
	printf("Type of DATA    :\t");
	switch(data->dataType) {
		case INT_TYPE:
			printf("INT\n");
			break;
		case FLOAT_TYPE:
			printf("FLOAT\n");
			break;
		case STRING_TYPE:
			printf("STRING\n");
			break;
		default:
			printf("NIL\n");
			break;
	}
	printf("Defined         :\t%s\n",((data->defined) ? "true":"false"));
	printf("Identifier type :\t");

	switch(data->idType) {
		case variable:
			printf("variable\n");
			break;
		case function:
			printf("function\n");
			break;
		default:
			printf("undef\n");
			break;
	}

	if (data->dataType == STRING_TYPE) {
		printf("String value    :\t%s\n",data->string_val);
	} else if (data->dataType == INT_TYPE) {
		printf("Int value     :\t%d\n",data->int_val);
	} else if (data->dataType == FLOAT_TYPE) {
		printf("Doubel value   :\t%lf\n",data->double_val);
	} else {
		printf("Value undifiened\n");
	}
}

int main() {

	TSymTable *symtab;
	TData *data;
	symtab = symTableInit();
	symTableInsert(symtab, "key1");
	data = symTableGetItem(symtab, "key1");
	data->dataType = STRING_TYPE;
	data->defined = true;
	data->idType = variable;
	data->string_val = "symbol1";

	printData(data);

	symTableDestroy(symtab);


	return 0;
}