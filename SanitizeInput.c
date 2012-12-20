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
#include "AllConstants.h"


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
void processInput( ListNode** itemsHead, ListNode** patronsHead, FILE** inputFile ){

	_Bool useStdIn = 0;

	if( inputFile == NULL ){
		useStdIn = 1;
	}
	else if( *inputFile == NULL ){
		return;
	}

	char fullLine[ LINE_MAX_SIZE ];

	// get each line until end of file
	while( fgets( fullLine, LINE_MAX_SIZE, ( ( useStdIn ) ? stdin : *inputFile ) ) != NULL ){
		// parse first word of line based on word separators
		char* parsedCommand = strtok( fullLine, DEFAULT_WORD_SEPARATORS );;

		if( parsedCommand != NULL ){
			
			if( strcmp( parsedCommand, ADD_PATRON_COMMAND ) == 0 ){
				processPatronCommand( patronsHead );
			}
			else if( strcmp( parsedCommand, ADD_ITEM_COMMAND ) == 0 ){
				processItemCommand( itemsHead );
			}
			else if( strcmp( parsedCommand, "borrow" ) == 0 ){
				processBorrowCommand( itemsHead, patronsHead );
			}
			else if( strcmp( parsedCommand, "return" ) == 0 ){
				processReturnCommand( itemsHead, patronsHead );
			}
			else if( strcmp( parsedCommand, "discard" ) == 0 ){
				processDiscardCommand( itemsHead );
			}
			else if( strcmp( parsedCommand, "out" ) == 0 ){
				processOutCommand( itemsHead, patronsHead );
			}
			else if( strcmp( parsedCommand, "available" ) == 0 ){
				processAvailableCommand( itemsHead );
			}
		}
	}
	if( useStdIn ){
		// if using stdin then we need to print finising statuses of everything
		printf("\n");
		printAllItemsStatus( *itemsHead );
		printAllPatronsStatus( *patronsHead );
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
void processPatronCommand( ListNode** patronsHead ){

	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char* pid;
	char* truncatedName;
	int tokensProcessed = 0;

	for( ; tokensProcessed < 3 && token != NULL; tokensProcessed++ ){
		switch( tokensProcessed ){
			case 0:
			  {
				if( isValidPID( token ) ){
					pid = (char*) allocate( PID_MAX_SIZE * sizeof(char) );

					if( pid == NULL ){
						printf("Memory allocation failed!\n");
						return;
					}
					strncpy( pid, token, PID_MAX_SIZE - 1 );
					pid[ PID_MAX_SIZE - 1 ] = '\0';
				}
				else{
					// DONT CONTINUE???? ERROR?
					return;
				}
				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			case 2:
			  {
				int nameLength = ( strlen( token ) >= NAME_MAX_SIZE ) ? getSizeToTrimTailTo( token, NAME_MAX_SIZE ) : strlen( token ) + 1;	
				truncatedName = (char*) allocate(  nameLength * sizeof(char) );

				if( truncatedName == NULL ){
					printf("Memory allocation failed!\n");
					return;
				}
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
		addPatron( patronsHead, pid, truncatedName );
	}

	switch( tokensProcessed ){
		case 3:
			unallocate(truncatedName);
		case 1:
			unallocate( pid );
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
void processItemCommand( ListNode** itemsHead ){

// item numCopies CID "author" "title"
	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char* cid;
	char* truncatedAuthor;
	char* truncatedTitle;
	long int numCopies = 0;
	int tokensProcessed = 0;

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
					cid = (char*) allocate( strlen( token )+1 * sizeof(char) );

					if( cid == NULL ){
						printf("Memory allocation failed!\n");
						return;
					}
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
				int authorLength = ( strlen( token ) >= AUTHOR_MAX_SIZE ) ? getSizeToTrimTailTo( token, AUTHOR_MAX_SIZE ) : strlen( token ) + 1;	
				truncatedAuthor = (char*) allocate(  authorLength * sizeof(char) );

				if( truncatedAuthor == NULL ){
					printf("Memory allocation failed!\n");
					return;
				}
				strncpy( truncatedAuthor, token, authorLength -1 );
				truncatedAuthor[ authorLength - 1 ] = '\0';
				
				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			case 5:
			  {
				int titleLength = ( strlen( token ) >= TITLE_MAX_SIZE ) ? getSizeToTrimTailTo( token, TITLE_MAX_SIZE ) : strlen( token ) + 1;	

				truncatedTitle = (char*) allocate(  titleLength * sizeof(char) );

				if( truncatedTitle == NULL ){
					printf("Memory allocation failed!\n");
					return;
				}
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
		addItem( itemsHead, numCopies, cid, truncatedAuthor, truncatedTitle );
	}

	switch( tokensProcessed ){
		case 6:
			unallocate( truncatedTitle );
		case 4:
			unallocate( truncatedAuthor );
		case 2:
			unallocate( cid );
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
void processAvailableCommand( ListNode** itemsHead ){

	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	
	if( cid != NULL && isValidCID( cid ) ){
		getCopiesAvailable( *itemsHead, cid );
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
void processBorrowCommand( ListNode** itemsHead, ListNode** patronsHead ){
	
	const char* pid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( pid != NULL && cid != NULL && isValidPID( pid ) && isValidCID( cid ) ){
		borrowItem( itemsHead, patronsHead, pid, cid );	
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
void processDiscardCommand( ListNode** itemsHead ){

	const char* numToDiscard = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( numToDiscard != NULL && cid != NULL ){
		short int nToDiscard = getValidItemsNum( numToDiscard );

		if( nToDiscard >= 0 && isValidCID( cid ) ){
			discardCopiesOfItem( itemsHead, nToDiscard, cid );
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
void processOutCommand( ListNode** itemsHead, ListNode** patronsHead ){

	const char* uid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( uid != NULL ){
		if( isValidCID( uid ) ){
			patronsWithItemOut( *itemsHead, uid );
		}
		else if( isValidPID( uid ) ){
			itemsOutByPatron( *patronsHead, uid );
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
void processReturnCommand( ListNode** itemsHead, ListNode** patronsHead ){

	const char* pid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( pid != NULL && cid != NULL && isValidPID( pid ) && isValidCID ){
		returnPatronsItem( *itemsHead, *patronsHead, pid, cid );
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

	const char* periodLocationIterator = strchr( cid, PERIOD_WORD_SEPARATOR );
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
		for( size_t i = 1; i < PID_MAX_SIZE - 1; ++i ){
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
int getValidItemsNum( const char* num ){
	if( num == NULL ){
		return -1;
	}

	long int number = 0;
	number = strtoul( num, NULL, 10 );
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
int getSizeToTrimTailTo( const char* token, unsigned short int maxCharsInString ){
	if( token == NULL ){
		return 0;
	}

	int lastCharIndex = maxCharsInString - 2;

	while( token[ lastCharIndex ] == ' ' ){
		--lastCharIndex;
	}

	lastCharIndex += 2;
	return lastCharIndex;
}
