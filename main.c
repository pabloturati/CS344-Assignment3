#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants/constants.h"
#include "ioHandlerMethods/ioHandlerMethods.h"

int main()
{
  char *command;

  do
  {
    command = promptForCommand();
    printf("variable length command is: %s\n", command);

  } while (TRUE);

  // Interact with user in main menu

  return 0;
}