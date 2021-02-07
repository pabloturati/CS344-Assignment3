#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "../constants/constants.h"
#include "../builtinFunctions/builtinFunctions.h"
#include "subProcessHandlers.h"

void printArgs(char **args, char *message)
{
  printf("%s\n", message);
  int i = 0;
  printf("%s\n", args[0]);
  while (args[i] != NULL)
  {
    printf("%s\n", args[i]);
    ++i;
  }
}

/* 
Launches a child process using the passed arguments  
Input: args (list of strings)
Output: 0 in case of success, otherwise error code > 0
Reference: parts of code adapted from example 4_2_execv_fork_ls
*/
int launchSubProcess(char **args)
{
  int childProcessStatus = 0;

  pid_t spawnPid = fork();
  switch (spawnPid)
  {
  case -1: // In case of Fork error
    perror("fork()\n");
    return 1;
  case 0: // Child process
    if (adjustProcessStreams(args) == 0)
    {
      // Execute command
      execvp(args[0], args);
      // exec returns only if there is an error. Flush stdout and redirect to screen.
      perror(EXEC_ERROR_MSG_LABEL);
      fflush(stdout);
    }
    return killChildProcess();
  default:
    // Parent execution. Waits for child to complete.
    waitpid(spawnPid, &childProcessStatus, 0);
  }
  return childProcessStatus > 0;
}

/*
Traverses command list searching for input or output redirection commands
If input or output redirection found, cuts command list and redirects accordingly
Input: args (list of strings)
Output: 0 in case of success, otherwise error code > 0
*/
int adjustProcessStreams(char **args)
{
  int i = 0, inputRedirectStatus = 0, outputRedirectStatus = 0;

  while (args[i] != NULL)
  {
    // If it finds input redirect symbol <, substitute for NULL and redirect stdin.
    if (strncmp(REDIRECT_INPUT_SYMBOL, args[i], 1) == 0)
    {
      inputRedirectStatus = handleRedirectFlow(args, i, INPUT_OPERATION, openFileForReading);
      // Quit on error
      if (inputRedirectStatus > 0)
        return 1;
    }
    //If it finds output redirect symbol >, substitute for NULL and redirect stdout.
    else if (strncmp(REDIRECT_OUTPUT_SYMBOL, args[i], 1) == 0)
    {

      outputRedirectStatus = handleRedirectFlow(args, i, OUTPUT_OPERATION, openFileForWriting);
      // Quit on error
      if (outputRedirectStatus > 0)
        return 1;
    }
    ++i;
  }
  return inputRedirectStatus && outputRedirectStatus == 0;
}

/* 
Executes a builtin shell command or defers the request to a child subprocess 
Input: args (list of strings)
Output: 0 in case of success, otherwise error code > 0
Reference: parts of code adapted from example 4_2_execv_fork_ls 
"Putting together bultin and processes"
*/
int executeCommand(char **args)
{
  if (isAnEmptyLineOrComment(args))
  {
    return EXIT_FAILURE;
  }
  // Search for the command in the list of builtins
  for (int i = 0; i < commandFuncArrLength(); i++)
  {
    if (strcmp(args[0], SHELL_COMMANDS[i]) == 0)
      return (*shellCommandFunctions[i])(args);
  }

  // If not found in the builtin commands, execute a subprocess
  return launchSubProcess(args);
}
