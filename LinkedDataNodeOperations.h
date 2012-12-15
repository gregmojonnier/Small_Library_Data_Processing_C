#ifndef LINKED_DATA_NODE_OPERATIONS_H
#define LINKED_DATA_NODE_OPERATIONS_H
/*
* This file contains methods that perform operations
* on linked lists of ListNodes. The ListNodes contain
* void* data which are either ItemData* or PatronData*.
*
*
* @author Greg Mojonnier
*/

#include "LinkedDataNodeStructures.h"


// Function to create and insert a ListNode into specified list
_Bool insertNodeAtHead( ListNode** currentHead, void* data );

// Functions to delete nodes from list of ListNodes
_Bool deleteNode( ListNode** currentHead, ListNode* nodeToDelete, _Bool(*freeVoidDataFunction)(void* data) );
_Bool deleteAndFreeList( ListNode* currentHead, _Bool(*freeVoidDataFunction)(void* data) );

// These are passed into delete node functions as function pointers
// to insure proper clean up based on what the nodes void* data represents
_Bool freeItemDataStruct( void* item );
_Bool freePatronDataStruct( void* patron );

// Functions to find a specific node based on a UID from list of ListNodes
ListNode* findNodeWithUID( ListNode* currentHead, const char* uid, _Bool(*doesDataMatchUID)(const char* uid, void* data) );

// These are passed into findNodeWithUID() as function pointer
// to properly match the UID based on what the nodes void* data represents
_Bool doesPatronMatchUID( const char* uid, void* data );
_Bool doesItemMatchUID( const char* uid, void* data );

// Function to find a specific node whose void* data == to the void* data argument
// This is used for ItemData and PatronData's sublists (patronsCurrentlyRenting, itemsCurrentlyRenting)
// which use their void* data strictly as a pointer
ListNode* findNodeWithData( ListNode* currentHead, void* data );

// General utility LL functions
int getListSize( ListNode* head );
void printItemLinkedList( ListNode* headNode );
void printPatronLinkedList( ListNode* headNode );

#endif
