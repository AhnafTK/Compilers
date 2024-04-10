/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

 /* TIP: Do not change pragmas, unless necessary .......................................*/
 /*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
 /*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

 /* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'F', /* Fixed mode (constant size) */
	MODE_ADDIT = 'A', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'M'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

// #define SET_BITWISE_OP		setting the content of the reader
// #define RST_BITWISE_OP		resetting the content of the reader
// #DEFINE CHK_BITWISE OP		checking the content of the reader

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 = (000)_10 */
// Bits implemented.
/* TO_DO: BIT 7: FUL = Full */
#define SET_FUL 0x80	// 1000 0000 (OR)
#define RST_FUL	0x7F	// 0111 1111 (AND)
#define CHK_FUL SET_FUL

/* TO_DO: BIT 6: EMP: Empty */
#define SET_EMP 0x40	// 0100 0000 (OR)
#define RST_EMP	0xBF	// 1011 1111 (AND)
#define CHK_EMP SET_EMP

/* TO_DO: BIT 5: REL = Relocation */
#define SET_REL 0x20	// 0010 0000 (OR)
#define RST_REL	0xDF	// 1101 1111 (AND)
#define CHK_REL SET_REL

/* TO_DO: BIT 4: END = EndOfBuffer */
#define SET_END 0x10	// 0001 0000 (OR)
#define RST_END	0xEF	// 1110 1111 (AND)
#define CHK_END SET_END // AND, the AND destroys everything other than the val you're checking. 
						// if 1:1, result 1, if 0:1: 0. look at the table below.

/*
X1	|	X2	|	&	|	|	|	^	|	~X1		|	~X2		|
0		0		0		0		0		0			0
0		1		0		1		1		1			0
1		0		0		1		1		0			1
1		1		1		1		0		0			0
*/

/*
*		Bit 7 (MSB)		|		6		|		5		|		4		|
	---------------------------------------------------------------------
FUL:		1					0				0				0			// when 1, reader content is not possible to include chars
EMP:		0					1				0				0			// when 1, reader content is empty.
REL:		0					0				1				0			// default 0, when 1, the location of the reader char arr in mem changed bc realloc
END:		0					0				0				1			// default 0, when 1, end of content reached.
*/

#define NCHAR				128			/* Chars from 0 to 127 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	zeus_intg mark;			/* the offset to the mark position (in chars) */
	zeus_intg read;			/* the offset to the get a char position (in chars) */
	zeus_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	zeus_char* content;			/* pointer to the beginning of character array (character buffer) */
	zeus_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	zeus_intg		increment;			/* character array increment factor */
	zeus_intg		mode;				/* operational mode indicator */
	zeus_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	zeus_intg		histogram[NCHAR];	/* Statistics of chars */
	zeus_intg		numReaderErrors;	/* Number of errors from Reader */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate(zeus_intg, zeus_intg, zeus_intg);
ReaderPointer	readerAddChar(ReaderPointer const, zeus_char);
zeus_boln		readerClear(ReaderPointer const);
zeus_boln		readerFree(ReaderPointer const);
zeus_boln		readerIsFull(ReaderPointer const);
zeus_boln		readerIsEmpty(ReaderPointer const);
zeus_boln		readerSetMark(ReaderPointer const, zeus_intg);
zeus_intg		readerPrint(ReaderPointer const);
zeus_intg		readerLoad(ReaderPointer const, FILE* const);
zeus_boln		readerRecover(ReaderPointer const);
zeus_boln		readerRetract(ReaderPointer const);
zeus_boln		readerRestore(ReaderPointer const);
/* Getters */
zeus_char		readerGetChar(ReaderPointer const);
zeus_char* readerGetContent(ReaderPointer const, zeus_intg);
zeus_intg		readerGetPosRead(ReaderPointer const);
zeus_intg		readerGetPosWrte(ReaderPointer const);
zeus_intg		readerGetPosMark(ReaderPointer const);
zeus_intg		readerGetSize(ReaderPointer const);
zeus_intg		readerGetInc(ReaderPointer const);
zeus_intg		readerGetMode(ReaderPointer const);
zeus_byte		readerGetFlags(ReaderPointer const);
zeus_intg		readerShowStat(ReaderPointer const);
zeus_intg		readerNumErrors(ReaderPointer const);

#endif
