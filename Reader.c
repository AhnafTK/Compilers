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
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main code for Reader (.c)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

ReaderPointer readerCreate(zeus_intg size, zeus_intg increment, zeus_intg mode) {
	ReaderPointer readerPointer;

	if (mode != 'F' && mode != 'A' && mode != 'M') { return NULL; };  // if the mode given isnt valid

	// if size is < 0 or too big then return null
	if (size < 0 || size > INT_MAX) {return NULL;}
	// if size is invalid/not provided
	if (!size) {
		size = READER_DEFAULT_SIZE;
		increment = READER_DEFAULT_INCREMENT;
	}
	// check if increment is unacceptable, return null if true
	if (increment < 0 || increment > INT_MAX) { return NULL;}
	if (!increment) {mode = MODE_FIXED;} // if no increment, then go with fixed

	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));
	if (!readerPointer) // if not calloc'ed properly, return null.
		return NULL;
	readerPointer->content = (zeus_char*)malloc(size); // malloc the size
	if (!readerPointer->content) {
		free(readerPointer); // prevent dangling pointer by freeing it if content not allocated
		return NULL; // return null
	}

	// after SET_EMP, should be 0100 000  (correct)
	readerPointer->flags = READER_DEFAULT_FLAG | SET_EMP; // OR the default flag with SET_EMP mask to set the bit, val is now 0100 0000 / 40 HEX
	readerPointer->size = size; // set the size to size
	readerPointer->increment = increment; // set the increment to increment

	if (mode == 'F') { readerPointer->mode = MODE_FIXED; }; // if the mode is F then set as fixed
	if (mode == 'A') { readerPointer->mode = MODE_ADDIT; }; // if the mode is A then set as addit 
	if (mode == 'M') { readerPointer->mode = MODE_MULTI; }; // if the mode is M then set as multi

	return readerPointer; // return readerPointer
}   

/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
*************************************************************
*/

