/*
* This file contains methods which each map to a legal command.
* The methods in general are specifing how the more generic
* functions from LinkedDataNodeOperations.h should interpret
* ListNode*'s void* data.
*
*
* @author Greg Mojonnier
*/

#include "ExecuteCommands.h"
#include "LinkedDataNodeOperations.h"
#include <string.h>
#include <allocate.h>
#include <stdio.h>
#include "AllConstants.h"

extern ListNode* g_PatronsHead;
extern ListNode* g_ItemsHead;

/*
* getCopiesAvailable
* ----------------------------------
*  
* Finds how many copies of an item with the specified CID
* are available to be checked out.
*
* @g_ItemsHead ---------------> List to find item in.
* @cid ---------------------> cid to match node from.
*
* @return ------------------> int which is the number of available copies.
*
*/
uint_least8_t getCopiesAvailable( const char* cid ){

	ListNode* itemNode = findNodeWithUID( g_ItemsHead, cid, 0 );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return 0;
	}	
	
	ItemData* item = (ItemData*)itemNode->data;
	uint_least8_t copiesAvailable = item->numCopies - getListSize( item->patronsCurrentlyRenting );

	printf("Item %s (%s/%s): %i of %i copies available\n", cid, item->author, item->title, copiesAvailable, item->numCopies );
	return copiesAvailable;
}

/*
* borrowItem
* ----------------------------------
*  
* Borrows the item specified by CID for the patron
* specified by PID. A node containing a pointer to one another is
* inserted into the CID & PID's sublists.(patronsCurrentlyRenting, itemsCurrentlyRenting)
*
*
* @g_ItemsHead ---------------> List of items.
* @g_PatronsHead -------------> List of patrons.
* @pid ---------------------> pid who will be borrowing an item.
* @cid ---------------------> cid of the item to borrow.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
void borrowItem( const char* pid, const char* cid ){
	ListNode* patronNode = findNodeWithUID( g_PatronsHead, pid, 1 );
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist\n", pid);
		return;
	}
	ListNode* itemNode = findNodeWithUID( g_ItemsHead, cid, 0 );
	
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return;
	}

	ItemData* item = (ItemData*)itemNode->data;
	
	if( getListSize( item->patronsCurrentlyRenting ) == item->numCopies ){
		fprintf( stderr, "No more copies of %s are available\n", cid );
		return;
	}

	PatronData* patron = (PatronData*) patronNode->data;
	if( getListSize( patron->itemsCurrentlyRenting ) == 5 ){
		fprintf( stderr, "%s cannot check out any more items\n", pid );
		return;
	}

	if( findNodeWithData( patron->itemsCurrentlyRenting, itemNode ) != NULL ){
		fprintf( stderr, "%s already has %s checked out\n", pid, cid );
		return;
	}

	insertNodeInOrder( &patron->itemsCurrentlyRenting, itemNode, newItemHasLowerPrecedence );
	insertNodeInOrder( &item->patronsCurrentlyRenting, patronNode, newPatronHasLowerPrecedence );
}

/*
* discardCopiesOfItem
* ----------------------------------
*  
* Discards a specified number of copies of an item specified by cid.
* Only allows you to discard non checked out items and only if there are
* enough available items to discard the full amount you specified.
* The node is deleted if copies of an item reaches 0.
*
*
* @g_ItemsHead ---------------> List of items.
* @numToDelete  ------------> Number of copies of the item to delete.
* @cid ---------------------> cid of the item to discard copies of.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
void discardCopiesOfItem( uint_least8_t numToDelete, const char* cid){

	ListNode* itemNode = findNodeWithUID( g_ItemsHead, cid, 0 );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return;
	}

	ItemData* item = (ItemData*)itemNode->data;

	if( ( item->numCopies - getListSize( item->patronsCurrentlyRenting ) ) < numToDelete ){
		fprintf( stderr, "Too few copies of %s are available", cid );
		return;
	}
	
	item->numCopies -= numToDelete;

	if( item->numCopies == 0 ){
		deleteNode( &g_ItemsHead, itemNode, freeItemDataStruct );
	}
}

/*
* addItem
* ----------------------------------
*  
* Allocates a new ItemData, sets all of its info from arguments,
* inserts node at head of list.
*
* @head --------------------> List to insert node into.
* @numCopies ---------------> number of copies to set into node.
* @cid ---------------------> cid to set into node.
* @author ------------------> author to set into node.
* @title -------------------> title to set into node.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
void addItem( uint_least8_t numCopies, const char* cid, const char* author, const char* title ){

	ListNode* existingItemNode;
	if( ( existingItemNode = findNodeWithUID( g_ItemsHead, cid, 0 ) ) != NULL ){
		ItemData* existingItem = (ItemData*)existingItemNode->data;
		fprintf( stderr, "Item %s (%s/%s) already associated with (%s/%s)\n", cid, author, title, existingItem->author, existingItem->title ); 
		return;
	}

	ItemData* i = (ItemData*) allocate( sizeof(ItemData) );

	if( i == NULL ){
		printf("Memory allocation failed!\n");
		return;
	}

	i->author = (char*) allocate( ( sizeof(char) * strlen(author) ) + 1 );

	if( i->author == NULL ){
		printf("Memory allocation failed!\n");
		return;
	}

	i->title = (char*) allocate( ( sizeof(char) * strlen(title) ) + 1 );

	if( i->title == NULL ){
		printf("Memory allocation failed!\n");
		return;
	}

	char cidBuffer[ CID_MIN_SIZE ];

	uint_least8_t periodLocation = strcspn( cid, PERIOD_WORD_SEPARATOR ); 
	
	strncpy( cidBuffer, cid, periodLocation );
	cidBuffer[ periodLocation ] = '\0';
	uint_least16_t cidNum = strtoul( cidBuffer, NULL, 10 );
	i->leftCID = cidNum;

	strcpy( cidBuffer, cid+periodLocation+1 );
	cidNum = strtoul( cidBuffer, NULL, 10 );
	i->rightCID = cidNum;

	i->numCopies = numCopies;
	i->patronsCurrentlyRenting = NULL;

	strcpy( i->author, author );
	strcpy( i->title, title );
	
	insertNodeInOrder( &g_ItemsHead, i, newItemHasLowerPrecedence );
}

/*
* patronsWithItemOut
* ----------------------------------
*  
* Determines all of the patrons who have the item
* specified by cid currently checked out.
*
*
* @g_ItemsHead ---------------> List of items.
* @cid ---------------------> cid who we want to know which patrons have out.
*
* @return ------------------> none.
*
*/
void patronsWithItemOut( const char* cid ){

	ListNode* itemNode = findNodeWithUID( g_ItemsHead, cid, 0 );
	
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );		
		return;
	}

	printItemStatus( (ItemData*) itemNode->data );
}

