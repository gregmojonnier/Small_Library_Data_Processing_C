#ifndef PROCESS_COMMANDS_H
#define PROCESS_COMMANDS_H
/*
* This file contains methods which each map to a legal command.
* The methods in general are specifing how the more generic
* functions from LinkedDataNodeOperations.h should interpret
* ListNode*'s void* data.
*
*
* @author Greg Mojonnier
*/

#include "LinkedDataNodeStructures.h"

unsigned char getCopiesAvailable( ListNode* itemsHead, const char* cid );
_Bool borrowItem( ListNode** itemsHead, ListNode** patronsHead, const char* pid, const char* cid );
_Bool discardCopiesOfItem( ListNode** itemsHead, unsigned char numToDelete, const char* cid);
_Bool addItem( ListNode** head, unsigned char numCopies, const char* cid, const char* author, const char* title );
void patronsWithItemOut( ListNode* itemsHead, const char* cid );
void itemsOutByPatron( ListNode* patronsHead, const char* pid );
_Bool returnPatronsItem( ListNode* itemsHead, ListNode* patronsHead, const char* pid, const char* cid );
_Bool addPatron( ListNode** head, const char* pid, const char* name );
void printAllItemsStatus( ListNode* itemToPrint );
void printAllPatronsStatus( ListNode* patronToPrint );
void printItemStatus( ItemData* item );
void printPatronStatus( PatronData* patron );
#endif
