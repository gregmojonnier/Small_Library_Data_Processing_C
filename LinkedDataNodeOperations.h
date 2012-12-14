#ifndef LINKED_DATA_NODE_OPERATIONS_H
#define LINKED_DATA_NODE_OPERATIONS_H
#include "LinkedDataNodeStructures.h"


_Bool deleteAndFreeList( ListNode* currentHead, _Bool(*freeVoidDataFunction)(void* data) );
_Bool freeItemDataStruct( void* item );
_Bool freePatronDataStruct( void* patron );
_Bool deleteNode( ListNode** currentHead, ListNode* nodeToDelete, _Bool(*freeVoidDataFunction)(void* data) );


// Functions which are passed as a function pointer
// to help determine how to look up a UID
_Bool doesPatronMatchUID( const char* uid, void* data );
_Bool doesItemMatchUID( const char* uid, void* data );
ListNode* findNodeWithUID( ListNode* currentHead, const char* uid, _Bool(*doesDataMatchUID)(const char* uid, void* data) );
ListNode* findNodeWithData( ListNode* currentHead, void* data );


_Bool insertNodeAtHead( ListNode** currentHead, void* data );

// General utility LL functions
int getListSize( ListNode* head );
void printItemLinkedList( ListNode* headNode );
void printPatronLinkedList( ListNode* headNode );

#endif
