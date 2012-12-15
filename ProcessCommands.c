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

	if( findNodeWithData( patron->itemsCurrentlyRenting, &(*itemNode) ) != NULL ){
		fprintf( stderr, "%s already has %s checked out\n", pid, cid );
		return 0;
	}

	if( insertNodeAtHead( &patron->itemsCurrentlyRenting, &(*itemNode) ) && insertNodeAtHead( &item->patronsCurrentlyRenting, &(*patronNode) ) ){
		return 1;
	}
	return 0;
}

void patronsWithItemOut( ListNode* itemsHead, ListNode* patronsHead, const char* cid ){

	ListNode* itemNode = findNodeWithUID( itemsHead, cid, doesItemMatchUID );
	
	if( itemNode == NULL ){
		fprintf( stderr, "%s does not exist", cid );		
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


void itemsOutByPatron( ListNode* itemsHead, ListNode* patronsHead, const char* pid ){

	ListNode* patronNode = findNodeWithUID( patronsHead, pid, doesPatronMatchUID );
	
	if( patronNode == NULL ){
		fprintf( stderr, "%s does not exist", pid );		
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

