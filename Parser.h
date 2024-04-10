/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@                                                               @@@@
@@          /       ==============================        /          @@
@         ////           @@       @@ #@                  ////         @
@         ////*          @@ @%  @ @@    @@  @ #@#       /////         @
@     ((( ////))))   @@  @% @%  @ @@ #@ @@  @ #@@#   ///////( ///     @
@     ((((,/ )))))    @@@@   @@@  @@ ##  @@@   #@#   ///// ,/////     @
@     (((((/)))((    ------------------------------    ((((./////*    @
@    //*./ /  .///   ---  PROGRAMMING LANGUAGE  ---  ////   / ( (//   @
@    ///// / /////   ==============================  * ////* / ////   @
@     ///// ///// ((                               (( ///// /////     @
@    ((((   / , (((((                             (((((  //   /(((    @
@    (((((((/ . (((((                          (((((* / (((((((       @
@      (((((( //((((/((((                    *((((/((((/( (((((/      @
@       .//,   * /   (((((                   (((((  ///    .//.       @
@     ,////////./(  (((((* ////         (///(((((( ,/(,////////       @
@         //////// ,// ((( /////,     ////// ((( //  /////// *        @
@            (((((((((,// * /////     /////   (//(((((((((            @
@            ((((((((((((/////         //.///  (((((((((((.           @
@                   (///////// //(   (//.//////////                   @
@                  (/////////             //////////                  @
@                                                                     @
@@          A L G O N Q U I N   C O L L E G E  -  2 0 2 3 W          @@
@@@@                                                               @@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

/*
************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
*************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token lookahead;
extern BufferReader* stringLiteralTable;
extern zeus_intg line;
extern Token tokenizer();
extern zeus_char* keywordTable[];
zeus_intg syntaxErrorNumber = 0;

#define STR_LANGNAME	"zeus"
#define LANG_WRTE		"print"
#define LANG_READ		"readline"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,

	KW_int, // 0
	KW_real,// 1
	KW_string,// 2
	KW_char,
	KW_if,// 3
	KW_in,
	KW_elseif,// 4
	KW_else,// 5
	KW_do,// 7
	KW_print,// 9
	KW_readline,// 10
	KW_function,// 11
	KW_end,// 12
	KW_return,// 13
	KW_for,// 14
	KW_while,// 15
	KW_void,// 16
	KW_const,// 17
	KW_PI,// 18
	KW_true, // 21
	KW_false // 22
};

/* Function definitions */
zeus_void startParser();
zeus_void matchToken(zeus_intg, zeus_intg);
zeus_void syncErrorHandler(zeus_intg);
zeus_void printError();

/* TO_DO: Place ALL non-terminal function declarations */
zeus_void codeSession();
zeus_void variableIdentifier();

zeus_void optionalStatements();

zeus_void inputStatement();
zeus_void outputStatement();

zeus_void program();

zeus_void statement();
zeus_void statements();
zeus_void statementsPrime();

// if statement
zeus_void selectionStatement();
zeus_void iterationWhileStatement();
zeus_void iterationForStatement();

// assignment
zeus_void assignmentStatement();
zeus_void assignmentExpression();

// string
zeus_void stringExpression();
zeus_void primaryStringExpression();

// logical
zeus_void logical_AND_Expression();
zeus_void logical_AND_ExpressionPrime();

zeus_void logical_OR_Expression();
zeus_void logical_OR_ExpressionPrime();

zeus_void logical_NOT_Expression();


zeus_void conditionalExpression();
zeus_void relationalExpression();
zeus_void relational_a_Expression();
zeus_void relational_s_Expression();

zeus_void charExpression();

zeus_void stringExpressionPrime();
zeus_void primary_a_relationalExpression();
zeus_void primary_s_relationalExpression();

zeus_void item();
zeus_void iterator();

zeus_void arithmeticExpression();
zeus_void unaryArithmeticExpression();
zeus_void primaryArithmeticExpression();
zeus_void additiveArithmeticExpression();
zeus_void multiplicativeArithmeticExpression();
zeus_void outputStatementPrime();

zeus_void variableDeclarationExpression();

zeus_void intVarDeclaration();
zeus_void stringVarDeclaration();
zeus_void realVarDeclaration();
zeus_void charVarDeclaration();

zeus_void variableIdentifier();
zeus_void additiveArithmeticExpressionPrime();
zeus_void multiplicativeArithmeticExpressionPrime();
zeus_void cubeStatement();
zeus_void squareStatement();
#endif
