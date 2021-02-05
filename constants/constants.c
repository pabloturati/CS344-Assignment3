#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "constants.h"

const char *TOO_FEW_ARGUMENT_MSG = "smallsh: Unable to execute, too few arguments\n";
const char *EXEC_ERROR_MSG_LABEL = "exec_error()";
const char *MISSING_PARAM_ERROR_MSG_LABEL = "missing_param";
const char *OPEN_READ_FILE_ERROR_MSG_LABEL = "source open()";
const char *OPEN_WRITE_FILE_ERROR_MSG_LABEL = "target open()";
const char *REDIRECT_ERROR_MSG_LABEL = "target dup2()";

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

/*
Prints error message and flushes stdout
Input: void
Output: Prints message to stdout
*/
void reportErrorAndFlushStdOut(const char *errorMsg)
{
  perror(errorMsg);
  fflush(stdout);
}

/*
Opens a file for reading and returns file pointer.
Input: fileStr (string)
Output: file descriptor (integer)
*/
int openFileForReading(char *fileStr)
{
  int sourceFileDescriptor = open(fileStr, O_RDONLY);
  if (sourceFileDescriptor == -1)
    reportErrorAndFlushStdOut(OPEN_READ_FILE_ERROR_MSG_LABEL);
  return sourceFileDescriptor;
}

/*
Opens a file for writing and returns file pointer.
Input: fileStr (string)
Output: file descriptor (integer)
*/
int openFileForWriting(char *fileStr)
{
  int destinationFileDescriptor = open(fileStr, O_WRONLY | O_CREAT | O_TRUNC, OUT_FILE_PERMISSION);
  if (destinationFileDescriptor == -1)
    reportErrorAndFlushStdOut(OPEN_WRITE_FILE_ERROR_MSG_LABEL);
  return destinationFileDescriptor;
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

/*
Function to open a file for reading or writing and redirect.
process flow.
Input:  args - List of commands (by reference)
        pos - Current position (integer)
        operationType - 0 for input / 1 for output (integer)
        openFileHandler (function) - Function to handle the file opening (i.e. reading or writing)
Output: currentProcessId()
*/
int handleRedirectFlow(char **args, int pos, int operationType, int (*openFileHandler)(char *))
{
  char *nextVal = args[pos + 1];
  if (hasNoMoreArgumentsAfterCurrent(nextVal))
    return 1;
  args[pos] = NULL;
  int filePtr = openFileHandler(nextVal);
  if (filePtr == -1)
    return 1;
  int result = dup2(filePtr, operationType);
  if (result == -1)
  {
    perror(REDIRECT_ERROR_MSG_LABEL);
    return 2;
  }
  return 0;
}
