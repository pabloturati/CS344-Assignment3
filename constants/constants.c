#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "constants.h"

const char *TOO_FEW_ARGUMENT_MSG = "smallsh: Unable to execute, too few arguments\n";
const char *EXEC_ERROR_MSG_LABEL = "exec_error";
const char *MISSING_PARAM_ERROR_MSG_LABEL = "missing_param";
const char *OPEN_READ_FILE_ERROR_MSG_LABEL = "source open()";
const char *OPEN_WRITE_FILE_ERROR_MSG_LABEL = "target open()";

/*********** Global status and accessors ***********/

static int status = 0;

/*
Getter function for global variable 'status'
Input: void
Output: returns current value of 'status' global variable (int)
*/
int getStatus()
{
  return status;
}

/*
Setter function for global variable 'status'
Input: new current value of 'status' global variable (int)
Output: void 
*/
void setStatus(int newStatus)
{
  status = newStatus;
}

/*********** Process helper methods ***********/

/*
Kills current process and returns error signal
Input: void
Output: returns constant error code (integer)
*/
int killChildProcess()
{
  kill(getpid(), SIGTERM);
  return KILL_PROCESS_RETURN_VAL;
}

/*
Function to get shell process id from files that do not have 
<unistd.h> header.
Input: void
Output: currentProcessId()
*/
int getShellProcessId()
{
  return getpid();
}

void reportErrorAndFlushStdOut(const char *errorMsg)
{
  perror(errorMsg);
  fflush(stdout);
}

int openFileForReading(char *fileStr)
{
  int sourceFile = open(fileStr, O_RDONLY);
  if (sourceFile == -1)
    reportErrorAndFlushStdOut(OPEN_READ_FILE_ERROR_MSG_LABEL);
  return sourceFile;
}

int openFileForWriting(char *fileStr)
{
  int destinationFile = open(fileStr, O_WRONLY | O_CREAT | O_TRUNC, OUT_FILE_PERMISSION);
  if (destinationFile == -1)
    reportErrorAndFlushStdOut(OPEN_WRITE_FILE_ERROR_MSG_LABEL);
  return destinationFile;
}

int hasNoMoreArgumentsAfterCurrent(char *nextVal)
{
  if (!nextVal)
  {
    reportErrorAndFlushStdOut(MISSING_PARAM_ERROR_MSG_LABEL);
    return TRUE;
  }
  return FALSE;
}

// int handleRedirectFlow(char **args, int pos, )
// {
//   char *nextVal = args[pos + 1];
//   if (hasNoMoreArgumentsAfterCurrent(nextVal))
//     return 1;
//   args[pos] = NULL;
//   int filePtr = openFileForReading(nextVal);
//   if (filePtr == -1)
//     return 1;
//   return dup2(filePtr, 0);
// }