ReaderPointer readerAddChar(ReaderPointer const readerPointer, zeus_char ch) {

	if (!readerPointer) return NULL; // if invalid readerPointer being passed 
	// if the ch being passed is too big or negative, then increment error and return the pointer.
	if (ch >= NCHAR || ch < 0) { readerPointer->numReaderErrors++; return readerPointer; } 

	zeus_char* tempReader = NULL; // temp reader for realloc
	zeus_intg newSize = 0; // holds new size generated based on mode 

	// if the size isn't full
	if (readerPointer->position.wrte * (zeus_intg)sizeof(zeus_char) < readerPointer->size) {
		// since not full, add the ch.
		readerPointer->content[readerPointer->position.wrte++] = ch; // add ch to content
		readerPointer->flags &= RST_EMP; // since content is not empty anymore , reset EMP bit to O.
		readerPointer->histogram[ch]++; // add to histogram
				
	} else {
		switch (readerPointer->mode) {
		case MODE_FIXED:
			readerPointer->flags |= SET_FUL; // since it's fixed, no realloc is possible. buffer is now full.
			return NULL; // return null
		case MODE_ADDIT:
			newSize = readerPointer->size + readerPointer->increment; // get new size for addit mode
			if (newSize < 0 || newSize > READER_MAX_SIZE) {  // if new size is invalid
				readerPointer->flags |= SET_FUL; // set to full since not reallocated
			}; 
			break;
		case MODE_MULTI:
			newSize = readerPointer->size * readerPointer->increment; // get new size for multi mode
			if (newSize < 0 || newSize > READER_MAX_SIZE) { // if new size is invalid
				readerPointer->flags |= SET_FUL;  // set to since not realloced
			}; 
			break;
		default:
			return NULL; // else, return NULL
		}
		// if all is well, then realloc the temp reader using newSize
		tempReader = (zeus_char*)realloc(readerPointer->content, newSize);
		if (!tempReader) { return NULL; }; // if invalid, return NULL

		// if the addresses aren't the same anymore.
		if ((tempReader != readerPointer->content)) {
			// REL must be set since the memory locations are different
			readerPointer->flags |= SET_REL;
		}

		readerPointer->content = tempReader; // apply to real reader.
		readerPointer->content[readerPointer->position.wrte++] = ch; // add the char
		if (readerIsEmpty(readerPointer)) { // check if reader is empty
			readerPointer->flags &= RST_EMP; // reset it 
		}
		readerPointer->flags &= RST_FUL; // reset the ful bit since it's been realloced
		readerPointer->histogram[ch]++; // add to histogram
		readerPointer->size = newSize; // size is now new size

	}
	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerClear(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // if invalid pointer, return false

	readerPointer->flags = READER_DEFAULT_FLAG; // flag to default
	readerPointer->position.mark = 0; // mark to 0 
	readerPointer->position.read = 0; // read to 0 
	readerPointer->position.wrte = 0; // wrte to 0

	return ZEUS_TRUE; // return true
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerFree(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // if the pointer passed is invalid

	// free the inner data first
	free(readerPointer->content);
	// set the content to NULL
	readerPointer->content = NULL;
	// free the actual pointer
	free(readerPointer);

	return ZEUS_TRUE; // return true
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerIsFull(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // if reader is invalid, return false

	//FUL: 1000			// when 1, reader content is not possible to include chars
	// ex -> 8 (1000) 4-> (0100)
	// chk->   (1000)	  (1000) // when 1 (full) check and result the same
	// result->(1000)     (0000) // when 0 (!full) check and result are diff
	// 0100 0000 (64)
	// 1000 0000 (x40)
	// 0000 0000

	zeus_intg checkResult = readerPointer->flags & CHK_FUL; // bitmask result stored in checkResult
	if (checkResult == CHK_FUL) {  // if the mask and the result are the same, the bit must be set
		return ZEUS_TRUE; // return true because true
	}
	return ZEUS_FALSE; // else return false
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerIsEmpty(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // if reader is invalid, return false
	
	/* TO_DO: Defensive programming */
	// ex: 12 -> 1100  | 8 -> 1000 
	//   CHK  -> 0100		  0100    when mask and result the same, the bit is set
	//           0100		  0000	  when mask and result are diff, the bit is not set.
	zeus_intg checkResult = readerPointer->flags & CHK_EMP; // result of the bitwise operation

	if (CHK_EMP == checkResult) { // if the mask and the result are the same, then it must be empty
		return ZEUS_TRUE; // return true because empty
	}

	return ZEUS_FALSE; // else return false
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerSetMark(ReaderPointer const readerPointer, zeus_intg mark) {
	// if pointer is invalid, or mark is less than 0 or mark is greater than position.wrte, then not valid
	if (!readerPointer || mark< 0 || mark > readerPointer->position.wrte)
		return ZEUS_FALSE; // return false
	/* TO_DO: Adjust mark */
	readerPointer->position.mark = mark;
	return ZEUS_TRUE; // return true
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
*************************************************************
*/
zeus_intg readerPrint(ReaderPointer const readerPointer) {
	if (!readerPointer) return READER_ERROR; // if the pointer is invalid
	zeus_intg cont = 0; // count
	zeus_char c; // char

	/* TO_DO: Defensive programming (including invalid chars) */
	c = readerGetChar(readerPointer);
	
	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
		/* TO_DO: Check flag if buffer EOB has achieved */
		int checkValue = readerGetFlags(readerPointer) & CHK_END;
		//		0001 0000		0000 0000
		//mask  0001 0000		0001 0000
		//res	0001 0000		0000 0000

		if (checkValue == CHK_END) { //end of buffer has been achieved.
			break;
		}
		   
	}
	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
*************************************************************
*/
zeus_intg readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	if (!readerPointer || !fileDescriptor) return READER_ERROR; // if the reader is invalid or the filedesc is invalid thne return reader_error
	zeus_intg size = 0; // holds size
	zeus_char c; // char

	c = (zeus_char)fgetc(fileDescriptor); // reads char from file
	while (!feof(fileDescriptor)) { // while not end of file
		if (!readerAddChar(readerPointer, c)) { // if the value returned by readerAddChar is null
			ungetc(c, fileDescriptor); // unget 
			return READER_ERROR; // return error
		}
		c = (char)fgetc(fileDescriptor); // get next char
		size++; // increase size
	}

	return size; // return the size
}


/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerRecover(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // if pointer is null then return false
	readerPointer->position.read = 0; // set the read to 0
	readerPointer->position.mark = 0; // set the mark to 0
	return ZEUS_TRUE; // return true
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerRetract(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // if pointer is null, return false
	if (readerPointer->position.read < 0) { return ZEUS_FALSE; } // if the read pos is less than 0 then return false
	readerPointer->position.read--; // dec the reader pos
	return ZEUS_TRUE; // return true
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
*************************************************************
*/
zeus_boln readerRestore(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // if the reader is invalid
	// sets read offset to the val of mark offset
	readerPointer->position.read = readerPointer->position.mark;
	return ZEUS_TRUE; // return true
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
*************************************************************
*/
zeus_char readerGetChar(ReaderPointer const readerPointer) {
	if (!readerPointer) return READER_ERROR; // if the pointer is invalid then return error

	// if reader and write positions are the same, then it returns \0 and sets END bit to 1
	if (readerPointer->position.read == readerPointer->position.wrte) {
		// set END bit to one
		readerPointer->flags |= SET_END;
		return ('\0'); // return \0
	}
	else {
		readerPointer->flags &= RST_END; // reset to 0
	}

	return readerPointer->content[readerPointer->position.read++]; // increase the read position
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
*************************************************************
*/
zeus_char* readerGetContent(ReaderPointer const readerPointer, zeus_intg pos) {
	if (!readerPointer) return NULL; // check to see if the pointer is invalid
	// if the pos conditions are met, return the readerPointer content + pos
	if (pos >= 0 || pos <= readerPointer->position.wrte) { return (readerPointer->content + pos); };

	return NULL; // else return null
}



/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
*************************************************************
*/
zeus_intg readerGetPosRead(ReaderPointer const readerPointer) {
	if (!readerPointer) return READER_ERROR; // check to see if the pointer is invalid

	/* Return read */
	return (readerPointer->position.read);
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
*************************************************************
*/
zeus_intg readerGetPosWrte(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // check to see if the pointer is invalid

	/* Return wrte */
	return (readerPointer->position.wrte);
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.

*************************************************************
*/
zeus_intg readerGetPosMark(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // check to see if the pointer is invalid

	/* Return mark */
	return (readerPointer->position.mark);
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
*************************************************************
*/
zeus_intg readerGetSize(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // check to see if the pointer is invalid

	//return size
	return (readerPointer->size);
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
*************************************************************
*/
zeus_intg readerGetInc(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // check to see if the pointer is invalid
	return (readerPointer->increment); // return increment
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
*************************************************************
*/
zeus_intg readerGetMode(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // check to see if pointer is invalid
	return(readerPointer->mode); // return the mode.
}


/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
*************************************************************
*/
zeus_byte readerGetFlags(ReaderPointer const readerPointer) {
	if (!readerPointer) return ZEUS_FALSE; // check to see if pointer is invalid
	return (readerPointer->flags); // return the flags
}


/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
*************************************************************
*/
zeus_intg readerShowStat(ReaderPointer const readerPointer) {
	if (!readerPointer) return READER_ERROR; // if the pointeri s invalid, return error
	zeus_intg numDifferentChars = 0; // used to store different num of chars
	for (int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0) { // if the current index contains anything, then increment num chars
			numDifferentChars++; // inc numchars
		}
	}
	return numDifferentChars; // return the val
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
*************************************************************
*/
zeus_intg readerNumErrors(ReaderPointer const readerPointer) {
	if (!readerPointer) return READER_ERROR; // if the reader is invalid
	return (readerPointer->numReaderErrors); // return num of errors
}
