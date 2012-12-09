#include "ProcessCommands.h"
#include "LinkedDataNodeOperations.h"
#include <string.h>
#include <allocate.h>
#include <stdio.h>


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

	if( !insertNodeAtHead( &(*head), (void*)p ) ){
		return 0;
	}
	return 1;
}

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
	
	if( !insertNodeAtHead( &(*head), (void*)i ) ){
		return 0;
	}
	return 1;
}

int getCopiesAvailable( ListNode* head, const char* cid ){
	
	if( head == NULL ){
		return 0;
	}

	ListNode* itemNode = findNodeWithUID( &(*head), cid, doesItemMatchUID );
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
