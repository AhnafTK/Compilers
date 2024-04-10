/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************

/*
************************************************************
* File name: scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A22, A32.
* Date: Sep 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/


#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 32  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define CHAR_LEN_MAX 3 /* max number of char, something like 'a'*/

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	STR_T,		/*  2: String literal token */
	CHAR_T,		/*  3: Char token*/
	INT_T,		/*  4: Integer token*/
	REAL_T,		/*   : Real token*/
	VARID_T,
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	COMMA_T,	/* , */
	COLON_T,
	CONCAT_T,
	LBR_T,		/*  5: Left brace token */
	RBR_T,		/*  6: Right brace token */
	KW_T,		/*  7: Keyword token */
	EOS_T,		/*  8: End of statement (semicolon) */
	RTE_T,		/*  9: Run-time error token */
	INL_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	ARITH_OP_T,
	INCR_T,
	REL_OP_T,
	LOG_OP_T,
	EQ_T,		/* = */
	LT_T,		/* < */
	LTE_T,		/* <= */
	GT_T,		/* > */
	GTE_T,		/* >= */
	EQLITY_T,	/* == */
	NOT_T,		/* ! */
	NEQU_T,		/* != */
	AND_T,		/* && */
	OR_T,		/* || */
	TAB_T,		/* Tab */
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_INC } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_GTE, OP_LT, OP_LTE } RelOperator;
typedef enum VariableDatatype {VAR_INT, VAR_REAL, VAR_CHAR, VAR_STRING} varDatatype;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator; 

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	zeus_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	zeus_intg intValue;						/* integer literal attribute (value) */
	zeus_intg keywordIndex;					/* keyword index in the keyword table */
	zeus_intg contentString;				/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	zeus_real floatValue;					/* floating-point literal attribute (value) */
	zeus_char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	zeus_char errLexeme[ERR_LEN + 1];		/* error token attribite */
	//zeus_char varLexeme[VID_LEN + 1];
	zeus_char charLexeme;

} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	zeus_byte flags;			/* Flags information */
	union {
		zeus_intg intValue;				/* Integer value */
		zeus_real floatValue;			/* Float value */
		zeus_char* stringContent;		/* String value */
		//zeus_char* charContent;		    /*char content maybe?*/
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	zeus_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFF

/*  Special case tokens processed separately one by one in the token-driven part of the scanner (JULIUS)
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';', 
 *  white space, #comment\n , ',' , ';' , '-' , '+' , '*' , '/', # , 
 *  .&., .|. , .!. , SEOF.
 */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '_'
#define CHRCOL4 '\'' // this is single quote ' 
#define CHRCOL7 '.' // period

// added by ahnaf
#define CHRCOL5 '"'


/* TO_DO: Error states and illegal state */
#define FS		100		/* Illegal state */
#define ESNR	19		/* Error state with no retract */ // 6 by paulo, 19 by us
#define ESWR	18		/* Error state with retract */

 /* TO_DO: State transition table definition */
//#define TABLE_COLUMNS 7 // default by paulo
#define TABLE_COLUMNS 10 // ours

/* TO_DO: Transition table - type of states defined in separate table */


static zeus_intg transitionTable[][TABLE_COLUMNS] = {
	/*[A-z], [0-9],    _,   TAG,   EQ , SQ('), DQ("), PERIOD,  SEOF,  other
	   L(0),  D(1), U(2), # (3),  =(4),   (5),   (6),    (7),  E(8),   O(9) */
	{	  4,	14, ESNR,     6,  ESNR,     1,    12,   ESNR,  ESWR,  ESNR}, // NOAS[0]	
	{     2,  ESNR, ESNR,  ESNR,  ESNR,  ESNR,  ESNR,   ESNR,  ESWR,  ESNR}, // NOAS[1]
	{  ESNR,  ESNR, ESNR,  ESNR,  ESNR,     3,  ESNR,   ESNR,  ESWR,  ESNR}, // NOAS[2]
	{    FS,    FS,   FS,    FS,    FS,    FS,    FS,     FS,    FS,    FS}, // ASNR[CHARID][3]
	{     4,     4,    4,  ESNR,  ESNR,  ESNR,  ESNR,   ESNR,     5,     5}, // NOAS[4]
	{    FS,    FS,   FS,    FS,    FS,    FS,    FS,     FS,    FS,    FS}, // ASNR[VARID][5]
	{	  7,     7,    7,     7,     9,     7,     7,      7,  ESWR,     7}, // NOAS[6]
	{	  7,     7,    7,     7,     7,     7,     7,      7,  ESWR,     8}, // NOAS[7]
	{    FS,    FS,   FS,    FS,    FS,    FS,    FS,     FS,    FS,    FS}, // ASNR[CMMNT][8]
	{     9,     9,    9,     9,    10,     9,     9,      9,  ESWR,     9}, // NOAS[9]
	{	  9,	 9,	   9,	 FS,	 9,	    9,	   9,	   9,  ESWR,     9}, // NOAS[10]
	{    FS,    FS,   FS,    FS,    FS,    FS,    FS,     FS,    FS,    FS}, // ASNR[MLCMMNT][11]
	{    12,	12,	  12,	 12,	12,	   12,	  13,	  12,  ESWR,    12}, // NOAS[12]
	{    FS,    FS,   FS,    FS,    FS,    FS,    FS,     FS,    FS,    FS}, // ASNR[SL][13]
	{  ESNR,	14,	ESNR,  ESNR,  ESNR,	 ESNR,	ESNR,	  16,    15,    15}, // NOAS[14]
	{    FS,    FS,   FS,    FS,    FS,    FS,    FS,     FS,    FS,    FS}, // ASNR[INTID][15]
	{  ESNR,	16,	ESNR,  ESNR,  ESNR,	 ESNR,	ESNR,	ESNR,    ESNR,  17}, // NOAS[16]
	{    FS,    FS,   FS,    FS,    FS,    FS,    FS,     FS,    FS,    FS}, // ASNR[REID][17]
};


/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static zeus_intg stateType[] = {
	// corresponds with the outputs in the TT
	NOFS, // [0]
	NOFS, // [1]
	NOFS, // [2]
	FSNR, // [3]  CHARID
	NOFS, // [4]
	FSWR, // [5]  VARID
	NOFS, // [6]
	NOFS, // [7]
	FSNR, // [8]  CMMNT
	NOFS, // [9]
	NOFS, // [10]
	FSNR, // [11] MLCMMNT
	NOFS, // [12]
	FSNR, // [13] SL
	NOFS, // [14]
	FSWR, // [15] INTID
	NOFS, // [16]
	FSWR, // [17] REID
	
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
zeus_intg startScanner(ReaderPointer psc_buf);
Token tokenizer(zeus_void);
static zeus_intg nextClass(zeus_char c);				/* character class function */
static zeus_intg nextState(zeus_intg, zeus_char);		/* state machine function */
zeus_void printToken(Token t);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(zeus_char* lexeme);

/* Declare accepting states functions */
Token funcSL	(zeus_char lexeme[]); // string literal
Token funcID	(zeus_char lexeme[]); // function ID
Token funcKEY	(zeus_char lexeme[]); // keyword
Token funcErr	(zeus_char lexeme[]); // error
Token funcIL	(zeus_char lexeme[]); // integer literal
Token funcRL	(zeus_char lexeme[]); // real literal
Token funcCHAR  (zeus_char lexeme[]);
/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[] = {

	//funcErr,	/* ERR1 [06] - No retract */
	//funcErr		/* ERR2 [07] - Retract */
	NULL, // [00]
	NULL, // [01]
	NULL, // [02] 
	funcCHAR, // [03] charid here
	NULL,	// [04]
	funcKEY, // [05] VARID / keyword
	NULL, // [06]
	NULL, // [07] 
	NULL, // [08] // CMMNT
	NULL, // [09]
	NULL, // [10] 
	NULL, // [11] MLCMMNT
	NULL, // [12]
	funcSL, // [13] SL
	NULL, // [14]
	funcIL, // [15] INTID
	NULL,// [16]
	funcRL, // [17] REID
	funcErr, // [18]
	funcErr //[19] -- retract
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 22

/* TO_DO: Define the list of keywords */
static zeus_char* keywordTable[KWT_SIZE] = {

	"int", // 0
	"real",// 1
	"string",// 2
	"char",
	"if",// 3
	"in",
	"elseif",// 4
	"else",// 5
	"do",// 7
	"print",// 9
	"readline",// 10
	"function",// 11
	"end",// 12
	"return",// 13
	"for",// 14
	"while",// 15
	"void",// 16
	"const",// 17
	"cube", // 18
	"square", // 19
	"true", // 20
	"false" // 21
};

/* About indentation (useful for positional languages (ex: Python, Cobol) */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	zeus_char indentationCharType;
	zeus_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
zeus_intg numScannerErrors;

#endif
