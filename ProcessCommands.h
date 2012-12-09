#ifndef PROCESS_COMMANDS_H
#define PROCESS_COMMANDS_H
#include "LinkedDataNodeStructures.h"

_Bool addPatron( ListNode** head, const char* pid, const char* name );
_Bool addItem( ListNode** head, int numCopies, const char* cid, const char* author, const char* title );
int getCopiesAvailable( ListNode* head, const char* cid );
_Bool borrowItem( ListNode** itemsHead, ListNode** patronsHead, const char* pid, const char* cid );
#endif
