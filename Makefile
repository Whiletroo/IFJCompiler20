# Author: xkrukh00

CC = gcc
CFLAGS = -std=c99 -g
OUTPUT_EXEC = compiler
all = dynamicStr.c scanner.c stack.c symtable.c parser.c expressions.c main.c

all: all
	$(CC) $(CFLAGS) $(all) -o compiler
