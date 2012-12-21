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

#include <stdint.h>

// Main input processing function calls all others
// based on the first command token
void processInput( );

// All of these process the specific command and continue 
// parsing tokens from where processInput left off after the 1st command token
void processPatronCommand();
void processItemCommand();

uint_least8_t isValidCID( const char* cid );
uint_least8_t isValidPID( const char* pid );
uint_least8_t getSizeToTrimTailTo( const char* token, uint_least8_t maxCharsInString );
#endif
