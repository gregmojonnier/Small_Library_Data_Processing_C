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


/*
* getCopiesAvailable
* ----------------------------------
*  
* Finds how many copies of an item with the specified CID
* are available to be checked out.
*
* @itemsHead ---------------> List to find item in.
* @cid ---------------------> cid to match node from.
*
* @return ------------------> int which is the number of available copies.
*
*/
unsigned char getCopiesAvailable( ListNode* itemsHead, const char* cid ){
	
	if( itemsHead == NULL ){
		return 0;
	}

	ListNode* itemNode = findNodeWithUID( itemsHead, cid, doesItemMatchUID );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return 0;
	}	
	
	ItemData* item = (ItemData*)itemNode->data;
	unsigned char copiesAvailable = item->numCopies - getListSize( item->patronsCurrentlyRenting );

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
* @itemsHead ---------------> List of items.
* @patronsHead -------------> List of patrons.
* @pid ---------------------> pid who will be borrowing an item.
* @cid ---------------------> cid of the item to borrow.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool borrowItem( ListNode** itemsHead, ListNode** patronsHead, const char* pid, const char* cid ){
	ListNode* patronNode = findNodeWithUID( *patronsHead, pid, doesPatronMatchUID );
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist\n", pid);
		return 0;
	}
	ListNode* itemNode = findNodeWithUID( *itemsHead, cid, doesItemMatchUID );
	
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return 0;
	}

	ItemData* item = (ItemData*)itemNode->data;
	
	if( getListSize( item->patronsCurrentlyRenting ) == item->numCopies ){
		fprintf( stderr, "No more copies of %s are available\n", cid );
		return 0;
	}

	PatronData* patron = (PatronData*) patronNode->data;
	if( getListSize( patron->itemsCurrentlyRenting ) == 5 ){
		fprintf( stderr, "%s cannot check out any more items\n", pid );
		return 0;
	}

	if( findNodeWithData( patron->itemsCurrentlyRenting, itemNode ) != NULL ){
		fprintf( stderr, "%s already has %s checked out\n", pid, cid );
		return 0;
	}

	if( insertNodeInOrder( &patron->itemsCurrentlyRenting, itemNode, newItemHasLowerPrecedence ) && insertNodeInOrder( &item->patronsCurrentlyRenting, patronNode, newPatronHasLowerPrecedence ) ){
		return 1;
	}
	return 0;
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
* @itemsHead ---------------> List of items.
* @numToDelete  ------------> Number of copies of the item to delete.
* @cid ---------------------> cid of the item to discard copies of.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool discardCopiesOfItem( ListNode** itemsHead, unsigned char numToDelete, const char* cid){

	ListNode* itemNode = findNodeWithUID( *itemsHead, cid, doesItemMatchUID );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return 0;
	}

	ItemData* item = (ItemData*)itemNode->data;

	if( ( item->numCopies - getListSize( item->patronsCurrentlyRenting ) ) < numToDelete ){
		fprintf( stderr, "Too few copies of %s are available", cid );
		return 0;
	}
	
	item->numCopies -= numToDelete;

	if( item->numCopies == 0 ){
		deleteNode( itemsHead, itemNode, freeItemDataStruct );
	}
	return 1;
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
_Bool addItem( ListNode** head, unsigned char numCopies, const char* cid, const char* author, const char* title ){
	if( head == NULL ){
		return 0;
	}

	ListNode* existingItemNode;
	if( ( existingItemNode = findNodeWithUID( *head, cid, doesItemMatchUID ) ) != NULL ){
		ItemData* existingItem = (ItemData*)existingItemNode->data;
		fprintf( stderr, "Item %s (%s/%s) already associated with (%s/%s)\n", cid, author, title, existingItem->author, existingItem->title ); 
		return 0;
	}

	ItemData* i = (ItemData*) allocate( sizeof(ItemData) );

	if( i == NULL ){
		printf("Memory allocation failed!\n");
		return 0;
	}

	i->author = (char*) allocate( ( sizeof(char) * strlen(author) ) + 1 );

	if( i->author == NULL ){
		printf("Memory allocation failed!\n");
		return 0;
	}

	i->title = (char*) allocate( ( sizeof(char) * strlen(title) ) + 1 );

	if( i->title == NULL ){
		printf("Memory allocation failed!\n");
		return 0;
	}

	char leftCIDBuffer[ CID_MIN_SIZE ];
	char rightCIDBuffer[ CID_MIN_SIZE ];

	unsigned char periodLocation = strcspn( cid, PERIOD_WORD_SEPARATOR ); 
	
	strncpy( leftCIDBuffer, cid, periodLocation );
	leftCIDBuffer[ periodLocation ] = '\0';

	strcpy( rightCIDBuffer, cid+periodLocation+1 );

	unsigned short int leftCID = strtoul( leftCIDBuffer, NULL, 10 );

	unsigned short int rightCID = strtoul( rightCIDBuffer, NULL, 10 );

	i->numCopies = numCopies;
	i->patronsCurrentlyRenting = NULL;
	i->leftCID = leftCID;
	i->rightCID = rightCID;

	strcpy( i->author, author );
	strcpy( i->title, title );
	
	if( !insertNodeInOrder( head, (void*)i, newItemHasLowerPrecedence ) ){
		return 0;
	}
	return 1;
}