/*
* itemsOutByPatron
* ----------------------------------
*  
* Determines all of the items currently checked
* out by the patron specified by pid.
*
*
* @g_PatronsHead -------------> List of patrons.
* @pid ---------------------> pid who we want to know which items has checked out.
*
* @return ------------------> none.
*
*/
void itemsOutByPatron( const char* pid ){

	ListNode* patronNode = findNodeWithUID( g_PatronsHead, pid, 1 );
	
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist\n", pid );		
		return;
	}

	printPatronStatus( (PatronData*) patronNode->data );
}

/*
* returnPatronsItem
* ----------------------------------
*  
* Returns the item specified by CID for the patron
* specified by PID. The nodes containing pointers to one another are
* removed from the CID & PID's sublists.(patronsCurrentlyRenting, itemsCurrentlyRenting)
*
*
* @g_ItemsHead ---------------> List of items.
* @g_PatronsHead -------------> List of patrons.
* @pid ---------------------> pid who will be returning an item.
* @cid ---------------------> cid of the item to return.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
void returnPatronsItem( const char* pid, const char* cid ){

	ListNode* patronNode = findNodeWithUID( g_PatronsHead, pid, 1 );
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist\n", pid );
		return;
	}

	ListNode* itemNode = findNodeWithUID( g_ItemsHead, cid, 0 );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return;
	}

	PatronData* patron = (PatronData*)patronNode->data;
	ItemData* item = (ItemData*)itemNode->data;

	ListNode* itemPtrToDelete = findNodeWithData( patron->itemsCurrentlyRenting, itemNode );

	if( itemPtrToDelete == NULL ){
		fprintf( stderr, "%s does not have %s checked out", pid, cid );
		return;
	}
	else{
		deleteNode( &patron->itemsCurrentlyRenting, itemPtrToDelete, NULL );
		deleteNode( &item->patronsCurrentlyRenting, findNodeWithData( item->patronsCurrentlyRenting, patronNode ), NULL );
	}
}

/*
* addPatron
* ----------------------------------
*  
* Allocates a new PatronData, sets all of its info from arguments,
* inserts node at head of list.
*
* @head --------------------> List to insert node into.
* @pid ---------------------> pid to set into node.
* @name --------------------> name to set into node.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
void addPatron( const char* pid, const char* name ){

	ListNode* existingPatron;
	if( ( existingPatron = findNodeWithUID( g_PatronsHead, pid, 1 ) ) != NULL ){
		fprintf( stderr, "Patron %s (%s) already associated with (%s)\n", pid, name, ((PatronData*)existingPatron->data)->name );
		return;
	}

	PatronData* p = (PatronData*) allocate( sizeof(PatronData) );

	if( p == NULL ){
		printf("Memory allocation failed!\n");
		return;
	}

	p->name = (char*) allocate( ( sizeof(char) * strlen(name) ) + 1 );

	if( p->name == NULL ){
		printf("Memory allocation failed!\n");
		return;
	}

	strcpy( p->name, name );
	p->leftPID[ 0 ] = *pid;
	p->leftPID[ 1 ] = '\0';

	p->rightPID = strtoul( pid+1, NULL, 10 );

	p->itemsCurrentlyRenting = NULL;

	insertNodeInOrder( &g_PatronsHead, p, newPatronHasLowerPrecedence );
}


/*
* printAllPatronsStatus
* ----------------------------------
*  
* Loops through entire Patron linked list and calls
* printPatronStatus() on the ItemData.
*
* @g_PatronsHead ---------------> Patron list to print out.
*
* @return ------------------> None.
*
*/
void printAllListsStatus(){

	ListNode* listToPrint = g_ItemsHead;
	while( listToPrint != NULL ){
		printItemStatus( (ItemData*) listToPrint->data );
		listToPrint = listToPrint->next;
		printf("\n");
	}

	listToPrint = g_PatronsHead;
	while( listToPrint != NULL ){
		printPatronStatus( (PatronData*) listToPrint->data );
		listToPrint = listToPrint->next;
		if( listToPrint != NULL ){
			printf("\n");
		}
	}
}

