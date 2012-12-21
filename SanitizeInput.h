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
#include <stdio.h>

// Main input processing function calls all others
// based on the first command token
void processInput( ListNode** itemsHead, ListNode** patronsHead );

// All of these process the specific command and continue 
// parsing tokens from where processInput left off after the 1st command token
void processPatronCommand( ListNode** patronsHead );
void processItemCommand( ListNode** itemsHead );
void processAvailableCommand( ListNode** itemsHead );
void processBorrowCommand( ListNode** itemsHead, ListNode** patronsHead );
void processDiscardCommand( ListNode** itemsHead );
void processOutCommand( ListNode** itemsHead, ListNode** patronsHead );
void processReturnCommand( ListNode** itemsHead, ListNode** patronshead );

_Bool isValidCID( const char* cid );
_Bool isValidPID( const char* pid );
char getValidItemsNum( const char* num );
unsigned char getSizeToTrimTailTo( const char* token, unsigned short int maxCharsInString );
#endif
