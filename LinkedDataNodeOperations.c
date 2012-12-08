#include "LinkedDataNodeOperations.h"
#include <allocate.h>
#include <stdio.h>
#include <string.h>


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
		unallocate( nodeToDelete );
		nodeToDelete = next;
	}
	unallocate( i );
	return 1;
}

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
		unallocate( nodeToDelete );
		nodeToDelete = next;
	}
	unallocate( p );
	return 1;
}
_Bool deleteAndFreeList( ListNode* currentHead, _Bool(*freeVoidDataFunction)(void* data) ){

	if( currentHead == NULL ){
	// empty list
		return 1;
	}	

	ListNode* nodeToDelete = currentHead;
	while( nodeToDelete != NULL ){
		// check result of this?
		(*freeVoidDataFunction)(nodeToDelete->data);
		ListNode* next = nodeToDelete->next;
		unallocate( nodeToDelete );
		nodeToDelete = next;
	}

	currentHead = NULL;	
	return 1;
}

void printPatronLinkedList( ListNode* headNode ){

	ListNode* patronToPrint = headNode;

	while( patronToPrint != NULL ){
		void* _patronData = patronToPrint->data;
		PatronData* patronData = (PatronData*) _patronData;
		printf("Patron: %s\n", patronData->name);
		patronToPrint = patronToPrint->next;
	}
}


_Bool doesPatronMatchUID( const char* uid, void* data ){
	PatronData* p = (PatronData*)data; 
	if( p == NULL ){
		return 0;
	}
			puts("We will be returning \n \n");
	printf("is this %i\n", strcmp( p->pid, uid ) );
	printf("equal to 0? %i\n", ( strcmp( p->pid, uid ) == 0 ) );
	return ( strcmp( p->pid, uid ) == 0 );
}

_Bool doesItemMatchUID( const char* uid, void* data ){
	ItemData* i = (ItemData*)data; 
	if( i == NULL ){
		return 0;
	}
	return ( strcmp( i->cid, uid ) == 0 );
}

ListNode* findNodeWithUID( ListNode* currentHead, const char* uid, _Bool(*doesDataMatchUID)(const char* uid, void* data) ){
	if( currentHead == NULL ){
		return NULL;
	}
	
	ListNode* nodeToCheck = currentHead;

	while( nodeToCheck != NULL ){

		if( (*doesDataMatchUID)( uid, nodeToCheck->data ) ){
			puts("-------FOUND THE MATCH-------");
			break;
		}
		nodeToCheck = nodeToCheck->next;
	}
	return nodeToCheck;
}




