#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

BufferPointer readerCreate(panda_intg size, panda_real factor) {

	BufferPointer readerPointer = NULL;

	if (size <= 0 || size > READER_MAX_SIZE)
		size = READER_DEFAULT_SIZE;

	if (factor <= 0.0f)
		factor = READER_DEFAULT_FACTOR;

	readerPointer = (BufferPointer)calloc(1, sizeof(Buffer));

	if (readerPointer == NULL)
		return NULL;

	readerPointer->content = (panda_strg)malloc(size);

	if (readerPointer->content == NULL) {
		free(readerPointer);
		return NULL;
	}

	readerPointer->size = size;
	readerPointer->factor = factor;

	readerPointer->position.wrte = 0;
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;

	readerPointer->flags.isEmpty = panda_TRUE;
	readerPointer->flags.isFull = panda_FALSE;
	readerPointer->flags.isRead = panda_FALSE;
	readerPointer->flags.isMoved = panda_FALSE;

	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;

	for (panda_intg i = 0; i < NCHAR; i++)
		readerPointer->histogram[i] = 0;

	return readerPointer;
}

BufferPointer readerAddChar(BufferPointer const readerPointer, panda_char ch) {

	panda_strg tempReader = NULL;
	panda_intg newSize = 0;
	unsigned char value = (unsigned char)ch;

	if (readerPointer == NULL || readerPointer->content == NULL)
		return NULL;

	if (value >= NCHAR) {
		readerPointer->numReaderErrors++;
		return readerPointer;
	}

	if (readerPointer->position.wrte >= readerPointer->size) {

		newSize = readerPointer->size +
			(panda_intg)(readerPointer->size * readerPointer->factor);

		if (newSize <= readerPointer->size)
			newSize = readerPointer->size + 1;

		tempReader = (panda_strg)realloc(readerPointer->content, newSize);

		if (tempReader == NULL) {
			readerPointer->flags.isFull = panda_TRUE;
			readerPointer->numReaderErrors++;
			return NULL;
		}

		if (tempReader != readerPointer->content)
			readerPointer->flags.isMoved = panda_TRUE;

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}

	readerPointer->content[readerPointer->position.wrte++] = ch;
	readerPointer->histogram[value]++;

	readerPointer->flags.isEmpty = panda_FALSE;
	readerPointer->flags.isFull =
		readerPointer->position.wrte >= readerPointer->size;

	return readerPointer;
}

panda_boln readerClear(BufferPointer const readerPointer) {

	if (readerPointer == NULL || readerPointer->content == NULL)
		return panda_FALSE;

	readerPointer->position.wrte = 0;
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;

	readerPointer->flags.isEmpty = panda_TRUE;
	readerPointer->flags.isFull = panda_FALSE;
	readerPointer->flags.isRead = panda_FALSE;
	readerPointer->flags.isMoved = panda_FALSE;

	readerPointer->numReaderErrors = 0;
	readerPointer->checkSum = 0;

	for (panda_intg i = 0; i < NCHAR; i++)
		readerPointer->histogram[i] = 0;

	return panda_TRUE;
}

panda_boln readerFree(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return panda_FALSE;

	if (readerPointer->content != NULL)
		free(readerPointer->content);

	free(readerPointer);

	return panda_TRUE;
}

panda_boln readerIsFull(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return panda_FALSE;

	return readerPointer->flags.isFull;
}

panda_boln readerIsEmpty(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return panda_TRUE;

	return readerPointer->flags.isEmpty;
}

panda_boln readerSetMark(BufferPointer const readerPointer, panda_intg mark) {

	if (readerPointer == NULL)
		return panda_FALSE;

	if (mark < 0 || mark > readerPointer->position.wrte)
		return panda_FALSE;

	readerPointer->position.mark = mark;

	return panda_TRUE;
}

panda_intg readerPrint(BufferPointer const readerPointer) {

	panda_intg count = 0;
	panda_char ch;

	if (readerPointer == NULL || readerPointer->content == NULL)
		return 0;

	while (readerPointer->position.read < readerPointer->position.wrte) {

		ch = readerGetChar(readerPointer);

		if (ch == READER_TERMINATOR)
			break;

		printf("%c", ch);
		count++;
	}

	printf("\n");

	return count;
}

