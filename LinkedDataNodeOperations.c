/*
* This file contains methods that perform operations
* on linked lists of ListNodes. The ListNodes contain
* void* data which are either ItemData* or PatronData*.
*
*
* @author Greg Mojonnier
*/

#include "LinkedDataNodeOperations.h"
#include <allocate.h>
#include <stdio.h>
#include <string.h>

/*
* insertNodeAtHead
* ----------------------------------
*  
* Allocates a new ListNode, sets new nodes dats to data argument,
* inserts node at head of list argument.
*
* @currentHead -------------> List to insert node into.
* @data --------------------> Data thats put into node thats inserted into list.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool insertNodeAtHead( ListNode** currentHead, void* data ){

	// The address of the outside variable is NULL???
	// error!
	if( currentHead == NULL ){
		return 0;
	}

	// special error message for if allocation fail?
	ListNode* newNode = (ListNode*) allocate( sizeof( ListNode ) ); 
	if( newNode == NULL ){
		return 0;
	}
	newNode->data = data;

	// if where the outside pointer
	// would go if it were dereferenced is null
	( *currentHead == NULL ) ? ( newNode->next = NULL ) : (newNode->next = *currentHead);

	*currentHead = newNode; 
	return 1;
}

/*
* deleteNode
* ----------------------------------
*  
* Frees the existing specified ListNode, calling freeVoidDataFunction 
* which properly unallocates its void* data.
*
* @currentHead -------------> List to delete node from.
* @nodeToDelete ------------> Node that we want to delete.
* @freeVoidDataFunction ----> Function pointer which determines how to clean up node's void* data.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool deleteNode( ListNode** currentHead, ListNode* nodeToDelete, _Bool(*freeVoidDataFunction)(void* data) ){
	// The address of the outside variable is NULL???
	// or its an empty list or nodeToDelete is null
	// return false
	if( currentHead == NULL || *currentHead == NULL || nodeToDelete == NULL ){
		return 0;
	}
	// special case if head is node to delete
	if( *currentHead == nodeToDelete ){
 
		// take out of list
		// if only element in list next is null
		// when outside ptr is dereferenced goes to ptr to next
		*currentHead = nodeToDelete->next;

		// delete everything related to nodeToDelete

		// DELETE EVERYTHING RELATED TO NEXTNODE
		// if null then void* data is just a pointer
		// so no special freeing is needed
		if( freeVoidDataFunction != NULL ){
			(*freeVoidDataFunction)(nodeToDelete->data);
		}
		unallocate( nodeToDelete );


		return 1;
	}

	ListNode* nodeToCheck = *currentHead;

	while( nodeToCheck != NULL ){
		ListNode* nextNode = nodeToCheck->next;

		if( nextNode == nodeToDelete ){
			
			// take nextNode out of list
			nodeToCheck->next = nextNode->next;


			// DELETE EVERYTHING RELATED TO NEXTNODE
			// if null then void* data is just a pointer
			// so no special freeing is needed
			if( freeVoidDataFunction != NULL ){
				(*freeVoidDataFunction)(nextNode->data);
			}
			unallocate( nextNode );


			return 1;
		}
		nodeToCheck = nextNode;
	}
	return 0;
}

/*
* deleteAndFreeList
* ----------------------------------
*  
* Frees the entire list, calling freeVoidDataFunction 
* for each node which properly unallocates its void* data.
*
* @currentHead -------------> List to completely delete.
* @freeVoidDataFunction ----> Function pointer which determines how to clean up node's void* data.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool deleteAndFreeList( ListNode* currentHead, _Bool(*freeVoidDataFunction)(void* data) ){

	if( currentHead == NULL ){
	// empty list
		return 1;
	}	

	ListNode* nodeToDelete = currentHead;
	while( nodeToDelete != NULL ){
		ListNode* next = nodeToDelete->next;
		// check result of this?

		// if null then void* data is just a pointer
		// so no special freeing is needed
		if( freeVoidDataFunction != NULL ){
			(*freeVoidDataFunction)(nodeToDelete->data);
		}
		unallocate( nodeToDelete );
		nodeToDelete = next;
	}

	currentHead = NULL;	
	return 1;
}


/*
* freeItemDataStruct
* ----------------------------------
*  
* Unallocates a void* data interpreting it as an ItemData*.
*
* @item --------------------> Void* to interpret as ItemData* and unallocate.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool freeItemDataStruct( void* item ){
	ItemData* i = (ItemData*)item;
	if( i == NULL ){
		return 0;
	}
	unallocate( i->author );
	unallocate( i->title );
	unallocate( i->cid );
	// numCopies gets taken care of when full struct is unallocated

	ListNode* nodeToDelete = i->patronsCurrentlyRenting;
	while( nodeToDelete != NULL ){
		ListNode* next = nodeToDelete->next;


		// Get data, its a pointer to a ListNode containing an patron
		ListNode* patronToCollectItemFromNode = (ListNode*)nodeToDelete->data;
		PatronData* patronToCollectFrom = (PatronData*)patronToCollectItemFromNode->data;
		// find listnode with data address of p

		ListNode* pcr = patronToCollectFrom->itemsCurrentlyRenting;

		deleteNode( &pcr, findNodeWithData( pcr, i ), NULL );


		unallocate( nodeToDelete );

		nodeToDelete = next;
	}
	unallocate( i );
	return 1;
}

/*
* freePatronDataStruct
* ----------------------------------
*  
* Unallocates a void* data interpreting it as an PatronData*.
*
* @patron ------------------> Void* to interpret as PatronData* and unallocate.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool freePatronDataStruct( void* patron ){
	PatronData* p = (PatronData*)patron; 
	if( p == NULL ){
		return 0;
	}
	unallocate( p->name );
	// pid gets taken care of when full struct is unallocated
	// unallocate the actual nodes of items currently renting
	// dont worry about the void* data in them because those point to
	// other item nodes that will take care of themselves
	
	ListNode* nodeToDelete = p->itemsCurrentlyRenting;
	while( nodeToDelete != NULL ){
		ListNode* next = nodeToDelete->next;
	
		// this is a node
		// it has a void* data which contains an address of a listnode
		// whom contains a void* data to an item
	
		// go to this other listnode and cast its void* data to an item
		// within this item loop through its patronsCurrentlyrenting LL
		// find the node containg a void* data with the patrons address
		// delete this node


		// Get data, its a pointer to a ListNode containing an item
		ListNode* itemToReturnNode = (ListNode*)nodeToDelete->data;
		ItemData* itemToReturn = (ItemData*)itemToReturnNode->data;
		// find listnode with data address of p

		ListNode* pcr = itemToReturn->patronsCurrentlyRenting;

		deleteNode( &pcr, findNodeWithData( pcr, p ), NULL );



		unallocate( nodeToDelete );
		nodeToDelete = next;
	}
	unallocate( p );
	return 1;
}

/*
* findNodeWithUID
* ----------------------------------
*  
* Finds a node with the matching UID. The UID is
* matched to nodes void* data with doesDataMatchUID.
*
* @currentHead -------------> List to find node from.
* @uid ---------------------> UID to look node up with.
* @doesDataMatchUID --------> Function pointer which determines how to match a UID to node's void* data.

* @return ------------------> Pointer to node matching UID, or NULL.
*
*/
ListNode* findNodeWithUID( ListNode* currentHead, const char* uid, _Bool(*doesDataMatchUID)(const char* uid, void* data) ){
	if( currentHead == NULL || uid == NULL ){
		return NULL;
	}
	
	ListNode* nodeToCheck = currentHead;

	while( nodeToCheck != NULL ){

		if( (*doesDataMatchUID)( uid, nodeToCheck->data ) ){
			break;
		}
		nodeToCheck = nodeToCheck->next;
	}
	return nodeToCheck;
}

