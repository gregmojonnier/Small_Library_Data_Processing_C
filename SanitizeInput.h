#ifndef SANITIZE_INPUT_H
#define SANITIZE_INPUT_H
/*
* This file contains methods which read commands in
* from the input.(Either from a file or STDIN)
* It then sanitize/parses the input to come up with the legal 
* commands which it then calls methods from ExecuteCommands.h
* to execute these commands.
*
*
* @author Greg Mojonnier
*/

#include "LinkedDataNodeStructures.h"

void ProcessInput( ListNode** itemsHead, ListNode** patronsHead, const char* fileName );
void ProcessPatronCommand( ListNode** patronsHead );
void ProcessItemCommand( ListNode** itemsHead );
void ProcessAvailableCommand( ListNode** itemsHead );
void ProcessBorrowCommand( ListNode** itemsHead, ListNode** patronsHead );
void ProcessDiscardCommand( ListNode** itemsHead );
void ProcessOutCommand( ListNode** itemsHead, ListNode** patronsHead );
void ProcessReturnCommand( ListNode** itemsHead, ListNode** patronshead );
_Bool IsValidCID( const char* cid );
_Bool IsValidPID( const char* pid );
int GetValidItemsNum( const char* num );
int GetSizeToTrimTailTo( const char* token, unsigned short int maxCharsInString );
#endif
