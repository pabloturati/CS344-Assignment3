#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../constants/constants.h"
#include "../subProcessHandlers/subProcessHandlers.h"
#include "builtinFunctions.h"

/* 
Creates an array of identifiers linked to functions to handle the shell built-in 
commands by matching the command and referencing the function index
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Shell Builtins"
*/
char *SHELL_COMMANDS[] = {"cd", "exit", "status"};

// Array of shell command functions, allows for execution based on array index
int (*shellCommandFunctions[])(char **) = {
    &cdCommand,
    &exitCommand,
    &helpCommand};

// Returns the array size of the identifiers. Allows for simple new command additions
int commandFuncArrLength()
{
  return sizeof(SHELL_COMMANDS) / sizeof(char *);
}

int cdCommand(char **args)
{
  // If cd is called without arguments, changes directory to HOME env var
  if (args[1] == NULL)
  {
    args[1] = getenv(HOME_ENV_VAR);
  }
  if (chdir(args[1]) != EXIT_SUCCESS)
  {
    perror("smallsh");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int exitCommand(char **args)
{
  exit(EXIT_SUCCESS);
  return getStatus();
}

int helpCommand(char **args)
{
  int status = getStatus();
  printf("%d\n", status);
  return status;
}

int isAnEmptyLineOrComment(char **args)
{
  return args[0] == NULL || strncmp(args[0], COMMENT_SIMBOL, 1) == 0;
}