#ifndef ALL_CONSTANTS_H
#define ALL_CONSTANTS_H
/*
* This file contains constants that will be used
* throughout the project. Primarily in SanitizeInput though.
* All SIZE constants account for a spot for \0.
*
*
* @author Greg Mojonnier
*/

static const int LINE_MAX_SIZE = 256;
// pid size max is 6( this is 5 + 1 spot or \0)
static const int PID_MAX_SIZE = 6;
static const int NAME_MAX_SIZE = 36;
static const int CID_MIN_SIZE = 4;
static const int CID_MAX_SIZE = 8;
static const int AUTHOR_MAX_SIZE = 52;
static const int TITLE_MAX_SIZE = 52;
static const int NUM_COPIES_MIN_SIZE = 0;
static const int NUM_COPIES_MAX_SIZE = 99;

static const char* DEFAULT_WORD_SEPARATORS = " \t\n";
static const char* QUOTE_WORD_SEPARATOR = "\"";
static const char PERIOD_WORD_SEPARATOR = '.';


static const char* ADD_PATRON_COMMAND = "patron";
static const char* ADD_ITEM_COMMAND = "item";

#endif
