#ifndef LINKED_DATA_NODE_STRUCTURES_H
#define LINKED_DATA_NODE_STRUCTURES_H
/*
* This file contains structures that make up
* the linked list data structure used in this project.
*
*
* @author Greg Mojonnier
*/

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
* @leftCID -----------------> Item's left half of catalog ID(3 digits).
* @rightCID ----------------> Item's right half of catalog ID(3 digits).
* @numCopies ---------------> Number of copies library owns.
* @patronsCurrentlyRenting -> Linked list of void* to patrons renting item.
*
*/
typedef struct {
	char* author;
	char* title;
	unsigned int leftCID:10;
	unsigned int rightCID:10;
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
* @leftPID ---------------> Patron's left half of ID(1 char).
* @rightPID---------------> Patron's right half of ID(4 digits).
* @itemsCurrentlyRenting -> Linked list of void* to items curently renting.
*
*/
typedef struct {
	char* name;
	char leftPID[2];
	unsigned int rightPID:14;
	ListNode* itemsCurrentlyRenting;
} PatronData;

#endif
