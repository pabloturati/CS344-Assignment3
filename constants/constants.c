#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include "constants.h"

/*********** User interaction messages ***********/

const char *TOO_FEW_ARGUMENT_MSG = "smallsh: Unable to execute, too few arguments\n";
const char *EXEC_ERROR_MSG_LABEL = "exec_error()";
const char *MISSING_PARAM_ERROR_MSG_LABEL = "missing_param";
const char *OPEN_READ_FILE_ERROR_MSG_LABEL = "source open()";
const char *OPEN_WRITE_FILE_ERROR_MSG_LABEL = "target open()";
const char *REDIRECT_ERROR_MSG_LABEL = "target dup2()";
const char *COMMAND_PARSE_ERROR_MSG = "command_error()";
const char *FORK_ERROR_MSG = "fork()";
const char *BACKGROUND_PROCESS_ID_MSG = "background pid is %d\n";
const char *PROCESS_TERMINATION_BY_SIGNAL_MSG = "\nTerminated by signal "; //Length 23
const char *SIGSTOP_FOREGROUND_ONLY_MSG = "Entering foreground-only mode (& is now ignored)\n";
const char *CHILD_PROCESS_TERMINATION_SUCCESS_MSG = "Background pid %d is done: exit value %d\n";
const char *CHILD_PROCESS_TERMINATION_ERROR_MSG = "Background pid %d is done. Terminated abnormally: exit value %d\n";

/*********** Command Structure ***********/

void resetCommandInstanceAndArray(struct ShCommand *currCommand, char **commandsArr)
{
  // Reset struct values
  currCommand->path = NULL;
  currCommand->args = NULL;
  currCommand->outRedirFile = NULL;
  currCommand->inRedirFile = NULL;
  currCommand->isBackgroundProcess = FALSE;
}

/*********** Global variables and accessors ***********/

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

/*********** General helper methods ***********/

/*
Method to match strings.
Input: str1, str2 -> strings to compare
Output: returns TRUE (1) if strings are equal, else FALSE (0)
*/
int stringEquals(char *str1, char *str2)
{
  return strncmp(str1, str2, 1) == 0;
}

/* Methods to abstract symbol analysis. */
/* Bools. Return 1 for TRUE */
int isRedirectInputSymbol(char *commandStr)
{
  return stringEquals(REDIRECT_INPUT_SYMBOL, commandStr) && strlen(commandStr) == 1;
}
int isRedirectOutputSymbol(char *commandStr)
{
  return stringEquals(REDIRECT_OUTPUT_SYMBOL, commandStr) && strlen(commandStr) == 1;
}
int isRunProcessOnBackgroundSymbol(char *commandStr)
{
  return stringEquals(BACKGROUND_PROCESS_SYMBOL, commandStr) && strlen(commandStr) == 1;
}

/*********** Data Process helper methods ***********/

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

/*
Verifies if there is a next command value. 
Input: nextVal (string)
Output: If FALSE, prints error message and returns 0.
If TRUE, returns 1. 
*/
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
int handleRedirectFlow(char *filename, int operationType, int (*openFileHandler)(char *))
{
  int filePtr = openFileHandler(filename);
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

/*
Function to print termination message using reentrant method 
Input:  const char * message, signal number (integer)
        pos - Current position (integer)
        operationType - 0 for input / 1 for output (integer)
        openFileHandler (function) - Function to handle the file opening (i.e. reading or writing)
Output: void - prints message of termination using status
*/
char *digitArr[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
void writeMessageWithSignalNumber(const char *message, int signo)
{
  write(STDOUT_FILENO, message, 23);
  write(STDOUT_FILENO, digitArr[signo], 2);
  write(STDOUT_FILENO, "\n", 2);
}

// FIX Remove

void printStructure(struct ShCommand *currCommand)
{
  printf("path %s\n", currCommand->path);
  printf("outRedirFile %s\n", currCommand->outRedirFile);
  printf("inRedirFile %s\n", currCommand->inRedirFile);
  printf("isBackgroundProcess %d\n", currCommand->isBackgroundProcess);
  printStringArr(currCommand->args);
}

void printStringArr(char **strArr)
{
  int i = 0;
  printf("Content of args\n");
  while (strArr[i] != NULL)
  {
    printf("Args %d is %s\n", i, strArr[i]);
    ++i;
  }
}