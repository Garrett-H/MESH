/**
 * @file    : main.c
 * @author  : Garrett Hay & Christina Hinton
 * @date    : Jan 27 2020
 * @brief   : 1st attempt at building a shell program
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mesh.h"

/**
 * the main function that controls what the program does
 * @param argc   Holds the number of arguments given in command line
 * @param arg[]  Holds the filename and arguments given in command line
 * @date 01/16/20
 * @return int
 */
int main(int argc, char *arg[]) {
  char* aDelimiter = " \t\r\n\a";
  char* cDelimiter = ";|&";
  size_t bufsize = 0;
  FILE* fin;
  int len;
  size_t cmdNum =0;
  char* buffer = (char*)malloc(0);
  char*** parseCmd = (char***)malloc(0);
  int loop = 0;

  //error checking for memory error
  if(!buffer) { memErr(1); }
  if(!parseCmd) { memErr(1); }

  //batch mode
  if (argc == 2) {
    fin = fopen(arg[1],"r");
    if(fin != NULL) { loop = 1; }
    else { 
      loop = 0;
      fprintf(stderr,"File Not Found\n");
    }
  }
  else if (argc > 2) {
    loop = 0;
    fprintf(stderr,"Too many arguments\n");
  }
  //user mode
  else { loop = 1; }
  //shell loop
  while (loop) {
    //file input
    if (argc > 1) { len = getline(&buffer,&bufsize,fin); }
    //user input
    else {
      printf("READY >");
      len = getline(&buffer,&bufsize,stdin);
    }
    //EOF read
    if(len<0) {
      printf("\n");
      loop    =0;
      cmdNum  =0;
    }
    else {
      //if a file echo the line
      if(argc == 2) { printf("%s\n",buffer); }
      //parse the input for commands
      parseCmd = parseit(buffer,cDelimiter,aDelimiter,&cmdNum); 
      /***  Needs error checking if returns NULL  ***/
    }
    if(cmdNum < 0) {fprintf(stderr, "Error: input size was negative\n");}
    //if no commands given
    else if(cmdNum == 0) {}
    //start executing commands
    else { loop = parForker(parseCmd,cmdNum); }
  }
  
  printf("Exiting\n");
  //freeing all dynamic pointers
  //should not need to free all tokens since strtok does reallocate memory
  free(buffer);
  for (size_t i = 0;parseCmd[i] != NULL; ++i) { free(parseCmd[i]); }
  free(parseCmd);

  return 0;
}