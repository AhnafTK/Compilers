/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: Ahnaf Kamal, Skylar Phanenhour
* Professors: Paulo Sousa
************************************************************

*/

/*
************************************************************
* File name: Compilers.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12, A22, A32.
* Date: Jan 01 2023
* Professor: Paulo Sousa
* Purpose: This file defines the functions called by main function.
* Function list: mainReader(), mainScanner(), mainParser().
*************************************************************/

#ifndef COMPILERS_H_
#define COMPILERS_H_

#define DEBUG 0

/*
 * ............................................................................
 * ADVICE 1:
 * Please check the "TODO" labels to develop your activity.
 *
 * ADVICE 2: This file must be updated according to each assignment
 * (see the progression: reader > scanner > parser).
 * ............................................................................
 */
 
/* Logical constants - adapt for your language */
#define ZEUS_TRUE  1
#define ZEUS_FALSE 0

/*
------------------------------------------------------------
Data types definitions
NOTE: Some types may not be directly used by your language,
		but they can be necessary for conversions.
------------------------------------------------------------
*/

/* TO_DO: Define your typedefs */
typedef void			zeus_void;			//Add to patch note, adding void keyword
typedef char			zeus_char;
typedef int				zeus_intg;
typedef float			zeus_real;
typedef char			zeus_string;

typedef unsigned char	zeus_boln;			// add to patch note
typedef unsigned char	zeus_byte;			// add to patch note, added byte.

typedef long			zeus_long;			//Add to patch note, adding long keyword/variable

/*
------------------------------------------------------------
Programs:
1: Reader - invokes MainReader code
2: Scanner - invokes MainScanner code
3: Parser - invokes MainParser code
------------------------------------------------------------
*/
enum PROGRAMS {
	PGM_READER	= 'R',
	PGM_SCANNER = 'S',
	PGM_PARSER	= 'P'
};

/*
------------------------------------------------------------
Main functions signatures
(Code will be updated during next assignments)
------------------------------------------------------------
*/
zeus_intg mainReader(zeus_intg argc, zeus_char** argv);

zeus_intg mainScanner(zeus_intg argc, zeus_char** argv);

zeus_intg mainParser(zeus_intg argc, zeus_char** argv);

zeus_void printLogo();

#endif
