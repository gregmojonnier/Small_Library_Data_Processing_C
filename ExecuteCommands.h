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

unsigned char getCopiesAvailable( const char* cid );
_Bool borrowItem( const char* pid, const char* cid );
_Bool discardCopiesOfItem( unsigned char numToDelete, const char* cid);
_Bool addItem( unsigned char numCopies, const char* cid, const char* author, const char* title );
void patronsWithItemOut( const char* cid );
void itemsOutByPatron( const char* pid );
_Bool returnPatronsItem( const char* pid, const char* cid );
_Bool addPatron( const char* pid, const char* name );
void printAllListsStatus( );
void printItemStatus( ItemData* item );
void printPatronStatus( PatronData* patron );
#endif
