/**
 * @file symtable.c
 * @author xkrukh00
 * @date 21.10.2020
 * @brief Implementation of symbol teable.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtable.h"
#include "error.h"


#define CHECK_PTR(ptr)\
	if (ptr == NULL) {\
		return ERR_INTERNAL;\
	}

#define P(str) printf("%s\n", str); //Macro for debuging

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
		return false;
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


int symTableInitData(TData *data, char *key) {

	data->identifier = malloc(sizeof(char) * (strlen(key) + 1 ));
	CHECK_PTR(data->identifier);
	strcpy(data->identifier, key);

	for (int i = 0; i < MAX_RETURN_TYPES; i++) {
		data->dataType[i] = UNDEFINED_TYPE;
	}

	for (int i = 0; i < MAX_PARAMETERS; i++) {
		data->funcParams[i] = UNDEFINED_TYPE;
	}

	data->idType = UNDEF;
	data->defined = false;
	data->string_val = NULL;
	data->localTable = NULL;

	return OK;

}


int symTableInsert(TSymTable *symtab, char *key) {
	
	if (symtab == NULL) {
		return ERR_INTERNAL;
	}

	int index = hashf(key);

	if ( symtab->items[index] == NULL) { // if entry is empty

		// allocate memory
		symtab->items[index] = malloc(sizeof(TSymbolItem));
		CHECK_PTR(symtab->items[index]);

		// init key value
		symtab->items[index]->key = malloc(sizeof(char) * (strlen(key) + 1 ));
		CHECK_PTR(symtab->items[index]->key);
		strcpy(symtab->items[index]->key, key);

		// set data to default
		symTableInitData(&symtab->items[index]->data, key);

		symtab->items[index]->nextItem = NULL;

		return OK;

	} else {							// if entry is not empty
		
		// allocate memory
		TSymbolItem *newitem = malloc(sizeof(TSymbolItem));
		CHECK_PTR(newitem);

		// init key value
		newitem->key = malloc(sizeof(char) * (strlen(key) + 1 ));
		CHECK_PTR(newitem->key);
		strcpy(newitem->key, key);

		// set data to default
		symTableInitData(&newitem->data, key);

		// insert new item to start of sinonym list
		newitem->nextItem = symtab->items[index];
		symtab->items[index] = newitem;

		return OK;

	}
}


TData *symTableGetItem(TSymTable *symtab, char *key) {

	if (symtab == NULL) {
		return NULL;
	} else {

		int index = hashf(key);
		TSymbolItem* tmpitem = symtab->items[index];

		if (tmpitem != NULL) {			
			while( tmpitem != NULL )  {
				if (!strcmp(tmpitem->key, key)) {
					return &tmpitem->data;
				}
				tmpitem = tmpitem->nextItem;
			}
			return NULL;
		}
		return NULL;
	}
}


int symTableAppendParams(TData *data, char *id, tDataType dataType) {

	// check if is place in array of parameters types
	if (data->funcParams[MAX_PARAMETERS - 1] != UNDEFINED_TYPE) {
		return ERR_INTERNAL;
	}

	// insert a type of func parametr if array of func parametrs
	for (int i = 0; i < MAX_PARAMETERS; i++) {
		if (data->funcParams[i] == UNDEFINED_TYPE) {
			data->funcParams[i] = dataType;
		}
	}

	// insert a new identifiers to symbol table of function (local symbol table)
	data->localTable = symTableInit();
	CHECK_PTR(data->localTable);

	symTableInsert(data->localTable, id);
	TData *tmpptr = symTableGetItem(data->localTable, id);
	CHECK_PTR(tmpptr);

	tmpptr->dataType[0] = dataType;
	tmpptr->defined = false;

	tmpptr->identifier = malloc(sizeof(char) * strlen(id));
	CHECK_PTR(tmpptr->identifier);
	strcpy(tmpptr->identifier, id);

	tmpptr->idType = variable;
	tmpptr->localTable = NULL;
	return OK;

}


int symTabDeleteItem(TSymTable *symtab, char *key) {

	CHECK_PTR(symtab);
	int index = hashf(key);
	TSymbolItem *actitem = symtab->items[index];
	TSymbolItem *tmpptr;
	CHECK_PTR(actitem);

	if ( symtab->items[index] != NULL) { // entry is not empty
		
		if (!strcmp(symtab->items[index]->key, key)) { 	// if it is firtst item in the list 
			
			symtab->items[index] = actitem->nextItem;
			free(actitem->data.identifier);
			free(actitem->key);
			free(actitem);

		} else {										// if it is not first
			
			while(actitem != NULL) {
				
				if (actitem->nextItem != NULL) {
					if (!strcmp(actitem->nextItem->key,key)){
						tmpptr = actitem->nextItem;
						actitem->nextItem= actitem->nextItem->nextItem;

						if (tmpptr->data.localTable != NULL) {
							symTableDestroy(tmpptr->data.localTable);
						}

						free(tmpptr->data.identifier);
						free(tmpptr->key);
						free(tmpptr);
					}
				}

				actitem = actitem->nextItem;

			}
		}
	}

	return OK;

}



void symTableDestroy(TSymTable *symtab) {

	if (symtab != NULL) {
		
		TSymbolItem *tmpitem ,*nextitem;

		// iteration over table
		for( int index = 0; index < MAX_ST_SIZE; index++) {

			tmpitem = symtab->items[index];
			while(tmpitem != NULL) {
				nextitem = tmpitem->nextItem;
				free(tmpitem->key);
				free(tmpitem->data.identifier);

				if (tmpitem->data.localTable != NULL) {
					symTableDestroy(tmpitem->data.localTable);
				}	

				free(tmpitem);
				tmpitem = nextitem;
			}
		}
		
		free(symtab);
	}
}


void printData(TData *data) {

	printf("\tIdentifier  	:\t%s\n",data->identifier);
	printf("\tType of DATA    :\t");
	switch(data->dataType[0]) {
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
	printf("\tDefined         :\t%s\n",((data->defined) ? "true":"false"));
	printf("\tIdentifier type :\t");

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

	if (data->dataType[0] == STRING_TYPE) {
		printf("\tString value    :\t%s\n",data->string_val);
	} else if (data->dataType[0] == INT_TYPE) {
		printf("\tInt value       :\t%d\n",data->int_val);
	} else if (data->dataType[0] == FLOAT_TYPE) {
		printf("\tDoubel value    :\t%lf\n",data->double_val);
	} else {
		printf("\tValue undifiened\n");
	}

}


void printTable(TSymTable *symtab){

	TSymbolItem *tmpitem;

	P("**** SYMBOL TABLE ****\n");

	for (int i = 0; i < MAX_ST_SIZE; i++) {
		
		if (symtab->items[i] != NULL) {

			printf("ON index %d\n\n", i);

			for( tmpitem = symtab->items[i]; tmpitem != NULL; tmpitem = tmpitem->nextItem) {

				if (tmpitem != NULL) {

					printData(&tmpitem->data);

					if (tmpitem->nextItem == NULL){
						P("\tPointer to next :\tNULL\n");
					} else {
						P("\tPointer to next :\tNOT NULL\n");
					}

					P("");

				}
			}
		}
	}
}


/* //TESTING
int main() {

	// pointers declaration
	TSymTable *symtab;
	TData *data;
	P("data initialized");
	

	// init symtab
	if ( (symtab = symTableInit()) == NULL ) {
		return ERR_INTERNAL;
	}
	P("symbol table initialized");



	// ******* insertn new symbol *********
	symTableInsert(symtab, "key1");
	P("symbol inserted");
	data = symTableGetItem(symtab, "key1");
	P("Got Pointer to data");

	// init data
	data->dataType[0] = STRING_TYPE;
	data->defined = true;
	data->idType = variable;
	data->string_val = "symbol1";

	P("data initialized");



	// print table
	printTable(symtab);


	// ********* insertn new symbol *********
	P("trying to insert new item key2");
	symTableInsert(symtab, "key2");
	P("symbol inserted");

	// get pointer to symbol data
	data = symTableGetItem(symtab, "key2");

	// init data
	data->dataType[0] = FLOAT_TYPE;
	data->defined = true;
	data->idType = variable;
	data->double_val = 420.0;


	printTable(symtab);


	// ********* insertn new symbol *********
	P("trying to insert new item foo1");
	if (symTableInsert(symtab, "foo1")) {
		return ERR_INTERNAL;
	}
	P("symbol inserted");

	// get pointer to symbol data
	if ( (data = symTableGetItem(symtab, "foo1")) == NULL) {
		return ERR_INTERNAL;
	}

	// init data
	data->dataType[0] = STRING_TYPE;
	data->defined = true;
	data->idType = function;
	data->string_val = "kuku epta";


	printTable(symtab);



// ********* insertn new symbol *********
	P("Insert symbol with same key, but another data key2");
	symTableInsert(symtab, "key2");
	P("symbol inserted");

	// get pointer to symbol data
	data = symTableGetItem(symtab, "key2");
	P("got data pointer");

	// init data
	data->dataType[0] = INT_TYPE;
	data->defined = true;
	data->idType = variable;
	data->int_val = 421;


	P("");
	P(" INSERT A LOT OF key2");
	P("");


	symTableInsert(symtab, "key2");
	symTableInsert(symtab, "key2");
	symTableInsert(symtab, "key2");
	symTableInsert(symtab, "key2");
	symTableInsert(symtab, "key2");	


	printTable(symtab);

	P("");
	P(" DELETE A LOT OF key2");
	P("");

	symTabDeleteItem(symtab, "key2");
	symTabDeleteItem(symtab, "key2");
	symTabDeleteItem(symtab, "key2");
	symTabDeleteItem(symtab, "key2");

	data = symTableGetItem(symtab, "foo1");


	data->localTable = symTableInit();
	printTable(symtab);




	// free allocated memory
	symTableDestroy(symtab);
	P("table destoyed");

	return 0;
}
*/