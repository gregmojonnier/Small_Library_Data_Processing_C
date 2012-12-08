#ifndef LINKED_DATA_NODE_OPERATIONS_H
#define LINKED_DATA_NODE_OPERATIONS_H
#include "LinkedDataNodeStructures.h"

_Bool insertNodeAtHead( ListNode** currentHead, void* data );
_Bool freeItemDataStruct( void* item );
_Bool freePatronDataStruct( void* patron );
_Bool deleteAndFreeList( ListNode* currentHead, _Bool(*freeVoidDataFunction)(void* data) );
void printPatronLinkedList( ListNode* headNode );
#endif
