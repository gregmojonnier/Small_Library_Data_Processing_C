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
#include <stdlib.h>
#include <string.h>
#include "ExecuteCommands.h"
#include "LinkedDataNodeOperations.h"
#include <allocate.h>
#include "SanitizeInput.h"


int main( int argc, char *argv[] ){

	// User must supply patron_file and item_file
	if( argc != 3 ){
		// verify is tab required before output?
		fputs( "usuage:  project1 patron_file item_file\n", stderr );
		printf("%i", EXIT_FAILURE );
		printf("%i", EXIT_SUCCESS );
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

	fclose( initialPatronsFile );
	fclose( initialItemsFile );


	ListNode* patronsHead = NULL;
	ListNode* itemsHead = NULL;

	ProcessInput(&itemsHead, &patronsHead, argv[ 1 ] );
	ProcessInput(&itemsHead, &patronsHead, argv [ 2 ] );

	ProcessInput(&itemsHead, &patronsHead, NULL );

	deleteAndFreeList( patronsHead, freePatronDataStruct );
	deleteAndFreeList( itemsHead, freeItemDataStruct );

	return( EXIT_SUCCESS );
}

