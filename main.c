#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"

int main (int argc, char **argv) {

    if (argc != 2) {
        char *file = argv[1];
        source = fopen(file, "r");
        setSourceFile(source);
        parse();
        return 0;
    } else {
        return 1;
    }
}