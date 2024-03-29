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
* If g_InputFile is NULL we use STDIN, other wise the 
* file specified is the input source. For each line
* this function will appropriately Processing the command.
*
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
			else if( strcmp( parsedCommand, BORROW_ITEM_COMMAND ) == 0 || strcmp( parsedCommand, RETURN_ITEM_COMMAND ) == 0 ){
				const char* pid = strtok( 0, DEFAULT_WORD_SEPARATORS );
				const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

				if( pid != NULL && cid != NULL && isValidPID( pid ) && isValidCID( cid ) ){
					if( strcmp( parsedCommand, BORROW_ITEM_COMMAND ) == 0 ){
						borrowItem( pid, cid );	
					}
					else{
						returnPatronsItem( pid, cid );
					}
				}
			}
			else if( strcmp( parsedCommand, DISCARD_ITEM_COMMAND ) == 0 ){

				const char* numToDiscard = strtok( 0, DEFAULT_WORD_SEPARATORS );
				const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

				if( numToDiscard != NULL && cid != NULL ){
					long int nToDiscard = strtoul( numToDiscard, NULL, 10 );
					if( nToDiscard >= ITEM_NUMS_MIN_SIZE && nToDiscard <= ITEM_NUMS_MAX_SIZE && isValidCID( cid ) ){
						discardCopiesOfItem( nToDiscard, cid );
					}
				}
			}
			else if( strcmp( parsedCommand, OUT_COMMAND ) == 0 || strcmp( parsedCommand, AVAILABLE_ITEM_COMMAND ) == 0 ){
				const char* uid = strtok( 0, DEFAULT_WORD_SEPARATORS );

				if( uid != NULL ){
					if( isValidCID( uid ) ){
						if( strcmp( parsedCommand, OUT_COMMAND ) == 0 ){	
							patronsWithItemOut( uid );
						}
						else{
							getCopiesAvailable( uid );
						}
					}
					else if( isValidPID( uid ) ){
						itemsOutByPatron( uid );
					}
				}
			}
		}
	}
	if( g_InputFile == NULL ){
		// if using stdin then we need to print finising statuses of everything
		printf("\n");
		printAllListsStatus( );
	}
}

/*
* processPatronCommand
* ----------------------------------
*  
* Processes patron command input line and 
* calls addPatron function to create a new patron data.
*
*
* @return ------------------> None.
*
*/
void processPatronCommand(){

	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char pid[ PID_MAX_SIZE ];
	char truncatedName[ NAME_MAX_SIZE ];

	uint_least8_t tokensProcessed = 0;

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
				uint_least8_t nameLength = ( strlen( token ) >= NAME_MAX_SIZE ) ? getSizeToTrimTailTo( token, NAME_MAX_SIZE ) : strlen( token ) + 1;	
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

	if( truncatedName != NULL ){
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
*
* @return ------------------> None.
*
*/
void processItemCommand(){

	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char cid[ CID_MAX_SIZE ];
	char truncatedAuthor[ AUTHOR_MAX_SIZE ];
	char truncatedTitle[ TITLE_MAX_SIZE ];
	long int numCopies = 0;
	uint_least8_t tokensProcessed = 0;

	for( ; tokensProcessed < 6 && token != NULL; tokensProcessed++ ){
	
		switch( tokensProcessed ){
			case 0:
			  {
			  	// number of copies
				numCopies = strtoul( token, NULL, 10 );
				if(!(numCopies >= ITEM_NUMS_MIN_SIZE && numCopies <= ITEM_NUMS_MAX_SIZE) ){
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
			  }
			case 2:
			case 4:
			  {
				token = strtok( 0, QUOTE_WORD_SEPARATOR);
				break;
			  }
			case 3:
			  {
			  	// author
				uint_least8_t authorLength = ( strlen( token ) >= AUTHOR_MAX_SIZE ) ? getSizeToTrimTailTo( token, AUTHOR_MAX_SIZE ) : strlen( token ) + 1;	
				
				strncpy( truncatedAuthor, token, authorLength -1 );
				truncatedAuthor[ authorLength - 1 ] = '\0';

				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			case 5:
			  {
				uint_least8_t titleLength = ( strlen( token ) >= TITLE_MAX_SIZE ) ? getSizeToTrimTailTo( token, TITLE_MAX_SIZE ) : strlen( token ) + 1;	

				strncpy( truncatedTitle, token, titleLength -1 );
				truncatedTitle[ titleLength - 1 ] = '\0';

				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			default:
			  {
				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
		}
	}

	if( truncatedAuthor != NULL && truncatedTitle != NULL ){
		addItem( numCopies, cid, truncatedAuthor, truncatedTitle );
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
* @return ------------------> uint_least8_t(1 or 0) indicating CID validity.
*
*/
uint_least8_t isValidCID( const char* cid ){

	if( cid == NULL || strlen( cid ) < CID_MIN_SIZE-1 || strlen( cid ) > CID_MAX_SIZE-1 ){
		return 0;
	}

	const char* periodLocationIterator = strchr( cid, PERIOD_WORD_SEPARATOR_CH );
	if( periodLocationIterator == NULL ){
		return 0;
	}

	const char* cidIt = cid;

	while( cidIt != periodLocationIterator ){
		if( !isdigit( *cidIt ) ){	
			return 0;
		}
		++cidIt;
	}

	cidIt = periodLocationIterator;

	while( cidIt != &cid[ strlen( cid ) ]  ){
		if( !isdigit( *cidIt ) && *cidIt != '.' ){	
			return 0;
		}
		++cidIt;
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
* @return ------------------> uint_least8_t(1 or 0) indicating PID validity.
*
*/
uint_least8_t isValidPID( const char* pid ){

	if( pid != NULL && isupper( pid[ 0 ] ) ){
		for( uint_least8_t i = 1; i < PID_MAX_SIZE - 1; ++i ){
			if( !isdigit( pid[ i ] ) ){
				return 0;
			}
		}
		return 1;
	}
	return 0;
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
* @return ------------------> uint_least8_t that is size to trim token to.
*
*/
uint_least8_t getSizeToTrimTailTo( const char* token, uint_least8_t maxCharsInString ){
	if( token == NULL ){
		return 0;
	}

	uint_least8_t lastCharIndex = maxCharsInString - 2;

	while( token[ lastCharIndex ] == ' ' ){
		--lastCharIndex;
	}

	lastCharIndex += 2;
	return lastCharIndex;
}