/*
* doesPatronMatchUID
* ----------------------------------
*  
* Matches a void* data to a UID by interpreting
*  it as a PatronData* and looking at its PID.
*
* @uid ---------------------> UID to match.
* @data --------------------> Void* to interpret as PatronData*.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool doesPatronMatchUID( const char* uid, void* data ){
	PatronData* p = (PatronData*)data; 
	if( uid == NULL || p == NULL ){
		return 0;
	}

	return ( strcmp( p->pid, uid ) == 0 );
}

/*
* doesItemMatchUID
* ----------------------------------
*  
* Matches a void* data to a UID by interpreting
*  it as a ItemData* and looking at its CID.
*
* @uid ---------------------> UID to match.
* @data --------------------> Void* to interpret as ItemData*.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool doesItemMatchUID( const char* uid, void* data ){
	ItemData* i = (ItemData*)data; 
	if( uid == NULL || i == NULL ){
		return 0;
	}
	return ( strcmp( i->cid, uid ) == 0 );
}


/*
* findNodeWithData
* ----------------------------------
*  
* Finds a node with its void* data the exact same
* as the supplied void* data argument.
*
* @currentHead -------------> List to find node from.
* @data --------------------> Data to match node with.

* @return ------------------> Pointer to node matching data, or NULL.
*
*/
ListNode* findNodeWithData( ListNode* currentHead, void* data ){

	if( currentHead == NULL ){
		return NULL;
	}
	
	ListNode* nodeToCheck = currentHead;

	while( nodeToCheck != NULL ){

		if( nodeToCheck->data == data ){
			break;
		}
		nodeToCheck = nodeToCheck->next;
	}
	return nodeToCheck;
}

/*
* getListSize
* ----------------------------------
*  
* Finds the length of the given list.
*
* @currentHead -------------> List to determine size of.

* @return ------------------> int indicating a list's size.
*
*/
int getListSize( ListNode* head ){

	ListNode* currentNode = head;
	int size = 0;

	while( currentNode != NULL ){
		++size;
		currentNode = currentNode->next;
	}
	return size;
}

/*
* printItemLinkedList
* ----------------------------------
*  
* Prints a linked list interpreting them as Items.
*
* @currentHead -------------> List to print.

* @return ------------------> none.
*
*/
void printItemLinkedList( ListNode* headNode ){

	puts("--------------------------");
	ListNode* itemToPrint = headNode;

	while( itemToPrint != NULL ){
		void* _itemData = itemToPrint->data;
		ItemData* itemData = (ItemData*) _itemData;
		printf("Item: %s\n", itemData->title);
		itemToPrint = itemToPrint->next;
	}
	puts("--------------------------");
}


/*
* printPatronLinkedList
* ----------------------------------
*  
* Prints a linked list interpreting them as Patrons.
*
* @currentHead -------------> List to print.

* @return ------------------> none.
*
*/
void printPatronLinkedList( ListNode* headNode ){

	puts("--------------------------");
	ListNode* patronToPrint = headNode;

	while( patronToPrint != NULL ){
		void* _patronData = patronToPrint->data;
		PatronData* patronData = (PatronData*) _patronData;
		printf("Patron: %s\n", patronData->name);
		patronToPrint = patronToPrint->next;
	}
	puts("--------------------------");
}
