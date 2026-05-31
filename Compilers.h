#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/* Language name */
#define STR_LANGNAME "Panda"

/* Logical constants */
#define PANDA_TRUE 1
#define PANDA_FALSE 0

#define INVALID NULL
#define EOF_CHAR '\0'

/* Panda datatype definitions */
typedef char            panda_char;
typedef char* panda_strg;
typedef int             panda_intg;
typedef float           panda_real;
typedef void            panda_void;

typedef unsigned char   panda_boln;
typedef unsigned char   panda_byte;

typedef long            panda_long;
typedef double          panda_doub;

/*
------------------------------------------------------------
Programs:
1: Coder
2: Reader
3: Scanner
4: Parser
5: Writer
------------------------------------------------------------
*/
enum PROGRAMS {
    PGM_CDR = '1',
    PGM_RDR = '2',
    PGM_SCN = '3',
    PGM_PSR = '4',
    PGM_WRT = '5',
};

/* Function declarations */
panda_intg main1Coder(panda_intg argc, panda_strg* argv);

panda_void printLogo();
panda_void errorPrint(panda_strg fmt, ...);

#endif