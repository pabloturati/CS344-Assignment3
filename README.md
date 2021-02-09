# CS344-Assignment3
CS344-Assignment 3 smallsh\
Author: Pablo Turati - turatip@oregonstate.edu\
Licence: MIT. Refer to `LICENSE` for details.

## Project Description

`smallsh` is a simplified terminal application that provides the following functionality:

- Prompt for running commands
- Handles blank lines and comments (lines beginning with the `#` character)
- Expands the PID `$$` variable.
- Executes `exit`, `cd` and `status` via built in code into the shell
- Executes other commands by creating new processes using exec() functions.
- Supports IO redirection
- Supports running commands in foreground and background processes
- Implements custom handlers for SIGINT and SIGTSTP 


## Compile instructions

This project uses GNU99 compile standards for building the executable.

### Option 1.  Using compile script

This application includes a bash script to build and deploy the runner (compileApp.sh).  To use it, make sure that this file (compileApp.sh) has execution persmissions by running:

`$ chmod +x compileApp.sh`

You may execute the compile instructions by running from the project root:

`$ ./compileApp.sh`

This will generate a `smallsh` exectable file as output. Refer to the next section on how to run the application and pass parameters.

### Option 2.  Individual commands

Refer to the contents of `compileApp.sh` for a detailed command list on how to manually compile using command line.  Notice that this project was designed to be compiled with GNU99 standards.

### Option 3.  Compile and run in sigle step

`compileApp.sh` accepts the flag `-e`, in which case it will compile, generate the executable and immediately run it.  To do this run:

`$ compileApp.sh -e`

### Compile run available commands

The following flags can be passed to the compileApp.sh shell script:
  - e -> run executable
  - v -> run executable with valgrind leak analysis
  - t -> run executable with testscript to stdout
  - r -> run executable with testscript and send results to a report file
  - c -> performs only cleaning of compilation files and creted test files

## Run procedure

Once the application executable has been created `smallsh`. It can be run as follows:

`$ ./smallsh`

To compile and run in a single step run: `$ compileApp.sh -e`

## Additional resources

## Project File structure
```
root
├── .gitignore
├── compileApp.sh
├── main.c
├── LICENSE
├── README.md
├── constants
│   ├── constants.c
│   └── constants.h
├── activeProcessHandlers
│   ├── activeProcessHandlers.c
│   └── activeProcessHandlers.h
├── ioHandlerMethods
│   ├── ioHandlerMethods.c
│   └── ioHandlerMethods.h
├── signalHandlers
│   ├── signalHandlers.c
│   └── signalHandlers.h
├── subProcessHandlers
│   ├── subProcessHandlers.c
│   └── subProcessHandlers.h
└── builtinFunctions
    ├── builtinFunctions.c
    └── builtinFunctions.h
```


## Version control

 Link to the repository is available at [here](https://github.com/pabloturati/CS344-Assignment3).  However, due to this being an Oregon State University assignment, it will remain private and public access will become available after April, 2021.