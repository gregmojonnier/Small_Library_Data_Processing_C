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

int getCopiesAvailable( ListNode* head, const char* cid );
_Bool borrowItem( ListNode** itemsHead, ListNode** patronsHead, const char* pid, const char* cid );
_Bool discardCopiesOfItem( ListNode** itemsHead, short int numToDelete, const char* cid);
_Bool addItem( ListNode** head, int numCopies, const char* cid, const char* author, const char* title );
void patronsWithItemOut( ListNode* itemsHead, ListNode* patronsHead, const char* cid );
void itemsOutByPatron( ListNode* itemsHead, ListNode* patronsHead, const char* pid );
_Bool returnPatronsItem( ListNode* itemsHead, ListNode* patronsHead, const char* pid, const char* cid );
_Bool addPatron( ListNode** head, const char* pid, const char* name );
#endif
