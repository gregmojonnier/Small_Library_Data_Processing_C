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
#include "ProcessCommands.h"
#include "LinkedDataNodeOperations.h"
#include <allocate.h>

void TestLinkedListStuff();


int main( int argc, char *argv[] ){
	TestLinkedListStuff();

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

	return( EXIT_SUCCESS );
}

void TestLinkedListStuff(){

	ListNode* patronsHead = NULL;

	addPatron( &patronsHead, "p15369", "Greg M" );
	printPatronLinkedList( patronsHead );
	addPatron( &patronsHead, "p34831", "Harry Y" );
	printPatronLinkedList( patronsHead );
	addPatron( &patronsHead, "p34831", "Herb I" );
	printPatronLinkedList( patronsHead );
	addPatron( &patronsHead, "p97223", "Sarah S" );
	printPatronLinkedList( patronsHead );
	deleteAndFreeList( patronsHead, freePatronDataStruct );


	ListNode* itemsHead = NULL;

	addItem( &itemsHead, 4, "c135923", "Roth R", "Wonderful Book" );
	printf("Copies available are %i\n", getCopiesAvailable( itemsHead, "c135923" ) );
	printItemLinkedList( itemsHead );
	addItem( &itemsHead, 8, "c328424", "J K", "Once Upon A Time" );
	printf("Copies available are %i\n", getCopiesAvailable( itemsHead, "c328424" ) );
	printItemLinkedList( itemsHead );
	addItem( &itemsHead, 2, "c988383", "Roth R", "Wonderful Book pt 2" );
	printf("Copies available are %i\n", getCopiesAvailable( itemsHead, "c988383" ) );
	printItemLinkedList( itemsHead );

	printf("The list size is %i\n", getListSize( itemsHead ) );
	deleteAndFreeList( itemsHead, freeItemDataStruct );
}
