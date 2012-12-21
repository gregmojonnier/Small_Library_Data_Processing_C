/*
* This file contains methods which read commands in
* from the input.(Either from a file or STDIN)
* It then sanitize/parses the input to come up with the legal 
* commands which it then calls methods from ExecuteCommands.h
* to execute these commands.
*
*
* @author Greg Mojonnier
*/

#include "SanitizeInput.h"
#include "ExecuteCommands.h"
#include <string.h>
#include <ctype.h>
#include <allocate.h>
#include <stdlib.h>
#include <stdio.h>
#include "AllConstants.h"

extern FILE* g_InputFile;

/*
* processInput
* ----------------------------------
*  
* Entry point for input processing. This will
* loop through line by line of the input source.
* If fileName is NULL we use STDIN, other wise the 
* file specified is the input source. For each line
* this function will call a Processing function for
* the appropriate command.
*
* @itemsHead ---------------> Item's list to construct from input commands.
* @patronsHead -------------> Patron's list to construct from input commands.
* @inputFile ---------------> File of input source, if NULL then use STDIN.
*
* @return ------------------> None.
*
*/
void processInput(){

	char fullLine[ LINE_MAX_SIZE ];

	// get each line until end of file
	while( fgets( fullLine, LINE_MAX_SIZE, ( ( g_InputFile == NULL ) ? stdin : g_InputFile ) ) != NULL ){
		// parse first word of line based on word separators
		char* parsedCommand = strtok( fullLine, DEFAULT_WORD_SEPARATORS );;

		if( parsedCommand != NULL ){
			
			if( strcmp( parsedCommand, ADD_PATRON_COMMAND ) == 0 ){
				processPatronCommand();
			}
			else if( strcmp( parsedCommand, ADD_ITEM_COMMAND ) == 0 ){
				processItemCommand();
			}
			else if( strcmp( parsedCommand, BORROW_ITEM_COMMAND ) == 0 ){
				processBorrowCommand();
			}
			else if( strcmp( parsedCommand, RETURN_ITEM_COMMAND ) == 0 ){
				processReturnCommand();
			}
			else if( strcmp( parsedCommand, DISCARD_ITEM_COMMAND ) == 0 ){
				processDiscardCommand();
			}
			else if( strcmp( parsedCommand, OUT_COMMAND ) == 0 ){
				processOutCommand();
			}
			else if( strcmp( parsedCommand, AVAILABLE_ITEM_COMMAND ) == 0 ){
				processAvailableCommand();
			}
		}
	}
	if( g_InputFile == NULL ){
		// if using stdin then we need to print finising statuses of everything
		printf("\n");
		printAllItemsStatus( );
		printAllPatronsStatus( );
	}
}

