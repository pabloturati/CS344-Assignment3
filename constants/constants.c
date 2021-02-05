#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "constants.h"

const char *TOO_FEW_ARGUMENT_MSG = "smallsh: Unable to execute, too few arguments\n";

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
