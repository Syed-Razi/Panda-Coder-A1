#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

panda_intg main1Coder(panda_intg argc, panda_strg* argv) {

    if (argc < 5) {
        printf("Usage: %s 1 [cypher=1|decypher=0] <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    panda_strg operation = argv[2];
    panda_strg inputFileName = argv[3];
    panda_strg outputFileName = argv[4];
    panda_strg key = STR_LANGNAME;

    if (atoi(operation) == CYPHER) {
        cypher(inputFileName, outputFileName, key);
    }
    else if (atoi(operation) == DECYPHER) {
        decipher(inputFileName, outputFileName, key);
    }
    else {
        errorPrint("Error: Invalid operation.");
        return EXIT_FAILURE;
    }

    printf("Operation '%s' completed successfully.\n", operation);

    panda_strg output = NULL;

    output = vigenereMem(outputFileName, key, atoi(operation));

    if (output != NULL) {
        printf("OUTPUT:\n%s\n", output);
        free(output);
    }

    return EXIT_SUCCESS;
}