/*
* processPatronCommand
* ----------------------------------
*  
* Processes patron command input line and 
* calls addPatron function to create a new patron data.
*
* @patronsHead -------------> Patron's list to add new patron to.
*
* @return ------------------> None.
*
*/
void processPatronCommand(){

	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char pid[ PID_MAX_SIZE ];
	char truncatedName[ NAME_MAX_SIZE ];

	unsigned char tokensProcessed = 0;

	for( ; tokensProcessed < 3 && token != NULL; tokensProcessed++ ){
		switch( tokensProcessed ){
			case 0:
			  {
				if( isValidPID( token ) ){
					strncpy( pid, token, PID_MAX_SIZE - 1 );
					pid[ PID_MAX_SIZE - 1 ] = '\0';
				}
				else{
					return;
				}
				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			case 2:
			  {
				unsigned char nameLength = ( strlen( token ) >= NAME_MAX_SIZE ) ? getSizeToTrimTailTo( token, NAME_MAX_SIZE ) : strlen( token ) + 1;	
				strncpy( truncatedName, token, nameLength -1 );
				truncatedName[ nameLength - 1 ] = '\0';
			  }
			default:
			  {
				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
		}
	}

	if( pid != NULL && truncatedName != NULL ){
		addPatron( pid, truncatedName );
	}
}

/*
* processItemCommand
* ----------------------------------
*  
* Processes item command input line and 
* calls addItem function to create a new item data.
*
* @itemsHead -------==------> Item's list to add new item to.
*
* @return ------------------> None.
*
*/
void processItemCommand(){

// item numCopies CID "author" "title"
	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char cid[ CID_MAX_SIZE ];
	char truncatedAuthor[ AUTHOR_MAX_SIZE ];
	char truncatedTitle[ TITLE_MAX_SIZE ];
	unsigned char numCopies = 0;
	unsigned char tokensProcessed = 0;

	for( ; tokensProcessed < 6 && token != NULL; tokensProcessed++ ){
	
		switch( tokensProcessed ){
			case 0:
			  {
				numCopies = getValidItemsNum( token );
				if( numCopies < 0 ){
					return;
				}

				token = strtok( 0, DEFAULT_WORD_SEPARATORS  );
				break;
			  }
			case 1:
			  {
				// CID
				if( isValidCID( token ) ){

					strncpy( cid, token, strlen( token ) );
					cid[ strlen( token ) ] = '\0';
				}
				else{
					return;
				}
				token = strtok( 0, QUOTE_WORD_SEPARATOR ); 
				break;
			  }
			case 2:
			case 4:
			  {
				token = strtok( 0, QUOTE_WORD_SEPARATOR);
				break;
			  }
			case 3:
			  {
				unsigned char authorLength = ( strlen( token ) >= AUTHOR_MAX_SIZE ) ? getSizeToTrimTailTo( token, AUTHOR_MAX_SIZE ) : strlen( token ) + 1;	
				
				strncpy( truncatedAuthor, token, authorLength -1 );
				truncatedAuthor[ authorLength - 1 ] = '\0';

				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			case 5:
			  {
				unsigned char titleLength = ( strlen( token ) >= TITLE_MAX_SIZE ) ? getSizeToTrimTailTo( token, TITLE_MAX_SIZE ) : strlen( token ) + 1;	

				strncpy( truncatedTitle, token, titleLength -1 );
				truncatedTitle[ titleLength - 1 ] = '\0';

				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			default:
			  {
				token = strtok( 0, QUOTE_WORD_SEPARATOR );
			  }
				break;
		}
	}

	if( cid != NULL && truncatedAuthor != NULL && truncatedTitle != NULL ){
		addItem( numCopies, cid, truncatedAuthor, truncatedTitle );
	}
}


/*
* processAvailableCommand
* ----------------------------------
*  
* Processes available command input line and 
* calls getCopiesAvailable function to output copies of item available.
*
* @itemsHead -------==------> Item's list to determine copies available from.
*
* @return ------------------> None.
*
*/
void processAvailableCommand(){

	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	
	if( cid != NULL && isValidCID( cid ) ){
		getCopiesAvailable( cid );
	}
}


/*
* processBorrowCommand
* ----------------------------------
*  
* Processes borrow command input line and 
* calls borrowItem function.
*
* @itemsHead -------==------> Item's list to find item were borrowing from.
* @patronsHead -----==------> Patrons's list to find patron thats borrowing from.
*
* @return ------------------> None.
*
*/
void processBorrowCommand(){
	
	const char* pid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( pid != NULL && cid != NULL && isValidPID( pid ) && isValidCID( cid ) ){
		borrowItem( pid, cid );	
	}
}

/*
* processDiscardCommand
* ----------------------------------
*  
* Processes discard command input line and 
* calls discardCopiesOfItem function.
*
* @itemsHead -------==------> Item's list to discard copies of item from.
*
* @return ------------------> None.
*
*/
// discard n cid
void processDiscardCommand(){

	const char* numToDiscard = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( numToDiscard != NULL && cid != NULL ){
		unsigned char nToDiscard = getValidItemsNum( numToDiscard );

		if( nToDiscard >= 0 && isValidCID( cid ) ){
			discardCopiesOfItem( nToDiscard, cid );
		}
	}
}

/*
* processOutCommand
* ----------------------------------
*  
* Processes out command input line and 
* calls patronsWithItemOut or itemsOutByPatron
* depending on if it is with a CID or PID;
*
* @itemsHead -------==------> Item's list.
* @patronsHead -----==------> Patrons's list.
*
* @return ------------------> None.
*
*/
void processOutCommand(){

	const char* uid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( uid != NULL ){
		if( isValidCID( uid ) ){
			patronsWithItemOut( uid );
		}
		else if( isValidPID( uid ) ){
			itemsOutByPatron( uid );
		}
	}
}

/*
* processReturnCommand
* ----------------------------------
*  
* Processes return command input line and 
* calls returnPatronsItem.
*
* @itemsHead -------==------> Item's list.
* @patronsHead -----==------> Patrons's list.
*
* @return ------------------> None.
*
*/
void processReturnCommand(){

	const char* pid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( pid != NULL && cid != NULL && isValidPID( pid ) && isValidCID ){
		returnPatronsItem( pid, cid );
	}
}

/*
* isValidCID
* ----------------------------------
*  
* Determines if a string is a valid CID.
*
* @cid -------------==------> Char* to be checked.
*
* @return ------------------> _Bool indicating CID validity.
*
*/
_Bool isValidCID( const char* cid ){

	if( cid == NULL ){
		return 0;
	}

	if( strlen( cid ) < CID_MIN_SIZE-1 || strlen( cid ) > CID_MAX_SIZE-1 ){
		return 0;
	}

	const char* periodLocationIterator = strchr( cid, PERIOD_WORD_SEPARATOR_CH );
	if( periodLocationIterator == NULL ){
		// Error needs period?
		return 0;
	}


	const char* start = cid;

	while( start != periodLocationIterator ){
		char ch = *start;
		if( !isdigit( ch ) ){	
			// not digits before error?
			return 0;
		}
		++start;
	}

	const char* middle = periodLocationIterator;

	while( middle != &cid[ strlen( cid ) ]  ){
		char ch = *middle;
		if( !isdigit( ch ) && ch != '.' ){	
			// not digits before error?
			return 0;
		}
		++middle;
	}
	return 1;
}

/*
* isValidPID
* ----------------------------------
*  
* Determines if a string is a valid PID.
*
* @pid -------------==------> Char* to be checked.
*
* @return ------------------> _Bool indicating PID validity.
*
*/
_Bool isValidPID( const char* pid ){

	if( pid == NULL ){
		return 0;
	}

	if( isupper( pid[ 0 ] ) ){
		for( unsigned char i = 1; i < PID_MAX_SIZE - 1; ++i ){
			if( !isdigit( pid[ i ] ) ){
				// ERROR? must be all digits after uppercase letter
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

/*
* getValidItemsNum
* ----------------------------------
*  
* Takes a string read in from input
* converts it to a number and determines if its
* a valid Item's number. -1 is returned if it is not.
*
* @num -------------==------> Char* to be checked.
*
* @return ------------------> Int that was converted from char*.
*
*/
char getValidItemsNum( const char* num ){
	if( num == NULL ){
		return -1;
	}

	long int number = strtoul( num, NULL, 10 );
	if( number < ITEM_NUMS_MIN_SIZE || number > ITEM_NUMS_MAX_SIZE ){
		return -1;
	}
	return number;
}

/*
* getSizeToTrimTailTo
* ----------------------------------
*  
* Takes a string read in from input
* and the maximum size is should be.'
* It then will return the indice that would trim
* trailing white space from the maximum size.
*
* @token -----------==------> Char* to find size to trim to. 
* @maxCharsInString -=------> Maximum number of chars token should be.
*
* @return ------------------> Int that is size to trim token to.
*
*/
unsigned char getSizeToTrimTailTo( const char* token, unsigned short int maxCharsInString ){
	if( token == NULL ){
		return 0;
	}

	unsigned char lastCharIndex = maxCharsInString - 2;

	while( token[ lastCharIndex ] == ' ' ){
		--lastCharIndex;
	}

	lastCharIndex += 2;
	return lastCharIndex;
}
