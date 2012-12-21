#ifndef LINKED_DATA_NODE_OPERATIONS_H
#define LINKED_DATA_NODE_OPERATIONS_H
/*
* This file contains methods that perform operations
* on linked lists of ListNodes. The ListNodes contain
* void* data which are either ItemData* or PatronData* or ListNode*.
*
*
* @author Greg Mojonnier
*/

#include "LinkedDataNodeStructures.h"
#include <stdint.h>


// Function to create and insert a ListNode into specified list
void insertNodeInOrder( ListNode** currentHead, void* data, _Bool(*newDataHasLowerPrecedence)(void* _newData, void* _currentData) );

// These are passed into insertNodeInOrder, they determine
// if the new Patron/Item has a lower precedence than current
_Bool newPatronHasLowerPrecedence( void* _newPatron, void* _currentPatron );
_Bool newItemHasLowerPrecedence( void* _newItem, void* _currentItem );

// Functions to delete node from list of ListNodes
_Bool deleteNode( ListNode** currentHead, ListNode* nodeToDelete, void(*freeVoidDataFunction)(void* data) );
void deleteAndFreeBothLists( );

// These are passed into delete node functions as function pointers
// to insure proper clean up based on what the nodes void* data represents
void freeItemDataStruct( void* item );
void freePatronDataStruct( void* patron );

// Functions to find a specific node based on a UID from list of ListNodes
ListNode* findNodeWithUID( ListNode* nodeToCheck, const char* uid, unsigned char lookingUpPatron );

// Function to find a specific node whose void* data == to the void* data argument
// This is used for ItemData and PatronData's sublists (patronsCurrentlyRenting, itemsCurrentlyRenting)
// which use their void* data strictly as a pointer
ListNode* findNodeWithData( ListNode* nodeToCheck, void* data );

// General utility LL function
uint_least8_t getListSize( ListNode* currentNode );

#endif
