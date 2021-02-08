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

/* 
Launches a child process using the passed arguments  
Input: args (list of strings)
Output: 0 in case of success, otherwise error code > 0
Reference: parts of code adapted from example 4_2_execv_fork_ls
*/
int launchSubProcess(struct ShCommand *commandStruct)
{
  int childProcessStatus = 0;

  pid_t spawnPid = fork();
  switch (spawnPid)
  {
  case -1: // In case of Fork error
    perror(FORK_ERROR_MSG);
    return 1;
  case 0: // Child process
    if (adjustProcessStreams(commandStruct) == 0)
    {
      // Execute command
      execvp(commandStruct->path, commandStruct->args);
      // exec returns only if there is an error. Flush stdout and redirect to screen.
      perror(EXEC_ERROR_MSG_LABEL);
      fflush(stdout);
    }
    return killChildProcess();
  default:
    // If is background process, continue without waiting.
    if (commandStruct->isBackgroundProcess)
    {
      printf(BACKGROUND_PROCESS_ID_MSG, getpid());
      waitpid(spawnPid, &childProcessStatus, WNOHANG);
    }
    else
    {
      // Else wait for child to finish.
      waitpid(spawnPid, &childProcessStatus, 0);
    }
  }
  return childProcessStatus > 0;
}

/*
Handles input redirection
If input or output redirection found, cuts command list and redirects accordingly
Input: struct ShCommand
Output: 0 in case of success, otherwise error code > 0
*/
int adjustProcessStreams(struct ShCommand *commandStruct)
{
  int outputRedirectStatus = 0;
  int inputRedirectStatus = 0;

  // If the user requested a redirect or if this is a background process
  if (commandStruct->inRedirFile || commandStruct->isBackgroundProcess)
  {
    // Redirects input accouding to user request or, if is background process, to /dev/null
    char *inputRedirectFile = commandStruct->inRedirFile ? commandStruct->inRedirFile : NULL_REDIRECT_PATH;
    inputRedirectStatus = handleRedirectFlow(inputRedirectFile, INPUT_OPERATION, openFileForReading);
  }
  if (commandStruct->outRedirFile || commandStruct->isBackgroundProcess)
  {
    char *outputRedirectFile = commandStruct->outRedirFile ? commandStruct->inRedirFile : NULL_REDIRECT_PATH;
    outputRedirectStatus = handleRedirectFlow(outputRedirectFile, OUTPUT_OPERATION, openFileForWriting);
  }
  return (inputRedirectStatus == 0 && outputRedirectStatus == 0) ? 0 : 1;
}

/* 
Executes a builtin shell command or defers the request to a child subprocess 
Input: args (list of strings)
Output: 0 in case of success, otherwise error code > 0
Reference: parts of code adapted from example 4_2_execv_fork_ls 
"Putting together bultin and processes"
*/
int executeCommand(struct ShCommand *commandStruct)
{
  if (isAnEmptyLineOrComment(commandStruct->args))
  {
    return EXIT_FAILURE;
  }
  // Search for the command in the list of builtins
  for (int i = 0; i < commandFuncArrLength(); i++)
  {
    if (strcmp(commandStruct->args[0], SHELL_COMMANDS[i]) == 0)
      return (*shellCommandFunctions[i])(commandStruct->args);
  }

  // If not found in the builtin commands, execute a subprocess
  return launchSubProcess(commandStruct);
}
