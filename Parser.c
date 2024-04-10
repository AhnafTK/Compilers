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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
 /* TO_DO: This is the function to start the parser - check your program definition */

zeus_void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
 /* TO_DO: This is the main code for match - check your definition */
zeus_void matchToken(zeus_intg tokenCode, zeus_intg tokenAttribute) {
	zeus_intg matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
 /* TO_DO: This is the function to handler error - adjust basically datatypes */
zeus_void syncErrorHandler(zeus_intg syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
 /* TO_DO: This is the function to error printing - adjust basically datatypes */
zeus_void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
		//case MNID_T:
			//printf("MNID_T:\t\t%s\t\n", t.attribute.idLexeme);
			//break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> } ours: -> <statements>
 * FIRST(<program>)= {MNID_T (main&)}.
 ***********************************************************
 */
zeus_void program() { // problem is that this is running only once, need some sort of a wider function, like codeSession + dataSession combined
	codeSession();
	switch (lookahead.code) {
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}


/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KW_T (KW_code)}.
 ***********************************************************
 */
zeus_void codeSession() {
	//	matchToken(KW_T, KW_code);
	//	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	//	matchToken(RBR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *				KW_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
zeus_void optionalStatements() {
	statements();

	switch (lookahead.code) {
		/*
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
			statements();
			break;
		}
		*/
	default:
		; // Empty
	}
	printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if),
 *		KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
zeus_void statements() {
	statement();
	statementsPrime();
	printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T,
 *		KW_T(KW_if), KW_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
zeus_void statementsPrime() {

	switch (lookahead.code) {
	case KW_T:
		//printf("AAAAAAAAAAAAAAA: %d", lookahead.attribute.codeType);
		switch (lookahead.attribute.codeType) {
		case KW_readline:
			statement();
			statementsPrime();
			break;
		case KW_print:
			statement();
			statementsPrime();
			break;
		case KW_while:
			statement();
			statementsPrime();
			break;
		case KW_if:
			statement();
			statementsPrime();
			break;
		case KW_for:
			statement();
			statementsPrime();
			break;
		case KW_int:
		case KW_real:
		case KW_char:
		case KW_string:
			statement();
			statementsPrime();
			break;
		}
		break;
	case VARID_T:
		statement();
		statementsPrime();
		break;
	case STR_T:
		statement();
		statementsPrime();
		break;
	case CHAR_T:
		statement();
		statementsPrime();
		break;
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KW_T(KW_if), KW_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 * Single statement
 * <statement> -> <inputStatement> | <outputStatement> | <selectmentStatement> |
 *				  <iterationStatement> | <iterationForStatement> | <variableDeclarationStatement> |
 *				  <cubeStatement> | <squareStatement>
 */
zeus_void statement() {
	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.codeType) {
		case KW_readline:
			inputStatement();
			break;
		case KW_print:
			outputStatement();
			break;
		case KW_if:
			selectionStatement();
			break;
		case KW_while:
			iterationWhileStatement();
			break;
		case KW_for:
			iterationForStatement();
			break;
		case KW_int:
		case KW_real:
		case KW_char:
		case KW_string:
			variableDeclarationExpression();
			break;
		}
		break;

		/*
	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
			outputStatement();
		}
		break;
		*/
	case VARID_T:
		assignmentStatement();
		break;
	default:
		printError();
		//break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <assignment_statement> -> <assignment_expression>
 ***********************************************************
 */

/*
* <assignmentStatement> -> <assignmentExpression> 
*/
zeus_void assignmentStatement() {
	assignmentExpression();
	printf("%s%s\n", STR_LANGNAME, ": Assignment Statement parsed");
}
/*
*
* BNF: <assignment expression> -> <variable identifier> = <arithmetic expression> | 
														  <string expression> | 
														  <char expression> | 
														  <string expression prime> | 
														  <additive arithmetic expression> 
*/
zeus_void assignmentExpression() {

	// something
	matchToken(VARID_T, NO_ATTR);
	matchToken(EQ_T, NO_ATTR); // followed by = 

	switch (lookahead.code) {

	case VARID_T:
		matchToken(VARID_T, NO_ATTR);
		if (lookahead.code == CONCAT_T) {
			stringExpressionPrime();
		}
		else {
			additiveArithmeticExpression();
		}
		break;
	case INT_T:
		arithmeticExpression();
		break;
	case REAL_T:
		arithmeticExpression();
		break;
	case STR_T:
		stringExpression();
		break;
	case CHAR_T:
		charExpression();
		break;
	case ARITH_OP_T:
		arithmeticExpression();
		break;
	}
}

/*
* <input expression> -> <input statement>
*/
zeus_void inputExpression() {
	inputStatement();
}

/*
* <selectionStatement> -> if(<conditionalExpression>)
*							<optionalStatements>
*						  else
*							<optionalStatements>
*						  end
*/
zeus_void selectionStatement() {
	matchToken(KW_T, KW_if);		// if 
	matchToken(LPR_T, NO_ATTR);     // (
	conditionalExpression();		// condition...
	matchToken(RPR_T, NO_ATTR);		// )
	optionalStatements();			// stuff
	matchToken(KW_T, KW_else);		// else
	optionalStatements();		// stuff

	matchToken(KW_T, KW_end);		// end
	printf("%s%s\n", STR_LANGNAME, ": Selection statement parsed");
}
/*
* <iteration statement> ->  while (<conditional expression>)
								<statements>
							end

*/
zeus_void iterationWhileStatement() {
	matchToken(KW_T, KW_while);		// while
	matchToken(LPR_T, NO_ATTR);     // (
	conditionalExpression();		// condition
	matchToken(RPR_T, NO_ATTR);		// )
	statements();					// statements
	matchToken(KW_T, KW_end);		// end
	printf("%s%s\n", STR_LANGNAME, ": While-loop statement parsed");

}
/*
* <item> -> (integer_variable | integer_literal)
*/
zeus_void item() {
	// literals 
	switch (lookahead.code) {
	case INT_T:
		matchToken(INT_T, NO_ATTR);
		break;
	case VARID_T:
		matchToken(VARID_T, NO_ATTR);
		break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Item statement parsed");

}
/*
* <iterator> -> <item> : <item>
*/
zeus_void iterator() {
	item();
	matchToken(COLON_T, NO_ATTR);
	item();
	printf("%s%s\n", STR_LANGNAME, ": Iterator statement parsed");

};
/*
* <iterationForStatement> -> for(<item> in <iterator>)
*								<statements>
*							 end
*/
zeus_void iterationForStatement() {
	matchToken(KW_T, KW_for);		// for
	matchToken(LPR_T, NO_ATTR);		// (
	item();							// a | 1
	matchToken(KW_T, KW_in);		// in
	iterator();						// 1:10
	matchToken(RPR_T, NO_ATTR);		// )
	statements();
	matchToken(KW_T, KW_end);		// end
	printf("%s%s\n", STR_LANGNAME, ": For-loop statement parsed");

}

/*
 ************************************************************
 * Input statement
 * BNF: <input_statement> -> readline()
 ***********************************************************
 */
zeus_void inputStatement() {
	matchToken(KW_T, KW_readline);
	matchToken(LPR_T, NO_ATTR);
	matchToken(RPR_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
}

/*
 ************************************************************
 * Output Statement
 * <output statement> -> print(<outputStatementPrime>)
 ***********************************************************
 */

zeus_void outputStatement() {
	//matchToken(MNID_T, NO_ATTR);
	matchToken(KW_T, KW_print);
	matchToken(LPR_T, NO_ATTR);
	//outputVariableList();
	outputStatementPrime();
	matchToken(RPR_T, NO_ATTR);
	//matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}
/*
* <outputStatementPrime> -> STR_T | VARID_T
*/
zeus_void outputStatementPrime() {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case VARID_T:
		matchToken(VARID_T, NO_ATTR);
		break;
	}
}

/*
* <arithmetic expression> ->  <unary arithmetic expression> | <additive arithmetic expression>
*/
zeus_void arithmeticExpression() {
	//printf("code: %d", lookahead.code);
	switch (lookahead.code) {
	case LPR_T:
	case INT_T:
	case REAL_T:
	case VARID_T:
		additiveArithmeticExpression();
		break;
	case ARITH_OP_T:
		switch (lookahead.attribute.arithmeticOperator) {
		case OP_SUB:
		case OP_ADD:
			//additiveArithmeticExpression();
			unaryArithmeticExpression();
			break;
		}
		break;
	default:
		printError();
		break;
	}

}
/*
* <unary arithmetic expression> -> - <primary arithmetic expression> | + <primary arithmetic expression>
*/
zeus_void unaryArithmeticExpression() {
	switch (lookahead.attribute.arithmeticOperator) {
	case(OP_SUB):
		matchToken(ARITH_OP_T, OP_SUB);
		primaryArithmeticExpression();
		break;
	case(OP_ADD):
		matchToken(ARITH_OP_T, OP_ADD);
		primaryArithmeticExpression();
		break;
	default:
		printError();
		break;
	}
}
/*
*				A								A										alpha
* <additive arithmetic expression> -> <additive arithmetic expression>  +  <multiplicative arithmetic expression>
									  | <additive arithmetic expression>  -  <multiplicative arithmetic expression>
									  | <multiplicative arithmetic expression>
															beta

  <additive arithmetic expression> -> <multiplicative arithmetic expression><additive arithmetic expression prime>
*/
zeus_void additiveArithmeticExpression() {
	multiplicativeArithmeticExpression();
	additiveArithmeticExpressionPrime();
}
/*
* <additive arithmetic expression prime> -> + <multiplicative arithmetic expression><additive arithmetic expression prime>
*										  | - <multiplicative arithmetic expression><additive arithmetic expression prime>
*										  | e
*/
zeus_void additiveArithmeticExpressionPrime() {
	switch (lookahead.code) {
	case ARITH_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_ADD:
		case OP_SUB:
			matchToken(ARITH_OP_T, lookahead.code);
			multiplicativeArithmeticExpression();
			additiveArithmeticExpressionPrime();
			break;
		}
		break;
	}
}
/*
* A = <multiplicative arithmetic expression>
* a(alpha) = * <primary arithmetic expression> | <multiplicative arithmetic expression> / <primary arithmetic expression>
* beta = <primary arithmetic expression>
* need: A -> betaA'
*		A' -> alphaA'| e
* <multiplicative arithmetic expression> -> <multiplicative arithmetic expression> * <primary arithmetic expression>
											| <multiplicative arithmetic expression> / <primary arithmetic expression>
											| <primary arithmetic expression>

										 -> <primary arithmetic expression><multiplicative arithmetic expression prime>
*/
zeus_void multiplicativeArithmeticExpression() {
	primaryArithmeticExpression();
	multiplicativeArithmeticExpressionPrime();
}
/*
* need: A -> betaA'
*		A' -> alphaA'| e
* <multiplicative arithmetic expression prime> -> * <primary arithmetic expression>
												| <multiplicative arithmetic expression> / <primary arithmetic expression>
												<multiplicative arithmetic expression prime> | e
*/
zeus_void multiplicativeArithmeticExpressionPrime() {

	switch (lookahead.code) {
	case ARITH_OP_T:
		switch (lookahead.attribute.codeType) {
		case OP_MUL:
		case OP_DIV:
			matchToken(ARITH_OP_T, lookahead.code);
			multiplicativeArithmeticExpression();
			additiveArithmeticExpressionPrime();
			break;
		}
		break;
	}
}

/*
* <primary arithmetic expression> -> <integer_variable>
					| <real_variable>
					| REAL_T | INT_T
					| (<arithmetic expression>)

*/
zeus_void primaryArithmeticExpression() {
	switch (lookahead.code) {
	case VARID_T:
		matchToken(VARID_T, NO_ATTR);
		break;
	case INT_T:
		matchToken(INT_T, NO_ATTR);
		break;
	case REAL_T:
		matchToken(REAL_T, NO_ATTR);
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmeticExpression();
		matchToken(RPR_T, NO_ATTR);
		break;
	case ARITH_OP_T:
		arithmeticExpression();
		break;
	}
}

/*
* <string expression> -> -> <primary string expression><string expression Prime>
*/
zeus_void stringExpression() {
	primaryStringExpression();
	stringExpressionPrime();
}

/*
* <string expression prime> -> ** <primary string expression><string expression prime> | e
*/
zeus_void stringExpressionPrime() {
	if (lookahead.code == CONCAT_T) {
		matchToken(CONCAT_T, NO_ATTR);
		primaryStringExpression();
	}
}

/*
* <primary string expression>  ->  STR_T | VARID_T
*/
zeus_void primaryStringExpression() {
	//printf("%d", lookahead.code);
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		break;
	case VARID_T:
		matchToken(VARID_T, NO_ATTR);
		break;
	}

}

/*
* 	<char expression> -> CHAR_T | <char_variable>
*/
zeus_void charExpression() {
	switch (lookahead.code) {
	case CHAR_T:
		matchToken(CHAR_T, NO_ATTR);
		break;
	case VARID_T:
		matchToken(VARID_T, NO_ATTR);
		break;
	}
}
/*
* <conditional expression> -> <logical OR  expression>
*/
zeus_void conditionalExpression() {
	logical_OR_Expression();
	printf("%s%s\n", STR_LANGNAME, ": Conditional expression parsed");

}

/*
*
* BNF: <logical_OR_Expression> -> -> <logical_AND_Expression><logical_OR_ExpressionPrime> 
*
*							   
*/
zeus_void logical_OR_Expression() {
	logical_AND_Expression();
	logical_OR_ExpressionPrime();
}

/*
* BNF: <logical_OR_ExpressionPrime -> || <logical_AND_Expression><logical_OR_ExpressionPrime> | e
*/
zeus_void logical_OR_ExpressionPrime() {
	if (lookahead.code == OR_T) {
		matchToken(OR_T, NO_ATTR);
		logical_AND_Expression();
		logical_OR_ExpressionPrime();
	}
}

/*
* <logical_AND_expression> -> <logical_NOT_expression><logical_AND_ExpressionPrime>
*/
zeus_void logical_AND_Expression() {
	logical_NOT_Expression();
	logical_AND_ExpressionPrime();
}
/*
* <logical_AND_Expression_Prime> -> && <relational_NOT_expression><logical_AND_Expression_Prime> | ϵ
*/
zeus_void logical_AND_ExpressionPrime() {
	if (lookahead.code == AND_T) {
		matchToken(AND_T, NO_ATTR);
		logical_NOT_Expression();
		logical_AND_ExpressionPrime();
	}
}
/*
* <logical_NOT_expression> -> ! <relational expression> | <relational expression>
*/
zeus_void logical_NOT_Expression() {
	if (lookahead.code == NOT_T) {
		matchToken(NOT_T, NO_ATTR);
	}
	relationalExpression();
}

/*
* <relational expression> -> <relational a_expression>  | <relational s_expression>
*/
zeus_void relationalExpression() {
	switch (lookahead.code) {
	case INT_T:
	case REAL_T:
	case VARID_T:
		relational_a_Expression();
		break;
	case STR_T:
		relational_s_Expression();
		break;
	}
}

/*
*<relational a_expression> ->
		  <primary a_relational expression>  ==  <primary a_relational expression>
		| <primary a_relational  expression>  !=  <primary a_relational  expression>
		| <primary a_relational  expression>  >   <primary a_relational  expression>
		| <primary a_relational  expression>  >=   <primary a_relational  expression>
		| <primary a_relational  expression>  <   <primary a_relational  expression>
		| <primary a_relational expression>  <=   <primary a_relational expression>

		primary relational expresson == primary relational expression
*/
zeus_void relational_a_Expression() {

	primary_a_relationalExpression();

	switch (lookahead.attribute.relationalOperator) {
	case OP_EQ:
		matchToken(EQLITY_T, NO_ATTR);
		break;
	case OP_NE:
		matchToken(NEQU_T, NO_ATTR);
		break;
	case OP_GT:
		matchToken(GT_T, NO_ATTR);
		break;
	case OP_GTE:
		matchToken(GTE_T, NO_ATTR);
		break;
	case OP_LT:
		matchToken(LT_T, NO_ATTR);
		break;
	case OP_LTE:
		matchToken(LTE_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}

	primary_a_relationalExpression();

}
/*
* <primary a_relational expression> ->  <integer_variable> | <real_variable> | REAL_T  | INT_T
*/
zeus_void primary_a_relationalExpression() {

	switch (lookahead.code) {
	case REAL_T:
		matchToken(REAL_T, NO_ATTR);
		break;
	case INT_T:
		matchToken(INT_T, NO_ATTR);
		break;
	case VARID_T:
		matchToken(VARID_T, NO_ATTR);
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR); 
		break;
	default:
		printError();
		break;
	}
}

/*
* <relational s_expression> ->
		  <primary s_relational  expression>  ==  <primary s_relational expression>
		| <primary s_relational  expression>  !=  <primary s_relational  expression>
		| <primary s_relational  expression>  >   <primary s_relational  expression>
		| <primary s_relational  expression>  >=   <primary s_relational  expression>
		| <primary s_relational  expression>  <   <primary s_relational expression>
		| <primary s_relational  expression>  <=  <primary s_relational expression>
*/
zeus_void relational_s_Expression() {
	primary_s_relationalExpression();
	switch (lookahead.attribute.relationalOperator) {
	case OP_EQ:
		matchToken(EQLITY_T, NO_ATTR);
		break;
	case OP_NE:
		matchToken(NEQU_T, NO_ATTR);
		break;
	case OP_GT:
		matchToken(GT_T, NO_ATTR);
		break;
	case OP_GTE:
		matchToken(GTE_T, NO_ATTR);
		break;
	case OP_LT:
		matchToken(LT_T, NO_ATTR);
		break;
	case OP_LTE:
		matchToken(LTE_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	primary_s_relationalExpression();

}

/*
* <primary s_relational expression> -> <primary string expression>
*/
zeus_void primary_s_relationalExpression() {
	primaryStringExpression();
	printf("%s%s\n", STR_LANGNAME, ": Primary String Expression parsed");
}

/*
* <int var declaration>	-> int <variable identifier>
*/
zeus_void intVarDeclaration() {
	matchToken(KW_T, KW_int);
	variableIdentifier();
	printf("%s%s\n", STR_LANGNAME, ": integer variable declaration parsed");
}

/*
* <real var declaration>  -> real <variable identifier>
*/
zeus_void realVarDeclaration() {
	matchToken(KW_T, KW_real);
	variableIdentifier();
	printf("%s%s\n", STR_LANGNAME, ": real variable declaration parsed");
}
/*
* <string var declaration>  -> string <variable identifier>
*/
zeus_void stringVarDeclaration() {
	matchToken(KW_T, KW_string);
	variableIdentifier();
	printf("%s%s\n", STR_LANGNAME, ": string variable declaration parsed");
}
/*
* <char var declaration>  -> char <variable identifier>
*/
zeus_void charVarDeclaration() {
	matchToken(KW_T, KW_char);
	variableIdentifier();
	printf("%s%s\n", STR_LANGNAME, ": char variable declaration parsed");
}
/*
* <variable identifier> -> VARID_T
*/
zeus_void variableIdentifier() {
	matchToken(VARID_T, NO_ATTR);
}

/*
* <variable declaration expression> -> <int variable declaration>
*									  |<real variable declaration>
*									  |<string variable declaration>
*									  |<char variable declaration>
*/
zeus_void variableDeclarationExpression() {
	switch (lookahead.attribute.codeType) {
	case KW_int:
		intVarDeclaration();
		break;
	case KW_real:
		realVarDeclaration();
		break;
	case KW_string:
		stringVarDeclaration();
		break;
	case KW_char:
		charVarDeclaration();
		break;
	}
}