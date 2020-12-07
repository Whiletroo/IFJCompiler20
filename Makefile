# Author: xkrukh00

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -pedantic -g
OUTPUT_EXEC = compiler
all = dynamicStr.c scanner.c symtable.c parser.c main.c expressions.c precstack.c gena.c

all: all
	$(CC) $(CFLAGS) $(all) -o compiler
