# Memory Efficient Shell (MESH)
MESH is the first assignment for Dr. Xan's CSCI 485 class.

## Design Overview
### Structure
1. MESH takes an input line (interactive = "stdin", batch = "File* Stream) dynamically increasing the buffer size until the end of the line.
2. MESH then parses the input line into separate c strings for each command.
3. MESH next parses the commands into an array of c strings that are separated by each argument.
4. MESH finally uses a custom forker to fork and simultaneously execute each command on the original line
   1. The forker also checks for the exit/quit command and sends a signal back to main program to exit once all commands are executed.
5. MESH then loops until "exit","quit", or EOF are called.
   1. "CTRL+D" is counted by the system as an manual EOF so the program also exits then as well.
6. Frees any remaining dynamic pointers and returns "0".

### Specifications
* The dynamic buffers are reallocated once all changes are completed for them, so as to not take any extra space.
* The dynamic buffers act as vectors when resizing themselves, starting at chunks of 8 of specific types.
* "execvp" was used as the executor as to auto include "$PATH" elements.
* incorrect number of arguments for the program indicates, "Too many arguments" and Exits the program.
* empty inputs and extra whitespace are removed at parsing and are not forked or executed.
* As "strtok()" function used in parsing does not declare new dynamic memory they are freed along with original input buffer at exiting program.

## Compilation
Requires preferably POSIX compliant system and [gcc](https://gcc.gnu.org/) \
Use the Makefile provided to compile the code to an executable.
```bash
make
```

## Usage
To enter 'interactive mode' directly call the executable
```bash
./mesh
```
To use 'batch mode' call the executable with a singular file as a argument
```bash
./mesh foobar
```

To exit the program enter into the shell 'exit' or 'quit' as a command
```bash
READY > exit
```
## Objectives
- [x] Interactive & Batch Modes
- [x] Multiple Commands via seperators ";","|", and "&"
- [x] Simultaneous execution of multi-commands
- [x] Defensive Programming with multiple error checks
- [x] Unrestricted limit of characters via dynamic memory allocation
- [x] Messages to "stderr" for multiple arguments in execution
- [x] Makefile made.
- [x] Readme created via Github Markdown.
- [x] Runs normally on cs2 server.

## Bugs
When echoing the command line from batch files the last line will print *after* the commands are executed. 

## Contributors
[Garrett Hay](https://github.com/Garrett-H)

## License
[MIT](https://choosealicense.com/licenses/mit/)