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

// Main input processing function calls all others
// based on the first command token
void processInput( );

// All of these process the specific command and continue 
// parsing tokens from where processInput left off after the 1st command token
void processPatronCommand();
void processItemCommand();
void processAvailableCommand();
void processBorrowCommand();
void processDiscardCommand();
void processOutCommand();
void processReturnCommand();

_Bool isValidCID( const char* cid );
_Bool isValidPID( const char* pid );
char getValidItemsNum( const char* num );
unsigned char getSizeToTrimTailTo( const char* token, unsigned short int maxCharsInString );
#endif
