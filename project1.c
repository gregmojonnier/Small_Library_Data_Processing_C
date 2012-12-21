/**
* 
* This is a program to suffice the data processing
* needs of a small public library. It opens a patron/item
* list file upon execution and builds a dynamic data structure.
* Library patrons, available items and checked out items are all
* tracked. The user is then allowed to enter commands to transform
* the library's data. 
*
* Author: Greg Mojonnier
*
*/
#include <stdio.h>
#include <allocate.h>
#include "SanitizeInput.h"
#include "LinkedDataNodeOperations.h"

// Global variables to reduce program size from passing
// around ListNode pointers between functions
FILE* g_InputFile = NULL; 
ListNode* g_PatronsHead = NULL;
ListNode* g_ItemsHead = NULL;

int main( int argc, char *argv[] ){

	// User must supply patron_file and item_file
	if( argc != 3 ){
		fputs( "usuage:  project1 patron_file item_file\n", stderr );
		return( EXIT_FAILURE );
	}

	FILE* initialPatronsFile = fopen( argv[ 1 ], "r" );
	FILE* initialItemsFile = fopen( argv[ 2 ], "r" );
	_Bool isEitherInputFileNull = 0;

	if( initialPatronsFile == NULL ){
		perror( argv[ 1 ] );
		isEitherInputFileNull = 1;
	}
	if( initialItemsFile == NULL ){
		perror( argv[ 2 ] );
		isEitherInputFileNull = 1;
	}
	if( isEitherInputFileNull ){
		return( EXIT_FAILURE );
	}

	g_InputFile = initialPatronsFile;
	processInput();
	fclose( initialPatronsFile );

	g_InputFile = initialItemsFile;
	processInput();
	fclose( initialItemsFile );

	g_InputFile = NULL;
	processInput();

	deleteAndFreeBothLists();

	return( EXIT_SUCCESS );
}

