#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "parser.h"

int main (int argc, char **argv) {

    if (argc == 2) {
        char *file = argv[1];
        source = fopen(file, "r");
        setSourceFile(source);
        return parse();
    } else {
        return 1;
    }

}