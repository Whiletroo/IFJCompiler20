/**
 * @file symtable.c
 * @author xkrukh00
 * @date 21.10.2020
 * @brief Implementation of symbol teable.
*/

#include "symtable.h"

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



int symTableInsert(TSymTable *symtab, char *key) {
	
	if (symtab == NULL) {
		return ERR_INTERNAL;
	}

	int index = hashf(key);

	if ( symtab->items[index] == NULL) { // if entry is epty

		// allocate memory
		symtab->items[index] = malloc(sizeof(TSymbolItem));
		CHECK_PTR(symtab->items[index]);

		// init key value
		symtab->items[index]->key = malloc(sizeof(char));
		CHECK_PTR(symtab->items[index]->key);
		memcpy(symtab->items[index]->key, key, strlen(key));

		// seting data to defaut
		symtab->items[index]->data.identifier = malloc(sizeof(char));
		CHECK_PTR(symtab->items[index]->data.identifier);
		memcpy(symtab->items[index]->data.identifier, key, strlen(key));
		symtab->items[index]->data.dataType = NIL_TYPE;
		symtab->items[index]->data.idType = UNDEF;
		symtab->items[index]->data.defined = false;
		symtab->items[index]->data.string_val = NULL;

		symtab->items[index]->nextItem = NULL;

		return OK;

	} else {							// if entry is not empty
		
		// allocate memory
		TSymbolItem *newitem = malloc(sizeof(TSymbolItem));
		CHECK_PTR(newitem);

		// init key value
		newitem->key = malloc(sizeof(char));
		CHECK_PTR(newitem->key);
		memcpy(newitem->key, key, strlen(key));

		// setting default values
		newitem->data.identifier = malloc(sizeof(char));
		CHECK_PTR(newitem->data.identifier);
		memcpy(newitem->data.identifier, key, strlen(key));
		newitem->data.dataType = NIL_TYPE;
		newitem->data.idType = UNDEF;
		newitem->data.defined = false;
		newitem->data.string_val = NULL;

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

	if (data->dataType == STRING_TYPE) {
		printf("\tString value    :\t%s\n",data->string_val);
	} else if (data->dataType == INT_TYPE) {
		printf("\tInt value       :\t%d\n",data->int_val);
	} else if (data->dataType == FLOAT_TYPE) {
		printf("\tDoubel value    :\t%lf\n",data->double_val);
	} else {
		printf("\tValue undifiened\n");
	}

}


void printTable(TSymTable *symtab){

	TSymbolItem *tmpitem;

	P("**** SYMBOL TABLE ****\n");

	for (int i = 0; i < MAX_ST_SIZE; i++) {

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


/*
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
	data->dataType = STRING_TYPE;
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
	data->dataType = FLOAT_TYPE;
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
	data->dataType = STRING_TYPE;
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
	data->dataType = INT_TYPE;
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



	printTable(symtab);

	// free allocated memory
	symTableDestroy(symtab);
	P("table destoyed");

	return 0;
}
*/