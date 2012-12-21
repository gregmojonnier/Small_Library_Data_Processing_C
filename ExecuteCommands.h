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
#include <stdint.h>

uint_least8_t getCopiesAvailable( const char* cid );
void borrowItem( const char* pid, const char* cid );
void discardCopiesOfItem( uint_least8_t numToDelete, const char* cid);
void addItem( uint_least8_t numCopies, const char* cid, const char* author, const char* title );
void patronsWithItemOut( const char* cid );
void itemsOutByPatron( const char* pid );
void returnPatronsItem( const char* pid, const char* cid );
void addPatron( const char* pid, const char* name );
void printAllListsStatus( );
void printItemStatus( ItemData* item );
void printPatronStatus( PatronData* patron );
#endif
