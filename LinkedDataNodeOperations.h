#ifndef LINKED_DATA_NODE_OPERATIONS_H
#define LINKED_DATA_NODE_OPERATIONS_H
#include "LinkedDataNodeStructures.h"

_Bool insertNodeAtHead( ListNode** currentHead, void* data );
_Bool freeItemDataStruct( void* item );
_Bool freePatronDataStruct( void* patron );
_Bool deleteAndFreeList( ListNode* currentHead, _Bool(*freeVoidDataFunction)(void* data) );
void printPatronLinkedList( ListNode* headNode );
_Bool doesPatronMatchUID( const char* uid, void* data );
_Bool doesItemMatchUID( const char* uid, void* data );
ListNode* findNodeWithUID( ListNode* currentHead, const char* uid, _Bool(*doesDataMatchUID)(const char* uid, void* data) );
ListNode* findNodeWithData( ListNode* currentHead, void* data );
_Bool deleteNode( ListNode** currentHead, ListNode* nodeToDelete, _Bool(*freeVoidDataFunction)(void* data) );
int getListSize( ListNode* head );

#endif
