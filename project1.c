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
#include <allocate.h>
#include "SanitizeInput.h"
#include "LinkedDataNodeOperations.h"

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

	ListNode* patronsHead = NULL;
	ListNode* itemsHead = NULL;

	processInput(&itemsHead, &patronsHead, &initialPatronsFile );
	processInput(&itemsHead, &patronsHead, &initialItemsFile );
	processInput(&itemsHead, &patronsHead, NULL );

	fclose( initialPatronsFile );
	fclose( initialItemsFile );

	deleteAndFreeList( patronsHead, freePatronDataStruct );
	deleteAndFreeList( itemsHead, freeItemDataStruct );

	return( EXIT_SUCCESS );
}

