#ifndef LINKED_DATA_NODE_STRUCTURES_H
#define LINKED_DATA_NODE_STRUCTURES_H

/*
* Data Structure: CheckedOutInfoNode
* ----------------------------------
*
* Represents checked out information.
* ItemNode will use it to track patrons.
* PatronNode will use it to track Items.
*
* @relevantNode -> Node containing relevant check out information.
* @next ---------> Next CheckedOutInfoNode in list.
*
*/
typedef struct _CheckedOutInfoNode {
	void* relevantNode;
	struct _CheckedOutInfoNode* next;
} CheckedOutInfoNode;

/*
* Data Structure: ItemNode
* ----------------------------------
*  
* Represents a library item.
*
* @author ------------------> Item's author.
* @title -------------------> Item's title.
* @cid ---------------------> Item's catalog ID.
* @numCopies ---------------> Number of copies library owns.
* @patronsCurrentlyRenting -> Linked list of void* to patrons renting item.
* @next --------------------> Next item node in inventory.
*
*/
typedef struct _ItemNode {
	char* author;
	char* title;
	char* cid;
	// allows 0-127
	unsigned int numCopies:7;
	CheckedOutInfoNode* patronsCurrentlyRenting;
	struct _ItemNode* next;
} ItemNode;

/*
* Data Structure: PatronNode
* ----------------------------------
*
* Represents a library patron.
*
* @name ------------------> Patron's name.
* @pid -------------------> Patron's ID.
* @next ------------------> Next patron node in patron list.
* @itemsCurrentlyRenting -> Linked list of void* to items curently renting.
*
*/
typedef struct _PatronNode {
	char* name;
	char pid[ 5 ];
	struct _PatronNode* next;
	CheckedOutInfoNode* itemsCurrentlyRenting;
} PatronNode;
#endif
