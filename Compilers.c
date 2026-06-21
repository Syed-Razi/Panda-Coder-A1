#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

panda_intg main(panda_intg argc, panda_strg* argv) {

    panda_intg i;

    printLogo();

    if (DEBUG) {
        for (i = 0; i < argc; ++i)
            printf("argv[%d] = %s\n", i, argv[i]);
    }

    if (argc < 2) {
        printf("OPTIONS:\n");
        printf("* [%c] - Coder\n", PGM_CDR);
        printf("* [%c] - Reader\n", PGM_RDR);
        printf("* [%c] - Scanner\n", PGM_SCN);
        printf("* [%c] - Parser\n", PGM_PSR);
        printf("* [%c] - Writer\n", PGM_WRT);
        return EXIT_FAILURE;
    }

    panda_char option = argv[1][0];

    switch (option) {

    case PGM_CDR:
        printf("\n[Option '%c': Starting CODER .....]\n\n", PGM_CDR);
        main1Coder(argc, argv);
        break;
    case PGM_RDR:
        printf("\n[Option '%c': Starting READER .....]\n\n", PGM_RDR);
        main2Reader(argc, argv);
        break;

    default:
        printf("Invalid option.\n");
        break;
    }

    return EXIT_SUCCESS;
}

panda_void printLogo() {

    printf(
        "\t=---------------------------------------=\n"
        "\t|  COMPILERS - ALGONQUIN COLLEGE (S26)  |\n"
        "\t=---------------------------------------=\n"
        "\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n"
        "\t    @@                             @@    \n"
        "\t    @@         P A N D A           @@    \n"
        "\t    @@                             @@    \n"
        "\t    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    \n"
        "\t                                         \n"
        "\t[COMPILER SCRIPT .......................]\n"
        "\t                                         \n"
    );
}

panda_void errorPrint(panda_strg fmt, ...) {

    va_list ap;

    va_start(ap, fmt);

    (panda_void)vfprintf(stderr, fmt, ap);

    va_end(ap);

    if (strchr(fmt, '\n') == NULL)
        fprintf(stderr, "\n");
}