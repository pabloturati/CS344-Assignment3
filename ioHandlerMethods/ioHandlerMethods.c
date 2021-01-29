#include <stdio.h>
#include <stdlib.h>
#include "../constants/constants.h"

/* 
Prompts user for command and returns a variable length string with the user input
Input: optionPromptMessage (string), menuList (string)
Output: option (integer)
Reference: parts of code adapted from https://brennan.io/2015/01/16/write-a-shell-in-c/  "Reading a line"
*/
char *promptForCommand()
{
  int currMax = COMMAND_MEMORY_SIZE;
  char *buffer = (char *)calloc(currMax, sizeof(char)); //
  int position = 0;
  int currChar;

  printf("%s", promptStart);

  while (TRUE)
  {
    currChar = getchar();

    // Checks for end-of-file or new line to return the final string
    if (currChar == EOF || currChar == '\n')
    {
      buffer[position] = '\0';
      return buffer;
    }

    // Assign a character and increase size
    buffer[position] = currChar;
    ++position;

    // If reached the max size, increase the memory allocation
    if (currMax >= position)
    {
      currMax += COMMAND_MEMORY_SIZE;
      buffer = realloc(buffer, currMax);
    }
  }
}