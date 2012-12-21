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
#include "AllConstants.h"

extern ListNode* g_ItemsHead;
extern ListNode* g_PatronsHead;
/*
* insertNodeInOrder
* ----------------------------------
*  
* Allocates a new ListNode, sets new nodes dats to data argument,
* inserts node into the list in order. Order is determined by the
* function passed in newDataHasLowerPrecedence. Data with lower precedence
* goes lower in the list. Items and Patrons have different criteria for ordering.
*
* @currentHead ---------------> List to insert node into.
* @data ----------------------> Data thats put into node thats inserted into list.
* @newDataHasLowerPrecedence -> Function pointer to determine data precedence.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool insertNodeInOrder( ListNode** currentHead, void* data, _Bool(*newDataHasLowerPrecedence)(void* _newData, void* _currentData) ){

	// The address of the outside variable is NULL???
	// error!
	if( currentHead == NULL ){
		return 0;
	}

	// special error message for if allocation fail?
	ListNode* newNode = (ListNode*) allocate( sizeof( ListNode ) ); 
	if( newNode == NULL ){
		printf("Memory allocation failed!\n");
		return 0;
	}
	newNode->data = data;


	// empty list
	if( *currentHead == NULL ){
		newNode->next = NULL;
		*currentHead = newNode; 
	}
	else{
		ListNode* nodeToCheck = *currentHead;
	

		if( !newDataHasLowerPrecedence( data, nodeToCheck->data ) ){

			newNode->next = *currentHead;
			*currentHead = newNode; 
			return 1;
		}

		while( nodeToCheck != NULL ){
			ListNode* nextNodeToCheck = nodeToCheck->next;

			if( nextNodeToCheck != NULL ){
				if( newDataHasLowerPrecedence( data, nextNodeToCheck->data ) ){
					nodeToCheck = nextNodeToCheck;
					continue;
				}
				else{
					nodeToCheck->next = newNode;
					newNode->next = nextNodeToCheck;
					break;
				}
			}
			else{
				nodeToCheck->next = newNode;
				newNode->next = NULL;
				break;
			}
		}
	}

	return 1;
}

/*
* newPatronHasLowerPrecedence
* ----------------------------------
*  
* Determines if _newPatron has lower precedence
* than _currentPatron. First compares their name
* if that is equal than their pid is used.
*
* @_newPatron -------------> New patron to check for lower precedence with.
* @_currentPatron ---------> Current patron to check against.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool newPatronHasLowerPrecedence( void* _newPatron, void* _currentPatron ){
	PatronData* newPatron = (PatronData*)_newPatron;
	PatronData* currentPatron = (PatronData*)_currentPatron;

	if( newPatron != NULL && currentPatron != NULL){
		char namePrecedence = strcmp( newPatron->name, currentPatron->name );


		if( namePrecedence == 0 ){
			// do by PID

			if( newPatron->leftPID[0] > currentPatron->leftPID[0] || ( newPatron->leftPID[0] == currentPatron->leftPID[0] && newPatron->rightPID > currentPatron->rightPID ) ){
				return 1;
			}
			else{
				return 0;
			}
		}
		else if( namePrecedence > 0 ){
			return 1;
		}
		else{
			//namePrecedence < 0
			return 0;
		}
	}
	else{
		return 0;
	}
}

/*
* newItemHasLowerPrecedence
* ----------------------------------
*  
* Determines if _newItem has lower precedence than _currentItem.
* First compares author, if they are the same it than tries
* title. If titles are the same it uses CID.
*
* @_newItem ----------------> New item to check for lower precedence with.
* @_currentItem ------------> Current item to check against.
*
* @return ------------------> _Bool indicating success or failure.
*
*/
_Bool newItemHasLowerPrecedence( void* _newItem, void* _currentItem ){
	ItemData* newItem = (ItemData*)_newItem;
	ItemData* currentItem = (ItemData*)_currentItem;

	if( newItem != NULL && currentItem != NULL ){
		char authorPrecedence = strcmp( newItem->author, currentItem->author );

		if( authorPrecedence == 0 ){
			// item title
			char titlePrecedence = strcmp( newItem->title, currentItem->title );
			
			if( titlePrecedence == 0 ){
				// cid
				if( ( newItem->leftCID > currentItem->leftCID || newItem->leftCID == currentItem->leftCID ) 
										&& newItem->rightCID > currentItem->rightCID ){
					return 1;
				}
				else{
					return 0;
				}
			}
			else if( titlePrecedence > 0 ){
				return 1;
			}
			else{
				return 0;
			}
		}
		else if( authorPrecedence > 0 ){
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
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
* deleteAndFreeBothLists
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
void deleteAndFreeBothLists(){

	while( deleteNode( &g_PatronsHead, g_PatronsHead, freePatronDataStruct ) );
	while( deleteNode( &g_ItemsHead, g_ItemsHead, freeItemDataStruct ) );

	g_PatronsHead = NULL;	
	g_ItemsHead = NULL;	
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
	// numCopies gets taken care of when full struct is unallocated

	ListNode* nodeToDelete = i->patronsCurrentlyRenting;
	while( nodeToDelete != NULL ){
		ListNode* next = nodeToDelete->next;


		// Get data, its a pointer to a ListNode containing an patron
		PatronData* patronToCollectFrom = (PatronData*)((ListNode*) nodeToDelete->data)->data;
		// find listnode with data address of p

		deleteNode( &patronToCollectFrom->itemsCurrentlyRenting, findNodeWithData( patronToCollectFrom->itemsCurrentlyRenting, i ), NULL );


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
		ItemData* itemToReturn = (ItemData*)((ListNode*)nodeToDelete->data)->data;
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
ListNode* findNodeWithUID( ListNode* nodeToCheck, const char* uid, _Bool(*doesDataMatchUID)(const char* uid, void* data) ){
	
	while( nodeToCheck != NULL && uid != NULL && (!(*doesDataMatchUID)( uid, nodeToCheck->data ) ) ){

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
	if( p == NULL ){
		return 0;
	}
	unsigned short int rightPID = strtoul( uid+1, NULL, 10 );

	return( *uid == p->leftPID[0] && rightPID == p->rightPID);
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
	if( i == NULL ){
		return 0;
	}

	char cidBuffer[ CID_MIN_SIZE ];
	unsigned char periodLocation = strcspn( uid, PERIOD_WORD_SEPARATOR );
	
	strncpy( cidBuffer, uid, periodLocation );
	cidBuffer[ periodLocation ] = '\0';
	unsigned short int leftCID = strtoul( cidBuffer, NULL, 10 );

	strcpy( cidBuffer, uid+periodLocation+1 );
	unsigned short int rightCID = strtoul( cidBuffer, NULL, 10 );

	return ( i->leftCID == leftCID && i->rightCID == rightCID );
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
ListNode* findNodeWithData( ListNode* nodeToCheck, void* data ){

	while( nodeToCheck != NULL && nodeToCheck->data != data ){
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
unsigned char getListSize( ListNode* currentNode ){

	unsigned char size = 0;

	while( currentNode != NULL ){
		++size;
		currentNode = currentNode->next;
	}
	return size;
}

