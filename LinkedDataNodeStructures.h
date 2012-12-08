#ifndef LINKED_DATA_NODE_STRUCTURES_H
#define LINKED_DATA_NODE_STRUCTURES_H

/*
* Data Structure: ListNode
* ----------------------------------
*
* Represents a generic list node for linked lists.
*
* @data ------------------> Generic data, most likely PatronData or ItemData struct.
* @next ------------------> Next linked list node.
*
*/
typedef struct _ListNode {
	void* data;
	struct _ListNode* next;
} ListNode;

/*
* ItemData
* ----------------------------------
*  
* Represents a library item.
*
* @author ------------------> Item's author.
* @title -------------------> Item's title.
* @cid ---------------------> Item's catalog ID.
* @numCopies ---------------> Number of copies library owns.
* @patronsCurrentlyRenting -> Linked list of void* to patrons renting item.
*
*/
typedef struct {
	char* author;
	char* title;
	char* cid;
	// allows 0-127
	unsigned int numCopies:7;
	ListNode* patronsCurrentlyRenting;
} ItemData;

/*
* PatronData
* ----------------------------------
*
* Represents a library patron.
*
* @name ------------------> Patron's name.
* @pid -------------------> Patron's ID.
* @itemsCurrentlyRenting -> Linked list of void* to items curently renting.
*
*/
typedef struct {
	char* name;
	char pid[ 6 ];
	ListNode* itemsCurrentlyRenting;
} PatronData;

#endif
