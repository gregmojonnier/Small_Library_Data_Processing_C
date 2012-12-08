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

// This type of linked list adding/removing will eventually
// go in ProcessCommands to help build the list
	ListNode* head = NULL;

	PatronData* p1 = (PatronData*) allocate( sizeof(PatronData) );

// normally name attributes will be dynamically allocated
	p1->name = "John Smith";
	char* pid1 = "abcd8";
	strcpy(p1->pid, pid1);

	PatronData* p2 = (PatronData*) allocate( sizeof(PatronData) );
	p2->name = "Tim Daniels";
	char* pid = "fafas";
	strcpy(p2->pid, pid);

	if( !insertNodeAtHead( &head, (void*)p1 ) ){
		puts("Error inserting node at head.");
	}

	printPatronLinkedList( head );

	if( !insertNodeAtHead( &head, (void*)p2 ) ){
		puts("Error inserting node at head.");
	}
	printPatronLinkedList( head );


	puts("Attempting to find John smith based on his PID.");
	
	ListNode* linkToJohn;
	if( linkToJohn = findNodeWithUID( head, pid1, doesPatronMatchUID ) ){
		puts("We found him!");
		printf("the node data we got is %s\n", ((PatronData*)(linkToJohn->data))->name );

	}
	else{
		puts("We did not find him.");
	}
	

	deleteAndFreeList( head, freePatronDataStruct );
	//unallocate( p1 );
	//unallocate( p2 );
}