/*
* printItemStatus
* ----------------------------------
*  
* Prints status of an individual item.
*
* @items -------------------> Item to print status of.
*
* @return ------------------> None.
*
*/
void printItemStatus( ItemData* item ){
	if( item == NULL ){
		return;
	}

	ListNode* patronsCurrentlyRenting = item->patronsCurrentlyRenting;

	if( patronsCurrentlyRenting == NULL ){
		printf( "Item %d.%d (%s/%s) is not checked out\n", item->leftCID, item->rightCID, item->author, item->title ); 
	}
	else{
		printf( "Item %d.%d (%s/%s) is checked out to:\n", item->leftCID, item->rightCID, item->author, item->title );

		while( patronsCurrentlyRenting != NULL ){
			PatronData* p = (PatronData*) ((ListNode*)patronsCurrentlyRenting->data)->data;
			if( p != NULL ){
				char fullPID[ PID_MAX_SIZE ];
				sprintf( fullPID, "%s%04i", p->leftPID, p->rightPID );
				printf( "   %s (%s)\n", fullPID, p->name );
			}
			patronsCurrentlyRenting = patronsCurrentlyRenting->next;
		}
	}
}


/*
* printPatronStatus
* ----------------------------------
*  
* Prints status of an individual patron.
*
* @patron ------------------> patron to print status of.
*
* @return ------------------> None.
*
*/
void printPatronStatus( PatronData* patron ){

	ListNode* itemsCurrentlyRenting = patron->itemsCurrentlyRenting;

	if( itemsCurrentlyRenting == NULL ){
		printf( "Patron %s%04i (%s) has no items checked out\n", patron->leftPID, patron->rightPID, patron->name ); 
	}
	else{
		printf( "Patron %s%04i (%s) has these items checked out:\n", patron->leftPID, patron->rightPID, patron->name );

		while( itemsCurrentlyRenting != NULL ){
			ItemData* i = (ItemData*)((ListNode*)itemsCurrentlyRenting->data)->data;

			if( i != NULL ){
				printf( "   %d.%d (%s/%s)\n", i->leftCID, i->rightCID, i->author, i->title );
			}
			itemsCurrentlyRenting = itemsCurrentlyRenting->next;
		}
	}
}
