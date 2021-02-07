#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../constants/constants.h"
#include "ioHandlerMethods.h"

/* 
Takes in a raw string and divides into tokens. Returns array of tokens. 
Input: inputLine (string *)
Output: tokens (array of strings);
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Parsing a line"
*/
char **splitInputCommand(char *inputLine)
{
  // Assumes initial size of array of strings (will adjust if necessary)
  int currMax = TOKEN_ARR_SIZE;
  int pos = 0;

  // Creates pointer to array of string pointer
  char **tokens = malloc(currMax * sizeof(char *));
  assert(tokens != NULL);

  // Variable to store individual tokens
  char *curToken;

  // Gets first token. Returns NULL if none.
  curToken = strtok(inputLine, TOKEN_DELIMETERS);

  // Gets current process Id
  int processId = getShellProcessId();

  while (curToken != NULL)
  {
    tokens[pos] = expandProcessVar(curToken, processId);
    ++pos;

    // If maximum number of tokens are passed, increase memory allocation
    if (currMax >= pos)
    {
      currMax += TOKEN_ARR_SIZE;
      tokens = realloc(tokens, currMax * sizeof(char *));
      assert(tokens != NULL);
    }

    curToken = strtok(NULL, TOKEN_DELIMETERS);
  }

  // Adds null at last position
  tokens[pos] = NULL;
  return tokens;
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
Handles input prompt request. Returns array of separate commands. 
Input: void
Output: tokens (array of strings);
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Reading a line"
*/
char **requestAndTokenizeInput()
{
  printf("%s", PROMPT_STR);
  char *inputLine = readCommandline();
  char **tokens = splitInputCommand(inputLine);
  free(inputLine);
  return tokens;
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