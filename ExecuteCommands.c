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


/*
* getCopiesAvailable
* ----------------------------------
*  
* Finds how many copies of an item with the specified CID
* are available to be checked out.
*
* @head --------------------> List to find item in.
* @cid ---------------------> cid to match node from.
*
* @return ------------------> int which is the number of available copies.
*
*/
int getCopiesAvailable( ListNode* head, const char* cid ){
	
	if( head == NULL ){
		return 0;
	}

	ListNode* itemNode = findNodeWithUID( head, cid, doesItemMatchUID );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist\n", cid );
		return 0;
	}	
	
	ItemData* item = (ItemData*)itemNode->data;
	ListNode* patronsRenting = item->patronsCurrentlyRenting;
	int copiesCheckedOut = getListSize( patronsRenting );

	printf("Item %s (%s/%s) %i of %i copies available\n", cid, item->author, item->title, copiesCheckedOut, item->numCopies );
	return (item->numCopies - copiesCheckedOut);
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

	if( insertNodeAtHead( &patron->itemsCurrentlyRenting, itemNode ) && insertNodeAtHead( &item->patronsCurrentlyRenting, patronNode ) ){
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
_Bool discardCopiesOfItem( ListNode** itemsHead, short int numToDelete, const char* cid){

	ListNode* itemNode = findNodeWithUID( *itemsHead, cid, doesItemMatchUID );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist", cid );
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
_Bool addItem( ListNode** head, int numCopies, const char* cid, const char* author, const char* title ){
	if( head == NULL ){
		return 0;
	}

	ListNode* existingItemNode;
	if( ( existingItemNode = findNodeWithUID( *head, cid, doesItemMatchUID ) ) != NULL ){
		ItemData* existingItem = (ItemData*)existingItemNode;
		fprintf( stderr, "Item %s (%s/%s) already associated with (%s/%s)\n", cid, author, title, existingItem->author, existingItem->title ); 
		return 0;
	}

	ItemData* i = (ItemData*) allocate( sizeof(ItemData) );
	i->cid = (char*) allocate( ( sizeof(char) * strlen(cid) ) + 1 );
	i->author = (char*) allocate( ( sizeof(char) * strlen(author) ) + 1 );
	i->title = (char*) allocate( ( sizeof(char) * strlen(title) ) + 1 );
	i->numCopies = numCopies;
	i->patronsCurrentlyRenting = NULL;

	strcpy( i->cid, cid );
	strcpy( i->author, author );
	strcpy( i->title, title );
	
	if( !insertNodeAtHead( head, (void*)i ) ){
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
* @patronsHead -------------> List of patrons.
* @cid ---------------------> cid who we want to know which patrons have out.
*
* @return ------------------> none.
*
*/
void patronsWithItemOut( ListNode* itemsHead, ListNode* patronsHead, const char* cid ){

	ListNode* itemNode = findNodeWithUID( itemsHead, cid, doesItemMatchUID );
	
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist", cid );		
		return ;
	}

	ItemData* item = (ItemData*) itemNode->data;
	ListNode* patronsCurrentlyRenting = item->patronsCurrentlyRenting;

	if( patronsCurrentlyRenting == NULL ){
		printf( "Item %s (%s/%s) is not checked out\n", cid, item->author, item->title ); 
	}
	else{
		printf( "Item %s (%s/%s) is checked out to:\n", cid, item->author, item->title );

		while( patronsCurrentlyRenting != NULL ){
			ListNode* patronNode = (ListNode*)patronsCurrentlyRenting->data;
			PatronData* p = (PatronData*) patronNode->data;
			printf( "   %s (%s)\n", p->pid, p->name );
			patronsCurrentlyRenting = patronsCurrentlyRenting->next;
		}
	}
}

/*
* itemsOutByPatron
* ----------------------------------
*  
* Determines all of the items currently checked
* out by the patron specified by pid.
*
*
* @itemsHead ---------------> List of items.
* @patronsHead -------------> List of patrons.
* @pid ---------------------> pid who we want to know which items has checked out.
*
* @return ------------------> none.
*
*/
void itemsOutByPatron( ListNode* itemsHead, ListNode* patronsHead, const char* pid ){

	ListNode* patronNode = findNodeWithUID( patronsHead, pid, doesPatronMatchUID );
	
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist", pid );		
		return;
	}

	PatronData* patron = (PatronData*) patronNode->data;
	ListNode* itemsCurrentlyRenting = patron->itemsCurrentlyRenting;

	if( itemsCurrentlyRenting == NULL ){
		printf( "Patron %s (%s) has no items checked out\n", pid, patron->name ); 
	}
	else{
		printf( "Patron %s (%s) has these items checked out:\n", pid, patron->name );

		while( itemsCurrentlyRenting != NULL ){
			ListNode* itemNode = (ListNode*)itemsCurrentlyRenting->data;
			ItemData* i = (ItemData*) itemNode->data;
			printf( "   %s (%s/%s)\n", i->cid, i->author, i->title );
			itemsCurrentlyRenting = itemsCurrentlyRenting->next;
		}
	}

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
		fprintf( stderr, "%s does not exist", pid );
		return 0;
	}

	ListNode* itemNode = findNodeWithUID( itemsHead, cid, doesItemMatchUID );
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist", cid );
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
		fprintf( stderr, "Patron %s %s already associated with %s\n", pid, name, ((PatronData*)existingPatron->data)->name );
		return 0;
	}

	PatronData* p = (PatronData*) allocate( sizeof(PatronData) );
	p->name = (char*) allocate( ( sizeof(char) * strlen(name) ) + 1 );
	strcpy( p->name, name );
	strcpy( p->pid, pid );
	p->itemsCurrentlyRenting = NULL;

	if( !insertNodeAtHead( head, (void*)p ) ){
		return 0;
	}
	return 1;
}
