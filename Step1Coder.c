/*
************************************************************
* File name: Step1Coder.c
* Compiler: MS Visual Studio 2026
* Course: CST8152 – Compilers
* Assignment: A1
* Purpose: Vigenere coder implementation
************************************************************
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif


panda_void vigenereFile(
    const panda_strg inputFileName,
    const panda_strg outputFileName,
    const panda_strg key,
    panda_intg encode)
{
    if (inputFileName == NULL ||
        outputFileName == NULL ||
        key == NULL)
    {
        return;
    }

    FILE* inputFile = fopen(inputFileName, "r");

    if (inputFile == NULL) {
        errorPrint("Error: Cannot open input file.");
        return;
    }

    FILE* outputFile = fopen(outputFileName, "w");

    if (outputFile == NULL) {
        fclose(inputFile);
        errorPrint("Error: Cannot open output file.");
        return;
    }

    panda_intg keyLength = strlen(key);

    if (keyLength <= 0) {
        fclose(inputFile);
        fclose(outputFile);
        return;
    }

    panda_intg ch;
    panda_intg keyIndex = 0;

    while ((ch = fgetc(inputFile)) != EOF) {

        if (ch >= ASCII_START &&
            ch <= ASCII_END)
        {
            panda_intg keyChar = key[keyIndex % keyLength];

            if (encode == CYPHER) {

                ch =
                    ((ch - ASCII_START) +
                        (keyChar - ASCII_START))
                    % ASCII_RANGE
                    + ASCII_START;
            }
            else {

                ch =
                    ((ch - ASCII_START) -
                        (keyChar - ASCII_START)
                        + ASCII_RANGE)
                    % ASCII_RANGE
                    + ASCII_START;
            }

            keyIndex++;
        }

        fputc(ch, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}


panda_strg vigenereMem(
    const panda_strg inputFileName,
    const panda_strg key,
    panda_intg encode)
{
    if (inputFileName == NULL ||
        key == NULL)
    {
        return NULL;
    }

    FILE* inputFile = fopen(inputFileName, "r");

    if (inputFile == NULL) {
        errorPrint("Error: Cannot open input file.");
        return NULL;
    }

    panda_intg size =
        getSizeOfFile(inputFileName);

    if (size <= 0) {
        fclose(inputFile);
        return NULL;
    }

    panda_strg output =
        (panda_strg)malloc(size + 1);

    if (output == NULL) {
        fclose(inputFile);
        return NULL;
    }

    panda_intg keyLength = strlen(key);

    panda_intg ch;
    panda_intg i = 0;
    panda_intg keyIndex = 0;

    while ((ch = fgetc(inputFile)) != EOF) {

        if (ch >= ASCII_START &&
            ch <= ASCII_END)
        {
            panda_intg keyChar =
                key[keyIndex % keyLength];

            if (encode == CYPHER) {

                ch =
                    ((ch - ASCII_START) +
                        (keyChar - ASCII_START))
                    % ASCII_RANGE
                    + ASCII_START;
            }
            else {

                ch =
                    ((ch - ASCII_START) -
                        (keyChar - ASCII_START)
                        + ASCII_RANGE)
                    % ASCII_RANGE
                    + ASCII_START;
            }

            keyIndex++;
        }

        output[i++] = (char)ch;
    }

    output[i] = '\0';

    fclose(inputFile);

    return output;
}

panda_void cypher(
    const panda_strg inputFileName,
    const panda_strg outputFileName,
    const panda_strg key)
{
    vigenereFile(
        inputFileName,
        outputFileName,
        key,
        CYPHER
    );
}


panda_void decipher(
    const panda_strg inputFileName,
    const panda_strg outputFileName,
    const panda_strg key)
{
    vigenereFile(
        inputFileName,
        outputFileName,
        key,
        DECYPHER
    );
}


panda_intg getSizeOfFile(
    const panda_strg filename)
{
    if (filename == NULL)
        return -1;

    FILE* file = fopen(filename, "rb");

    if (file == NULL)
        return -1;

    fseek(file, 0, SEEK_END);

    panda_intg size = ftell(file);

    fclose(file);

    return size;
}