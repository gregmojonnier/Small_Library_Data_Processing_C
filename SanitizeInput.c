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
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <allocate.h>
#include <stdlib.h>
#include "AllConstants.h"


void ProcessInput( ListNode** itemsHead, ListNode** patronsHead, const char* fileName ){

	_Bool useStdIn = 0;
	FILE* inputFile;

	if( fileName == NULL ){
		useStdIn = 1;
	}

	if( !useStdIn ){
		inputFile = fopen( fileName, "r" );

		if( inputFile == NULL ){
			return;
		}
	}

	char fullLine[ LINE_MAX_SIZE ];

	// get each line until end of file
	while( fgets( fullLine, LINE_MAX_SIZE, ( ( useStdIn ) ? stdin : inputFile ) ) != NULL ){
		//printf("The entire string is %s\n", fullLine);
		// parse first word of line based on word separators
		char* parsedCommand = strtok( fullLine, DEFAULT_WORD_SEPARATORS );;

		if( parsedCommand != NULL ){
			
			if( strcmp( parsedCommand, ADD_PATRON_COMMAND ) == 0 ){
				ProcessPatronCommand( patronsHead );
			}
			else if( strcmp( parsedCommand, ADD_ITEM_COMMAND ) == 0 ){
				ProcessItemCommand( itemsHead );
			}
			else if( strcmp( parsedCommand, "borrow" ) == 0 ){
				ProcessBorrowCommand( itemsHead, patronsHead );
			}
			else if( strcmp( parsedCommand, "return" ) == 0 ){
				ProcessReturnCommand( itemsHead, patronsHead );
			}
			else if( strcmp( parsedCommand, "discard" ) == 0 ){
				ProcessDiscardCommand( itemsHead );
			}
			else if( strcmp( parsedCommand, "out" ) == 0 ){
				ProcessOutCommand( itemsHead, patronsHead );
			}
			else if( strcmp( parsedCommand, "available" ) == 0 ){
				ProcessAvailableCommand( itemsHead );
			}
		}
	}
	if( !useStdIn ){
		fclose( inputFile );
	}
	else{
		// if using stdin then we need to print finising statuses of everything
		printf("\n");
		printAllItemsStatus( *itemsHead );
		printAllPatronsStatus( *patronsHead );
	}
}

void ProcessPatronCommand( ListNode** patronsHead ){

	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char* pid;
	char* truncatedName;
	int tokensProcessed = 0;

	for( ; tokensProcessed < 3 && token != NULL; tokensProcessed++ ){
		switch( tokensProcessed ){
			case 0:
			  {
				if( IsValidPID( token ) ){
					pid = (char*) allocate( PID_MAX_SIZE * sizeof(char) );
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
				int nameLength = ( strlen( token ) >= NAME_MAX_SIZE ) ? GetSizeToTrimTailTo( token, NAME_MAX_SIZE ) : strlen( token ) + 1;	
				truncatedName = (char*) allocate(  nameLength * sizeof(char) );
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

// item numCopies CID "author" "title"
void ProcessItemCommand( ListNode** itemsHead ){

	const char* token = strtok( 0, DEFAULT_WORD_SEPARATORS );
	char* cid;
	char* truncatedAuthor;
	char* truncatedTitle;
	long int numCopies = 0;
	int tokensProcessed = 0;

	for( ; tokensProcessed < 6 && token != NULL; tokensProcessed++ ){
		//printf("current token is %i\n", tokensProcessed );
		//printf("current token is %s\n", token );
	
		switch( tokensProcessed ){
			case 0:
			  {
				numCopies = GetValidItemsNum( token );
				if( numCopies < 0 ){
					return;
				}

				token = strtok( 0, DEFAULT_WORD_SEPARATORS  );
				break;
			  }
			case 1:
			  {
				// CID
				if( IsValidCID( token ) ){
					cid = (char*) allocate( strlen( token )+1 * sizeof(char) );
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
				int authorLength = ( strlen( token ) >= AUTHOR_MAX_SIZE ) ? GetSizeToTrimTailTo( token, AUTHOR_MAX_SIZE ) : strlen( token ) + 1;	
				truncatedAuthor = (char*) allocate(  authorLength * sizeof(char) );
				strncpy( truncatedAuthor, token, authorLength -1 );
				truncatedAuthor[ authorLength - 1 ] = '\0';
				
				token = strtok( 0, QUOTE_WORD_SEPARATOR );
				break;
			  }
			case 5:
			  {
				int titleLength = ( strlen( token ) >= TITLE_MAX_SIZE ) ? GetSizeToTrimTailTo( token, TITLE_MAX_SIZE ) : strlen( token ) + 1;	

				truncatedTitle = (char*) allocate(  titleLength * sizeof(char) );
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

void ProcessAvailableCommand( ListNode** itemsHead ){

	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	
	if( cid != NULL && IsValidCID( cid ) ){
		getCopiesAvailable( *itemsHead, cid );
	}
}

void ProcessBorrowCommand( ListNode** itemsHead, ListNode** patronsHead ){
	
	const char* pid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( pid != NULL && cid != NULL && IsValidPID( pid ) && IsValidCID( cid ) ){
		borrowItem( itemsHead, patronsHead, pid, cid );	
	}
}

// discard n cid
void ProcessDiscardCommand( ListNode** itemsHead ){

	const char* numToDiscard = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( numToDiscard != NULL && cid != NULL ){
		short int nToDiscard = GetValidItemsNum( numToDiscard );

		if( nToDiscard >= 0 && IsValidCID( cid ) ){
			discardCopiesOfItem( itemsHead, nToDiscard, cid );
		}
	}
}

void ProcessOutCommand( ListNode** itemsHead, ListNode** patronsHead ){

	const char* uid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( uid != NULL ){
		if( IsValidCID( uid ) ){
			patronsWithItemOut( *itemsHead, uid );
		}
		else if( IsValidPID( uid ) ){
			itemsOutByPatron( *patronsHead, uid );
		}
	}
}

void ProcessReturnCommand( ListNode** itemsHead, ListNode** patronsHead ){

	const char* pid = strtok( 0, DEFAULT_WORD_SEPARATORS );
	const char* cid = strtok( 0, DEFAULT_WORD_SEPARATORS );

	if( pid != NULL && cid != NULL && IsValidPID( pid ) && IsValidCID ){
		returnPatronsItem( *itemsHead, *patronsHead, pid, cid );
	}
}

_Bool IsValidCID( const char* cid ){

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
		//printf("the char is %s\n", &ch );
		if( !isdigit( ch ) ){	
			//printf("1.NOT  digit%sQQ\n", start);
			// not digits before error?
			return 0;
		}
		++start;
	}

	const char* middle = periodLocationIterator;

	while( middle != &cid[ strlen( cid ) ]  ){
		char ch = *middle;
		//printf("the char is %s\n", &ch );
		if( !isdigit( ch ) && ch != '.' ){	
			//printf("1.NOT  digit %s\n", middle);
			// not digits before error?
			return 0;
		}
		++middle;
	}
	return 1;
}

_Bool IsValidPID( const char* pid ){

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

int GetValidItemsNum( const char* num ){
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

int GetSizeToTrimTailTo( const char* token, unsigned short int maxCharsInString ){
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
