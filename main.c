#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "parser.h"

int main (int argc, char **argv) {

    if (argc == 2) {

        int result;
        char *file = argv[1];
        source = fopen(file, "r");
        setSourceFile(source);
        result = parse();

        if (result == 0) {
            fprintf(stdout,"exit code: %d\n", result);
        } else {
            fprintf(stderr,"exit code: %d\n", result);
        }
        fclose(source);
        return result;

    } else {

        fprintf(stderr, "Error: wrong number of arguments\n");
        return 1;

    }
}