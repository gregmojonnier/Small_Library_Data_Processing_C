#ifndef PROCESS_COMMANDS_H
#define PROCESS_COMMANDS_H
#include "LinkedDataNodeStructures.h"

_Bool addPatron( ListNode** head, const char* pid, const char* name );
_Bool addItem( ListNode** head, int numCopies, const char* cid, const char* author, const char* title );
int getCopiesAvailable( ListNode* head, const char* cid );
_Bool borrowItem( ListNode** itemsHead, ListNode** patronsHead, const char* pid, const char* cid );

void patronsWithItemOut( ListNode* itemsHead, ListNode* patronsHead, const char* cid );
void itemsOutByPatron( ListNode* itemsHead, ListNode* patronsHead, const char* pid );
_Bool returnPatronsItem( ListNode* itemsHead, ListNode* patronsHead, const char* pid, const char* cid );
_Bool discardCopiesOfItem( ListNode** itemsHead, short int numToDelete, const char* cid);
#endif
