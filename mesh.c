#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * Global variable for the default size of dynamic memory allocation
 * @date    1/4/20
 */
#define DCHUNK 8;

/**
 * Custom printing to stderr for memory allocation error checking
 * @param type  The type of memory error to display    
 * @date        1/1/20
 */
void memErr(int type) {
    switch(type) {
        case 1: fprintf(stderr, "Initial memory allocation error\n");
            break;
        case 2: fprintf(stderr, "Memory reallocation error\n");
            break;
        default: fprintf(stderr, "Error not a memory error type");
    }
}

/**
 * Helper function for 'parseit' that takes a c string, an array of
 * characters to seperate them by, and a integer that will be
 * given the number of commands found. Then it returns an array of 
 * c strings that are seperated.
 * @param cmd         char* to the input c string
 * @param cDelimiter  char* for the delimiter to split the c string by
 * @param numOfCmds   size_t* that changes integer to number of commands parsed
 * @date              1/4/20
 * @return            char** of parsed commands as an array
 */
char** cmdParse(char* cmd,char* cDelimiter,size_t* numOfCmds) {
    //initialization of array of command strings    
    int chunk = DCHUNK;
    char** cmds = (char**) malloc(sizeof(char*) * chunk);
    if(!cmds) {
        memErr(1);
        return cmds;
    }
    //grab initial string and array location
    char* tok;
    size_t len = 0;
    tok = strtok(cmd,cDelimiter);
    //while not at end of string
    while(tok != NULL) {
        //add current token to array
        cmds[len] = tok;
        len++;
        //reallocate size of string to be doubled
        if(len == chunk) {
            chunk *= 2;
            cmds = realloc(cmds, (sizeof(char) * (chunk+1)));
            if(!cmds) {  //memory error checking
                memErr(2);
                return cmds;
            }
        }
        //assigning next token
        tok = strtok(NULL,cDelimiter);
    }
    //adding null character at end of string
    cmds[len] = NULL;
    (*numOfCmds) = len;
    return realloc(cmds, (sizeof(char*) * len+1));
}

/**
 * Custom command parser that takes a c string command, array of characters to
 * split seperate commands by, array of characters to split command arguments
 * by, and a integer that will be given the number of commands found. Then it 
 * returns an array of command arrays each filled with c string commands and
 * argumentsof the command defined by the given delimiters.
 * @param uInput      char* to the command c string
 * @param cDelimiter  char* for the command delimiter to split the c string by
 * @param aDelimiter  char* for the argument delimiter to split the c string by
 * @param numOfCmds   size_t* that changes integer to number of commands parsed
 * @date              1/21/20
 * @return            char*** formatted commands and arguments arrays inside a main array
 */
char ***parseit(char* uInput, char* cDelimiter, char* aDelimiter, size_t* numOfCmds) {
    char** cmds = cmdParse(uInput,cDelimiter,numOfCmds);
    /***            NEED ERROR CHECKING if cmds is NULL             ***/

    //pointer legend
    //*** - array of arrays (all execs)
    //**  - array of strings (1 exec group)
    //*   - string (cmd or args)

    //setup main list
    //+1 for null
    char*** listOfExec = (char***) malloc(sizeof(char**) * (*numOfCmds)+1);
    if(!listOfExec) { //error checking for memory
        memErr(1);
        return NULL;
    }

    size_t a;
    int len=0;
    for(int i = 0; cmds[i] != NULL; ++i) {
        char** tmp = cmdParse(cmds[i],aDelimiter,&a);
        if(tmp[0] == NULL) { (*numOfCmds) -= 1; }
        else { listOfExec[len++] = tmp; }
    }
    listOfExec[len] = NULL;
    //free dynamic array of number of commands
    free(cmds);
    return realloc(listOfExec, (sizeof(char**) * (len+1)));
}

/**
 * A function that asynchronously executes commands 
 * @author        Garrett Hay
 * @param cmds    An array of strings being the command and arguments
 * @date          1/14/20
 * @return        int of status of process 1-success, 0-fail/exit
 */
int forker(char** cmds) {    
    pid_t pid;
    //pid_t wpid;
    int stat;

    //try to start child and parent fork
    pid = fork();
    //if child process start the command
    if(pid == 0) {
        //try command
        if(execvp(cmds[0], cmds) == -1) {
            printf("Command not found.\n");
            exit(1);
        }
    }
    //if error on fork print error
    else if(pid < 0) {
        fprintf(stderr,"Error on Forking"); 
        exit(1);
    }
    //if parent process wait for child process to end
    else {
        do {
            //wpid = waitpid(pid,&stat,WUNTRACED); 
            waitpid(pid,&stat,WUNTRACED);
        }
        while(!WIFEXITED(stat) && !WIFSIGNALED(stat));
    }

    return 1;
}

/**
 * A function synchronously executes any commands among the array
 * @author        Garrett Hay
 * @param cmds    An array of command arrays each in their own strings of commands and arguments
 * @param cmdNum  unsigned integer that gives the number of commands to execute
 * @date          1/21/20
 * @return        int of status of process 1-success, 0-fail/exit
 */
int parForker(char ***cmds, size_t cmdNum) {
    //array of all pIDs for children
    pid_t *pids = (pid_t *)malloc(sizeof(pid_t) * cmdNum);
    if (!pids) { //error checking for memory
        memErr(1);
        return 0;
    }

    int stat;
    int ifExit = 0;
    for (int i = 0; cmds[i] != NULL; ++i) {
        //check if exit
        if ((strcmp(cmds[i][0], "exit") == 0) || (strcmp(cmds[i][0], "quit") == 0)) {
            ifExit = 1;
            pids[i] = -1;
        }
        else {
            //try to start child and parent fork
            pids[i] = fork();
            //if child process start the command
            if (pids[i] == 0) {
                //try command
                if (execvp(cmds[i][0], cmds[i]) == -1) { printf("Command not found.\n"); }
                exit(1);
            }
            //if error on fork print error
            else if (pids[i] < 0) {
                fprintf(stderr, "Error on Forking");
                exit(1);
            }
        }
    }

    //if parent process wait for child process to end
    for (int i = 0; i < cmdNum; ++i) {
        if (pids[i] > 0) {
            do { waitpid(pids[i], &stat, WUNTRACED); } 
            while (!WIFEXITED(stat) && !WIFSIGNALED(stat));
        }
    }
    //free dynamic memory
    free(pids);
    if (ifExit == 1) { return 0; }
    else { return 1; }
}