/*
* patronsWithItemOut
* ----------------------------------
*  
* Determines all of the patrons who have the item
* specified by cid currently checked out.
*
*
* @itemsHead ---------------> List of items.
* @cid ---------------------> cid who we want to know which patrons have out.
*
* @return ------------------> none.
*
*/
void patronsWithItemOut( ListNode* itemsHead, const char* cid ){

	ListNode* itemNode = findNodeWithUID( itemsHead, cid, doesItemMatchUID );
	
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );		
		return ;
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
* @patronsHead -------------> List of patrons.
* @pid ---------------------> pid who we want to know which items has checked out.
*
* @return ------------------> none.
*
*/
void itemsOutByPatron( ListNode* patronsHead, const char* pid ){

	ListNode* patronNode = findNodeWithUID( patronsHead, pid, doesPatronMatchUID );
	
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist\n", pid );		
		return;
	}

	PatronData* patron = (PatronData*) patronNode->data;


	printPatronStatus( patron );

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
* @itemsHead ---------------> List of items.
* @patronsHead -------------> List of patrons.
* @pid ---------------------> pid who will be returning an item.
* @cid ---------------------> cid of the item to return.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool returnPatronsItem( ListNode* itemsHead, ListNode* patronsHead, const char* pid, const char* cid ){

	ListNode* patronNode = findNodeWithUID( patronsHead, pid, doesPatronMatchUID );
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist\n", pid );
		return 0;
	}

	ListNode* itemNode = findNodeWithUID( itemsHead, cid, doesItemMatchUID );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return 0;
	}

	PatronData* patron = (PatronData*)patronNode->data;
	ItemData* item = (ItemData*)itemNode->data;

	ListNode* itemPtrToDelete = findNodeWithData( patron->itemsCurrentlyRenting, itemNode );

	if( itemPtrToDelete == NULL ){
		fprintf( stderr, "%s does not have %s checked out", pid, cid );
		return 0;
	}
	else{
		deleteNode( &patron->itemsCurrentlyRenting, itemPtrToDelete, NULL );
		deleteNode( &item->patronsCurrentlyRenting, findNodeWithData( item->patronsCurrentlyRenting, patronNode ), NULL );
	}
	return 1;
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
_Bool addPatron( ListNode** head, const char* pid, const char* name ){
	if( head == NULL ){
		return 0;
	}

	ListNode* existingPatron;
	if( ( existingPatron = findNodeWithUID( *head, pid, doesPatronMatchUID ) ) != NULL ){
		fprintf( stderr, "Patron %s (%s) already associated with (%s)\n", pid, name, ((PatronData*)existingPatron->data)->name );
		return 0;
	}

	PatronData* p = (PatronData*) allocate( sizeof(PatronData) );

	if( p == NULL ){
		printf("Memory allocation failed!\n");
		return 0;
	}

	p->name = (char*) allocate( ( sizeof(char) * strlen(name) ) + 1 );

	if( p->name == NULL ){
		printf("Memory allocation failed!\n");
		return 0;
	}

	strcpy( p->name, name );
	strncpy( p->leftPID, pid, 1 );
	p->leftPID[ 1 ] = '\0';

	unsigned short int rightPID = strtoul( pid+1, NULL, 10 );

	p->rightPID = rightPID;
	p->itemsCurrentlyRenting = NULL;

	if( !insertNodeInOrder( head, (void*)p, newPatronHasLowerPrecedence ) ){
		return 0;
	}
	return 1;
}

/*
* printAllItemsStatus
* ----------------------------------
*  
* Loops through entire Item linked list and calls
* printItemStatus() on the ItemData.
*
* @itemsHead ---------------> Item list to print out.
*
* @return ------------------> None.
*
*/
void printAllItemsStatus( ListNode* itemToPrint ){

	while( itemToPrint != NULL ){
		ItemData* itemData = (ItemData*)itemToPrint->data; 
		printItemStatus( itemData );
		itemToPrint = itemToPrint->next;
		printf("\n");
	}
}

/*
* printAllPatronsStatus
* ----------------------------------
*  
* Loops through entire Patron linked list and calls
* printPatronStatus() on the ItemData.
*
* @patronsHead ---------------> Patron list to print out.
*
* @return ------------------> None.
*
*/
void printAllPatronsStatus( ListNode* patronToPrint ){

	while( patronToPrint != NULL ){
		PatronData* patronData = (PatronData*) patronToPrint->data;
		printPatronStatus( patronData );
		patronToPrint = patronToPrint->next;
		if( patronToPrint != NULL ){
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

	char fullCID[ CID_MAX_SIZE ];
	sprintf( fullCID, "%d.%d", item->leftCID, item->rightCID );

	if( patronsCurrentlyRenting == NULL ){
		printf( "Item %s (%s/%s) is not checked out\n", fullCID, item->author, item->title ); 
	}
	else{
		printf( "Item %s (%s/%s) is checked out to:\n", fullCID, item->author, item->title );

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

	char fullPID[ PID_MAX_SIZE ];
	sprintf( fullPID, "%s%04i", patron->leftPID, patron->rightPID );

	if( itemsCurrentlyRenting == NULL ){
		printf( "Patron %s (%s) has no items checked out\n", fullPID, patron->name ); 
	}
	else{
		printf( "Patron %s (%s) has these items checked out:\n", fullPID, patron->name );

		while( itemsCurrentlyRenting != NULL ){
			ItemData* i = (ItemData*)((ListNode*)itemsCurrentlyRenting->data)->data;

			if( i != NULL ){
				char fullCID[ CID_MAX_SIZE ];
				sprintf( fullCID, "%d.%d", i->leftCID, i->rightCID );

				printf( "   %s (%s/%s)\n", fullCID, i->author, i->title );
			}
			itemsCurrentlyRenting = itemsCurrentlyRenting->next;
		}
	}
}
