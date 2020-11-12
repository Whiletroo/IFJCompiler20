# Author: xkrukh00

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -g
OUTPUT_EXEC = compiler
all = gena.c dynamicStr.c scanner.c stack.c symtable.c

all: all
	$(CC) $(CFLAGS) $(all) -o compiler
