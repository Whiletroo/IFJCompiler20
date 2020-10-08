# Author: xkrukh00

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -g
OUTPUT_EXEC = compiler
OBJ = scanner.c

target: $(OBJ)
    $(CC) $(CFLAGS) -o $(OUTPUT_EXEC) $(OBJ)
