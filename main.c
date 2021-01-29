#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants/constants.h"
#include "ioHandlerMethods/ioHandlerMethods.h"

int main()
{
  char *inputLine;

  do
  {
    inputLine = promptForCommand();
    printf("variable length command is: %s\n\n", inputLine);
    char **tokens = splitInputCommand(inputLine);
    free(inputLine);
    int i = 0;
    char *curr = tokens[i];
    while (curr != NULL)
    {
      printf("Token %d is %s\n", i, curr);
      ++i;
      curr = tokens[i];
    }
    free(tokens);

  } while (TRUE);

  // Interact with user in main menu

  return 0;
}