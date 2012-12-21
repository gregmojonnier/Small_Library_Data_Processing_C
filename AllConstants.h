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

#define LINE_MAX_SIZE 256
#define PID_MAX_SIZE 6
#define NAME_MAX_SIZE 36
#define CID_MIN_SIZE 4
#define CID_MAX_SIZE 8
#define AUTHOR_MAX_SIZE 52
#define TITLE_MAX_SIZE 52
#define ITEM_NUMS_MIN_SIZE 0
#define ITEM_NUMS_MAX_SIZE 99

#define DEFAULT_WORD_SEPARATORS " \t\n"
#define QUOTE_WORD_SEPARATOR "\""
#define PERIOD_WORD_SEPARATOR "."
#define PERIOD_WORD_SEPARATOR_CH '.'

#define ADD_PATRON_COMMAND "patron"
#define ADD_ITEM_COMMAND "item"
#define BORROW_ITEM_COMMAND "borrow"
#define RETURN_ITEM_COMMAND "return"
#define DISCARD_ITEM_COMMAND "discard"
#define OUT_COMMAND "out"
#define AVAILABLE_ITEM_COMMAND "available"

#endif
