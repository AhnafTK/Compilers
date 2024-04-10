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
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Sep 01 2022
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern ReaderPointer stringLiteralTable;	/* String literal table */
zeus_intg line;								/* Current line number of the source code */
extern zeus_intg errorNumber;				/* Defined in platy_st.c - run-time error number */

extern zeus_intg stateType[];
extern zeus_char* keywordTable[];
extern PTR_ACCFUN finalStateTable[];
extern zeus_intg transitionTable[][TABLE_COLUMNS];

/* Local(file) global objects - variables */
static ReaderPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static ReaderPointer sourceBuffer;			/* Pointer to input source buffer */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

zeus_intg startScanner(ReaderPointer psc_buf) {
	/* in case the buffer has been read previously  */
	readerRecover(psc_buf);
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(zeus_void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	zeus_char c;				/* input symbol */
	zeus_intg state = 0;		/* initial state of the FSM */
	zeus_intg lexStart;			/* start offset of a lexeme in the input char buffer (array) */
	zeus_intg lexEnd;			/* end offset of a lexeme in the input char buffer (array)*/

	zeus_intg lexLength;		/* token length */
	zeus_intg i;				/* counter */
	zeus_char newc;				/* new char */
	
	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = readerGetChar(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		/* TO_DO: All patterns that do not require accepting functions */
		switch (c) {
			/* Cases for spaces */
		case ' ':
			break;
		case '\t':
			//currentToken.code = TAB_T;
			//return currentToken;
		case '\f':
			break;
		case '\n':
			line++;
			break;
		case ':':
			currentToken.code = COLON_T;
			return currentToken;
		case ',':
			currentToken.code = COMMA_T;
			return currentToken;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
			/* Arithmetic */
		case '+':
			currentToken.code = ARITH_OP_T;
			currentToken.attribute.arithmeticOperator = OP_ADD;
			return currentToken;
		case '-':
			currentToken.code = ARITH_OP_T;
			currentToken.attribute.arithmeticOperator = OP_SUB;
			return currentToken;
		case '*':
			newc = readerGetChar(sourceBuffer);
			if (newc == '*') {
				currentToken.code = CONCAT_T;
				return currentToken;
			}
			currentToken.code = ARITH_OP_T;
			currentToken.attribute.arithmeticOperator = OP_MUL;
			return currentToken;
		case '/':
			currentToken.code = ARITH_OP_T;
			currentToken.attribute.arithmeticOperator = OP_DIV;
			return currentToken;
			/* End of Arithmetic */
			/* Relational */
		case '=':
			newc = readerGetChar(sourceBuffer); // the next token after c
			if (newc == '=') {
				currentToken.code = EQLITY_T;
				return currentToken;
			}
			readerRetract(sourceBuffer);
			currentToken.code = EQ_T;
			return currentToken;
		case '<':
			newc = readerGetChar(sourceBuffer); // the next token after c
			if (newc == '=') {
				currentToken.attribute.relationalOperator = OP_LTE;
				currentToken.code = LTE_T;
				return currentToken;
			}
			readerRetract(sourceBuffer);
			currentToken.code = LT_T;
			currentToken.attribute.relationalOperator = OP_LT;
			return currentToken;
		case '>':
			newc = readerGetChar(sourceBuffer); // the next token after c
			if (newc == '=') {
				currentToken.attribute.relationalOperator = OP_GTE;
				currentToken.code = GTE_T;
				return currentToken;
			}
			readerRetract(sourceBuffer);
			currentToken.code = GT_T;
			currentToken.attribute.relationalOperator = OP_GT;
			return currentToken;
		/* End of relational */
		/* Logical */
		case '!':
			newc = readerGetChar(sourceBuffer); // the next token after c
			if (newc == '=') {
				currentToken.code = NEQU_T;
				currentToken.attribute.logicalOperator = OP_NE;
				return currentToken;
			}
			readerRetract(sourceBuffer);
			currentToken.code = NOT_T;
			currentToken.attribute.logicalOperator = OP_NOT;
			return currentToken;
		case '&':
			newc = readerGetChar(sourceBuffer); // the next token after c
			if (newc == '&') {
				currentToken.code = AND_T;
				currentToken.attribute.logicalOperator = OP_AND;
				return currentToken;
			}
			readerRetract(sourceBuffer);
			currentToken.code = ERR_T;
			return currentToken;
		case '|':
			newc = readerGetChar(sourceBuffer); // the next token after c
			if (newc == '|') {
				currentToken.code = OR_T;
				currentToken.attribute.logicalOperator = OP_OR;
				return currentToken;
			}
			readerRetract(sourceBuffer);
			currentToken.code = ERR_T;
			return currentToken;
		/* End of Logical */
		/* Comments */
		case '#':
			newc = readerGetChar(sourceBuffer); // the next token after c
			if (newc == '=') { // if that token is =, then current state is #=, beginning of ML comment
				do { // keep looping until end of file
					c = readerGetChar(sourceBuffer); // get the char 
					if (c == CHARSEOF0 || c == CHARSEOF255) { // if end of file, retract
						readerRetract(sourceBuffer); // retract
						//return currentToken;
					}
					if (c == '=') { // if another equal sign is hit
						newc = readerGetChar(sourceBuffer); // get the next char after that
						if (newc == '#') { break; } // if its an #, the ML comment is being closed.
					}
				} while (c != CHARSEOF0 && c != CHARSEOF255);
			}
			else {
				do {
					c = readerGetChar(sourceBuffer); // 
					if (c == CHARSEOF0 || c == CHARSEOF255) {
						readerRetract(sourceBuffer);
						//return currentToken;
					}
					else if (c == '\n') {
						line++;
					}
				} while (c != '\n' && c != CHARSEOF0 && c != CHARSEOF255);
				//} while (c != CHARSEOF0 && c != CHARSEOF255);
			}
			break;
		/* Cases for END OF FILE */
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_0;
			return currentToken;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.seofType = SEOF_255;
			return currentToken;
	
		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
			Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

		/* TO_DO: Adjust / check the logic for your language */
		default: // general case
			state = nextState(state, c);
			lexStart = readerGetPosRead(sourceBuffer) - 1;
			readerSetMark(sourceBuffer, lexStart);
			int pos = 0;
			while (stateType[state] == NOFS) {
				c = readerGetChar(sourceBuffer);
				state = nextState(state, c);
				pos++;
			}
			if (stateType[state] == FSWR)
				readerRetract(sourceBuffer);
			lexEnd = readerGetPosRead(sourceBuffer);
			lexLength = lexEnd - lexStart;
			lexemeBuffer = readerCreate((zeus_intg)lexLength + 2, 0, MODE_FIXED);
			if (!lexemeBuffer) {
				fprintf(stderr, "Scanner error: Can not create buffer\n");
				exit(1);
			}
			readerRestore(sourceBuffer);
			for (i = 0; i < lexLength; i++)
				readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
			
			readerAddChar(lexemeBuffer, READER_TERMINATOR);
			currentToken = (*finalStateTable[state])(readerGetContent(lexemeBuffer, 0));
			readerRestore(lexemeBuffer); //xxx
			return currentToken;
		} // switch

	} //while

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

zeus_intg nextState(zeus_intg state, zeus_char c) {
	zeus_intg col;
	zeus_intg next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*	[A-z](0), [0-9](1),	_(2), &(3), "(4), SEOF(5), other(6) */

// our logic
/*[A-z], [0-9],    _,   TAG,   EQ ,    SQ('),    DQ("),    PERIOD,  SEOF,  other
   L(0),  D(1), U(2), # (3),  =(4), SQ(')(5), DQ(")(6),    (.)(7),  E(8),   O(9) */
zeus_intg nextClass(zeus_char c) {
	zeus_intg val = -1;
	switch (c) {
	case CHRCOL2: // underscore
		val = 2;
		break;
	case CHRCOL4: // for single quotes
		val = 5; 
		break;
	case CHRCOL5: // for double quotes
		val = 6;
		break;
	case CHRCOL7: // period
		val = 7;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 8;
		break;
	default:
		if (isalpha(c)) // for letters
			val = 0;
		else if (isdigit(c)) // for digits
			val = 1;
		else
			val = 9; // for others
	}
	return val;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */
  /* TO_DO: Adjust the function for IL */
// do error checking
Token funcIL(zeus_char lexeme[]) {
	Token currentToken = { 0 };
	zeus_long tlong;
	if (lexeme[0] != '\0' && strlen(lexeme) > NUM_LEN) { 
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INL_T;
			currentToken.attribute.intValue = (zeus_intg)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	currentToken.code = INT_T; // since valid, set the code of the token
	return currentToken;
}

Token funcRL(zeus_char lexeme[]) {
	Token currentToken = { 0 };
	zeus_real tlong;
	// to do: error checking
	tlong = (zeus_real)atof(lexeme); // turn the lexeme str into double using atof, cast to float and store in tlong
	currentToken.code = REAL_T; // set the code as real
	currentToken.attribute.floatValue = tlong; // set the attribute as float
	return currentToken; // return it 
	
}

/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(zeus_char lexeme[]) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);
	zeus_char lastch = lexeme[length - 1];
	zeus_intg isID = ZEUS_FALSE;
	switch (lastch) {
		// Test Keyword
		currentToken = funcKEY(lexeme);
		break;
	}
	if (isID == ZEUS_TRUE) {
		strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
		currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
	}
	return currentToken;
}

Token funcCHAR(zeus_char lexeme[]) {
	Token currentToken = { 0 };
	zeus_intg i = 0, len = (zeus_intg)strlen(lexeme);
	//currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);

	// if length is greater than 3 then invalid because its not 'x' anymore.
	if (len > CHAR_LEN_MAX) {
		currentToken.code = RTE_CODE; // set it as error
		return currentToken; // return it 
	}

	currentToken.code = CHAR_T; // set the code to  char
	currentToken.attribute.charLexeme = lexeme[1];
	//strcpy(currentToken.attribute.CharLexeme, lexeme); // copy the lexeme content into CharLexeme attribute
	return currentToken;

}
/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */
/* TO_DO: Adjust the function for SL */
Token funcSL(zeus_char lexeme[]) {
	Token currentToken = { 0 };
	zeus_intg i = 0, len = (zeus_intg)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == '\n')
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, CHARSEOF0)) {
		currentToken.code = RTE_T;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	return currentToken;
}

/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(zeus_char lexeme[]) {
	Token currentToken = { 0 };
	zeus_intg kwindex = -1, j = 0;
	for (j = 0; j < KWT_SIZE; j++)
		if (!strcmp(lexeme, &keywordTable[j][0])) {
			kwindex = j;
		}
	// if its found
	if (kwindex != -1) {
		currentToken.code = KW_T;
		currentToken.attribute.codeType = kwindex;
	}
	else {
		//currentToken = funcErr(lexeme);
		int len = (zeus_intg)strlen(lexeme);
		// if its not in the keyword table, and the length is valid, then it's a variable name
		if (len < VID_LEN) {
			strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
			currentToken.attribute.idLexeme[VID_LEN] = CHARSEOF0;
			currentToken.code = VARID_T;
		}
		else {
			currentToken = funcErr(lexeme); // if length is too big
		}
		
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(zeus_char lexeme[]) {
	Token currentToken = { 0 };
	zeus_intg i = 0, len = (zeus_intg)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = CHARSEOF0;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == '\n')
			line++;
	currentToken.code = ERR_T;
	return currentToken;
}

/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

zeus_void printToken(Token t) {

	/* External variables */
	extern zeus_char* keywordTable[]; /* link to keyword table in */
	extern numScannerErrors;			/* link to number of errors (defined in Scanner.h) */

	switch (t.code) {
	case RTE_T:
		//numScannerErrors++;
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case TAB_T:
		printf("TAB_T\t\n");
		break;
	case ERR_T:
		numScannerErrors++;
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		/* TO_DO: Update numScannerErrors */
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case INT_T: // for ints
		printf("INT_T\t\t%d\n", t.attribute.intValue);
		break;
	case REAL_T: // for reals
		printf("REAL_T\t\t%0.4f\n", t.attribute.floatValue); // idk how many decimal places 
		break;
	case CHAR_T: // for chars
		printf("CHAR_T\t\t%c\n", t.attribute.charLexeme); // idk what to put here
		break;
	case VARID_T: // variables
		printf("VARID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (zeus_intg)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (zeus_intg)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T:\t\t(\n");
		break;
	case RPR_T:
		printf("RPR_T\t\t)\n");
		break;
	case LBR_T: // (
		printf("LBR_T\n");
		break;
	case RBR_T: // )
		printf("RBR_T\n");
		break;
	case CONCAT_T:
		printf("CONCAT\t\t**\n");
		break;
	case EQLITY_T: // ==
		printf("EQULITY_T\t==\n");
		break;
	case ARITH_OP_T:
		printf("ARITH_OP_T\n");
		break;
	case EQ_T: // equals
		printf("EQ_T\t\t=\n");
		break;
	case COMMA_T:
		printf("COMMA_T\t\t,\n");
		break;
	case COLON_T:
		printf("COLON_T\t\t:\n");
		break;
	case LT_T:
		printf("LT_T\t\t<\n");
		break;
	case LTE_T:
		printf("LTE_T\t\t<=\n");
		break;
	case GT_T:
		printf("GT_T\t\t>\n");
		break;
	case GTE_T:
		printf("GTE_T\t\t>=\n");
		break;
	case NOT_T:
		printf("NOT_T\t\t!\n");
		break;
	case NEQU_T:
		printf("NEQU_T\t\t!=\n");
		break;
	case AND_T:
		printf("AND_T\t\t&&\n");
		break;
	case OR_T:
		printf("OR_T\t\t||\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	default:
		//numScannerErrors++;
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/


