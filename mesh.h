#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/// @brief This is a presaved print statements for memory error checking
/// @param type Integer for which error statement to print
void memErr(int type);

/// @brief Helper function for 'parseit' that seperates by a delimiter
/// @param cmd         char* to the command input c string
/// @param cDelimiter  char* for the command delimiter to split the c string by
/// @param numOfCmds   int* that changes integer to number of commands parsed
/// @return            char** char** of parsed commands as an array
char** cmdParse(char* cmd,char* cDelimiter,size_t* numOfCmds);

/// @brief Custom command parser that seperates multiple commands and arugments
/// @param uInput      char* to the command c string
/// @param cDelimiter  char* for the command delimiter to split the c string by
/// @param aDelimiter  char* for the argument delimiter to split the c string by
/// @param numOfCmds   int* that changes integer to number of commands parsed
/// @return            char*** formatted in an array of arrays holding commands and arguments
char ***parseit(char* uInput, char* cDelimiter, char* aDelimiter, size_t* numOfCmds);

/// @brief Function to fork the shell and perform commands
/// @param cmds    An array of strings being the command and arguments
/// @date          1/3/19
int forker(char** cmds);

/// @brief Function to fork multiple commands simultaneously
/// @param array of arrays holding command and agrument c strings
/// @date				1/21/20
int parForker(char*** cmds, size_t cmdNum);

#endif // MESH_H