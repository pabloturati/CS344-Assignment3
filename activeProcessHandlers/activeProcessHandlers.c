#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../constants/constants.h"
#include "activeProcessHandlers.h"

static int *activeProcessesArr;
static int activeProcessesSize;

/*
Returns 1 if the list contains a certain process
Input: newProcessId (integer)
Output: returns 0 if successful, 1 if failure
*/
int initializeActiveProcessArr()
{
  activeProcessesArr = (pid_t *)malloc(MAX_ACTIVE_PROCESSES * sizeof(pid_t));
  if (activeProcessesArr == NULL)
  {
    return EXIT_FAILURE;
  }
  activeProcessesSize = 0;
  return EXIT_SUCCESS;
}

void freeProcessArray()
{
  free(activeProcessesArr);
}

/*
Traverses process array to find index of process
Input: newProcessId (integer)
Output: -1 if processId not found in list, otherwise index of the process
*/
int getProcessIdListIndex(pid_t processId)
{
  // return activeProcesses[activeProcessesSize];
  for (int i = 0; i < activeProcessesSize; i++)
  {
    if (activeProcessesArr[i] == processId)
      return i;
  }
  return -1;
}
/*
Adds a new process to the pending list
Input: newProcessId (integer)
Output: void
*/
void registerBackgroundProcess(pid_t processId)
{
  activeProcessesArr[activeProcessesSize] = processId;
  ++activeProcessesSize;
}

/*
Add to active process list
Input: new current value of 'status' global variable (int)
Output: returns -1 for not found else returns deleted Value
*/
int removeProcessFromList(pid_t processId)
{
  int pos = getProcessIdListIndex(processId);
  if (pos)
    return -1;
  pid_t deletedValue = activeProcessesArr[pos];
  // Unshift array from deleted value
  for (int i = pos; i < activeProcessesSize - 1; i++)
  {
    activeProcessesArr[i] = activeProcessesArr[i + 1];
  }
  --activeProcessesSize;
  return deletedValue;
}

/*
Add to active process list
Input: new current value of 'status' global variable (int)
Output: returns -1 for not found else returns deleted Value
Reference: Adapted in part from code in class example 4_2_waitpid_exit.c
*/
void checkForBackgroundProcessCompletions()
{
  for (int i = 0; i < activeProcessesSize; i++)
  {
    int childStatus;
    // Run waitpid on each item of the array (not waiting)
    int terminationStatus = waitpid(activeProcessesArr[i], &childStatus, WNOHANG);
    // If process ended
    if (terminationStatus != 0)
    { //process ended successfully
      if (WIFEXITED(childStatus))
        printf(CHILD_PROCESS_TERMINATION_SUCCESS_MSG, activeProcessesArr[i], WEXITSTATUS(childStatus));
      else
      { //process ended abnormally
        printf(CHILD_PROCESS_TERMINATION_ERROR_MSG, activeProcessesArr[i], WTERMSIG(childStatus));
      }
      removeProcessFromList(activeProcessesArr[i]);
    }
  }
}