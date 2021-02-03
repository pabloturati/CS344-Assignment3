#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../constants/constants.h"
#include "../builtinFunctions/builtinFunctions.h"

/* 
Launches a child process using the passed arguments  
Input: args (list of strings)
Output: 0 in case of success, otherwise error code > 0
Reference: parts of code adapted from example 4_2_execv_fork_ls
*/
int launchSubProcess(char **args)
{
  int childProcessStatus;
  pid_t spawnPid = fork();

  switch (spawnPid)
  {
  case -1: // In case of Fork error
    perror("fork()\n");
    return 1;
  case 0: // Child process
    printf("Child(%d) is running command\n", getpid());
    execvp(args[0], args);
    // exec only returns if there is an error
    perror("exec_error");
    return 2;
  default:
    // Parent execution. Waits for child to complete.
    printf("marker here\n");
    spawnPid = waitpid(spawnPid, &childProcessStatus, 0);
    printf("Parent(%d): Child(%d) terminated\n", getpid(), spawnPid);
  }
  return 0;
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

/*
Creates a new string placing the process id where there is a process variable
Input: str (string) - command containing process id, processId (integer)
Output: (string *)
*/
char *expandProcessVar(char *str, int processId)
{
  char *ptr = strstr(str, PROCESS_ID_VARIABLE);
  //Return if there is no process id variable. Recursive base case
  if (!ptr)
    return str;

  // Allocate buffer for substrings
  char *buffer = calloc(VAR_EXPANSION_BUFF_SIZE, sizeof(char));
  char *bufferEnd = calloc(VAR_EXPANSION_BUFF_SIZE, sizeof(char));

  int indexOfProcessVar = ptr - str;

  // Copy the head of the string up to the $$ identifier
  strncpy(buffer, str, indexOfProcessVar);
  buffer[indexOfProcessVar] = '\0';

  // Index of tail start = right after process var
  char *indexOfTailStart = str + indexOfProcessVar + strlen(PROCESS_ID_VARIABLE);

  // Index of tail end = right after process var
  unsigned long indexOfTailEnd = strlen(str) - indexOfProcessVar;
  // Copy the tails of the string from the identifier
  strncpy(bufferEnd, indexOfTailStart, indexOfTailEnd);
  bufferEnd[indexOfTailEnd] = '\0';

  sprintf(buffer + indexOfProcessVar, "%d%s", processId, bufferEnd);
  free(bufferEnd);

  // Recursive call in case of other variable expansions
  return expandProcessVar(buffer, processId);
}

int getShellProcessId()
{
  return getpid();
}