panda_intg readerLoad(BufferPointer const readerPointer, panda_strg fileName) {

	panda_strg input = NULL;
	panda_intg count = 0;

	if (readerPointer == NULL || fileName == NULL)
		return READER_ERROR;

	input = vigenereMem(fileName, STR_LANGNAME, DECYPHER);

	if (input == NULL)
		return READER_ERROR;

	for (panda_intg i = 0; input[i] != '\0'; i++) {

		if (readerAddChar(readerPointer, input[i]) == NULL) {
			free(input);
			return READER_ERROR;
		}

		count++;
	}

	free(input);

	return count;
}

panda_boln readerRecover(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return panda_FALSE;

	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;
	readerPointer->flags.isRead = panda_FALSE;

	return panda_TRUE;
}

panda_boln readerRetract(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return panda_FALSE;

	if (readerPointer->position.read <= 0)
		return panda_FALSE;

	readerPointer->position.read--;
	readerPointer->flags.isRead = panda_FALSE;

	return panda_TRUE;
}

panda_boln readerRestore(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return panda_FALSE;

	readerPointer->position.read = readerPointer->position.mark;
	readerPointer->flags.isRead = panda_FALSE;

	return panda_TRUE;
}

panda_char readerGetChar(BufferPointer const readerPointer) {

	if (readerPointer == NULL || readerPointer->content == NULL)
		return READER_TERMINATOR;

	if (readerPointer->position.read >= readerPointer->position.wrte) {
		readerPointer->flags.isRead = panda_TRUE;
		return READER_TERMINATOR;
	}

	return readerPointer->content[readerPointer->position.read++];
}

panda_strg readerGetContent(BufferPointer const readerPointer, panda_intg pos) {

	if (readerPointer == NULL || readerPointer->content == NULL)
		return NULL;

	if (pos < 0 || pos >= readerPointer->position.wrte)
		return NULL;

	return readerPointer->content + pos;
}

panda_intg readerGetPosRead(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return 0;

	return readerPointer->position.read;
}

panda_intg readerGetPosWrte(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return 0;

	return readerPointer->position.wrte;
}

panda_intg readerGetPosMark(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return 0;

	return readerPointer->position.mark;
}

panda_intg readerGetSize(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return 0;

	return readerPointer->size;
}

panda_void readerPrintFlags(BufferPointer const readerPointer) {

	if (!readerPointer)
		return;

	printf("Flag.isEmpty = %d\n", readerPointer->flags.isEmpty);
	printf("Flag.isFull = %d\n", readerPointer->flags.isFull);
	printf("Flag.isMoved = %d\n", readerPointer->flags.isMoved);
	printf("Flag.isRead = %d\n", readerPointer->flags.isRead);
}

panda_void readerPrintStat(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return;

	for (panda_intg i = 0; i < NCHAR; i++) {

		if (readerPointer->histogram[i] > 0) {

			if (i == '\n')
				printf("B[\\n]=%d, ", readerPointer->histogram[i]);
			else if (i == '\t')
				printf("B[\\t]=%d, ", readerPointer->histogram[i]);
			else if (i == '\r')
				printf("B[\\r]=%d, ", readerPointer->histogram[i]);
			else
				printf("B[%c]=%d, ", i, readerPointer->histogram[i]);
		}
	}

	printf("\n");
}

panda_intg readerNumErrors(BufferPointer const readerPointer) {

	if (readerPointer == NULL)
		return 0;

	return readerPointer->numReaderErrors;
}

panda_intg readerChecksum(BufferPointer readerPointer) {

	panda_intg sum = 0;

	if (readerPointer == NULL || readerPointer->content == NULL)
		return 0;

	for (panda_intg i = 0; i < readerPointer->position.wrte; i++)
		sum += (unsigned char)readerPointer->content[i];

	readerPointer->checkSum = sum % 255;

	return readerPointer->checkSum;
}