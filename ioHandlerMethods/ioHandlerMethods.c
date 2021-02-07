#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../constants/constants.h"
#include "ioHandlerMethods.h"

/* 
Handles input prompt request. Returns array of separate commands. 
Input: void
Output: tokens (array of strings);
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Reading a line"
*/
struct ShCommand *requestAndProcessCommand()
{
  struct ShCommand *currCommand = (struct ShCommand *)malloc(sizeof(struct ShCommand));
  char **commandsArr = malloc(TOKEN_ARR_SIZE * sizeof(char *));

  int commandParseStatus = 1;
  while (commandParseStatus != 0)
  {
    resetCommandInstanceAndArray(currCommand, commandsArr);

    // Prompt user for command
    printf("%s", PROMPT_STR);
    char *inputLine = readCommandline();

    // Parse command string
    if (parseLineToTokenArr(inputLine, commandsArr) == 0)
    {
      commandParseStatus = parseStringLineToCommand(commandsArr, currCommand);
    }
    else
      perror(COMMAND_PARSE_ERROR_MSG);
  }
  return currCommand;
}

/* 
Takes array of raw instructions, expands process id, refactors commands and
returns an ordere ShCommand struct instance.
Input: divided array of raw commands (string array)
Output: ShCommand struct with ordered instructions
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Reading a line"
*/
int parseStringLineToCommand(char **commandsArr, struct ShCommand *currCommand)
{
  int processId = getShellProcessId();
  int i = 0;

  char *currToken = commandsArr[i];

  while (currToken != NULL)
  {
    if (isRedirectInputSymbol(currToken))
    {
      char *nextToken = commandsArr[i + 1];
      if (hasNoMoreArgumentsAfterCurrent(nextToken))
        return EXIT_FAILURE;
      currCommand->inRedirFile = nextToken;
      commandsArr[i] = NULL;
    }
    else if (isRedirectOutputSymbol(currToken))
    {
      char *nextToken = commandsArr[i + 1];
      if (hasNoMoreArgumentsAfterCurrent(nextToken))
        return EXIT_FAILURE;
      currCommand->outRedirFile = nextToken;
      commandsArr[i] = NULL;
    }
    else if (isRunProcessOnBackgroundSymbol(currToken))
      currCommand->isBackgroundProcess = TRUE;
    else
    {
      commandsArr[i] = expandProcessVar(commandsArr[i], processId);
    }
    ++i;
    currToken = commandsArr[i];
  }

  currCommand->path = commandsArr[0];
  currCommand->args = commandsArr;
  return executeCommandIT_SUCCESS;
}

/* 
Divides single string line command into array of individual tokens 
Input: user input line (string)
Output: divided array of raw commands (string array)
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Reading a line"
*/
int parseLineToTokenArr(char *inputLine, char **commandsArr)
{
  // Assumes initial size of array of strings (will adjust if necessary)
  int currMax = TOKEN_ARR_SIZE;

  if (commandsArr == NULL)
    return EXIT_FAILURE;

  // Gets first token. Returns NULL if none.
  char *curToken = strtok(inputLine, TOKEN_DELIMETERS);

  int pos = 0;
  while (curToken != NULL)
  {
    //Assigns current token to token array and expands any process variable
    commandsArr[pos] = curToken;
    ++pos;

    // If maximum number of tokens are passed, increase memory allocation
    if (currMax <= pos)
    {
      currMax += TOKEN_ARR_SIZE;
      commandsArr = realloc(commandsArr, currMax * sizeof(char *));
      if (commandsArr == NULL)
        return EXIT_FAILURE;
    }
    curToken = strtok(NULL, TOKEN_DELIMETERS);
  }

  // Adds null at last position
  commandsArr[pos] = NULL;
  return EXIT_SUCCESS;
}

/* 
Prompts user for command and returns a variable length string with the user input
Input: void
Output: string - Contains user input line
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Reading a line"
*/
char *readCommandline()
{
  // Assumes initial size of the user input (will adjust if necessary)
  int currMax = COMMAND_MEMORY_SIZE;
  char *buffer = (char *)calloc(currMax, sizeof(char));
  assert(buffer != NULL);

  int pos = 0;
  int currChar;

  while (TRUE)
  {
    currChar = getchar();

    // Checks for end-of-file or new line to return the final string
    if (currChar == EOF || currChar == '\n')
    {
      buffer[pos] = '\0';
      return buffer;
    }

    // Assign a character and increase size
    buffer[pos] = currChar;
    ++pos;

    // If reached the max size, increase the memory allocation
    if (currMax >= pos)
    {
      currMax += COMMAND_MEMORY_SIZE;
      buffer = realloc(buffer, currMax);
      assert(buffer != NULL);
    }
  }
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