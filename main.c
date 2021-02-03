#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants/constants.h"
#include "ioHandlerMethods/ioHandlerMethods.h"
#include "builtinFunctions/builtinFunctions.h"

int main()
{
  do
  {
    char **tokens = requestAndTokenizeInput();
    setStatus(executeCommand(tokens));
    free(tokens);
  } while (TRUE);
  return 0;
}