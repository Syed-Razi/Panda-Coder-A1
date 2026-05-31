#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#define CODER_H_

#define ASCII_START 32
#define ASCII_END 126
#define ASCII_RANGE (ASCII_END - ASCII_START + 1)

#define CYPHER 1
#define DECYPHER 0

/* Function declarations */

panda_void vigenereFile(
    const panda_strg inputFileName,
    const panda_strg outputFileName,
    const panda_strg key,
    panda_intg encode
);

panda_strg vigenereMem(
    const panda_strg inputFileName,
    const panda_strg key,
    panda_intg encode
);

panda_void cypher(
    const panda_strg inputFileName,
    const panda_strg outputFileName,
    const panda_strg key
);

panda_void decipher(
    const panda_strg inputFileName,
    const panda_strg outputFileName,
    const panda_strg key
);

panda_intg getSizeOfFile(const panda_strg filename);

#endif