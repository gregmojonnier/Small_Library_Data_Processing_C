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

	const char* PID_1 = "p1535";
	const char* PID_2 = "p3222";
	const char* PID_3 = "p3483";
	const char* PID_4 = "p9722";

	const char* CID_1 = "c135923";
	const char* CID_2 = "c776622";
	const char* CID_3 = "c328424";
	const char* CID_4 = "c988383";


	ListNode* patronsHead = NULL;
	ListNode* itemsHead = NULL;

	addPatron( &patronsHead, PID_1, "Greg M" );
	addPatron( &patronsHead, PID_2, "Harry Y" );
	addPatron( &patronsHead, PID_3, "Herb I" );
	addPatron( &patronsHead, PID_4, "Sarah S" );

	printPatronLinkedList( patronsHead );
	printf("The Patron's list size is %i\n", getListSize( patronsHead ) );

	addItem( &itemsHead, 4, CID_1, "Roth R", "Wonderful Book" );
	addItem( &itemsHead, 2, CID_2, "J K", "Once Upon A Time" );
	addItem( &itemsHead, 8, CID_3, "Roth R", "Wonderful Book pt 2" );
	addItem( &itemsHead, 3, CID_4, "Kenny Z", "The Cook Book" );

	printItemLinkedList( itemsHead );
	printf("The Item's list size is %i\n", getListSize( itemsHead ) );


	patronsWithItemOut( itemsHead, patronsHead, CID_2 );
	itemsOutByPatron( itemsHead, patronsHead, PID_1 );
	printf("\n\n\n+++++++++++++++++++++++++++++++++++++++\n");
	printf("There are currently %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2 );
	printf("The patron %s is about to try to check out %s\n", PID_1, CID_2);
	printf("The result of borrow function was %i\n", borrowItem( &itemsHead, &patronsHead, PID_1, CID_2 ));
	printf("There are now %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2);
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");

	patronsWithItemOut( itemsHead, patronsHead, CID_2 );
	printf("ABOUT TO TRY TO HAVE %s return %s\n", PID_1, CID_2);
	printf("%i\n", returnPatronsItem( itemsHead, patronsHead, PID_1, CID_2));
	patronsWithItemOut( itemsHead, patronsHead, CID_2 );

	itemsOutByPatron( itemsHead, patronsHead, PID_1 );
	borrowItem( &itemsHead, &patronsHead, PID_1, CID_3 );
	itemsOutByPatron( itemsHead, patronsHead, PID_1 );


	printf("\n\n\n+++++++++++++++++++++++++++++++++++++++\n");
	printf("There are currently %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2 );
	printf("The patron %s is about to try to check out ANOTHER copy of %s\n", PID_1, CID_2);
	printf("The result of borrow function was %i\n", borrowItem( &itemsHead, &patronsHead, PID_1, CID_2 ));
	printf("There are now %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2);
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");


	printf("\n\n\n+++++++++++++++++++++++++++++++++++++++\n");
	printf("There are currently %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2 );
	printf("The patron %s is about to try to check out %s\n", PID_2, CID_2);
	printf("The result of borrow function was %i\n", borrowItem( &itemsHead, &patronsHead, PID_2, CID_2 ));
	printf("There are now %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2);
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");

	patronsWithItemOut( itemsHead, patronsHead, CID_2 );

	printf("\n\n\n+++++++++++++++++++++++++++++++++++++++\n");
	printf("There are currently %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2 );
	printf("The patron %s is about to try to check out AN ITEM THAT SHOULD BE OUT OF STOCK %s\n", PID_3, CID_2);
	printf("The result of borrow function was %i\n", borrowItem( &itemsHead, &patronsHead, PID_3, CID_2 ));
	printf("There are now %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_2 ), CID_2);
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");


	patronsWithItemOut( itemsHead, patronsHead, CID_2 );

//	addItem( &itemsHead, 3, CID_4, "Kenny Z", "The Cook Book" );
	printf("ZZZZ -There are currently %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_4 ), CID_4 );
	discardCopiesOfItem( &itemsHead, 4, CID_4 );
	discardCopiesOfItem( &itemsHead, 2, CID_4 );
	printf("ZZZZ -There are currently %i copies available of %s\n", getCopiesAvailable( itemsHead, CID_4 ), CID_4 );
	printItemLinkedList( itemsHead );
	printf("The Item's list size is %i\n", getListSize( itemsHead ) );
	discardCopiesOfItem( &itemsHead, 1, CID_4 );
	printItemLinkedList( itemsHead );
	printf("The Item's list size is %i\n", getListSize( itemsHead ) );



	deleteAndFreeList( patronsHead, freePatronDataStruct );
	deleteAndFreeList( itemsHead, freeItemDataStruct );

	ListNode* psHead = NULL;
	ListNode* isHead = NULL;

	ProcessInput(&isHead, &psHead, "testdata.txt");

	printPatronLinkedList( psHead );

	deleteAndFreeList( psHead, freePatronDataStruct );
	deleteAndFreeList( isHead, freeItemDataStruct );


	puts("\n\n\nTHE ITEMS HERE WE COME\n\n\n");
	ListNode* psHead1 = NULL;
	ListNode* isHead1 = NULL;
	ProcessInput(&isHead1, &psHead1, "itemsdata.txt");

	printItemLinkedList( isHead1 );

	deleteAndFreeList( psHead1, freePatronDataStruct );
	deleteAndFreeList( isHead1, freeItemDataStruct );
	// ADD CHECKS FOR CAST FAILURES




}
