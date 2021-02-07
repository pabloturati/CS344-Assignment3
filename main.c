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
    struct ShCommand *currCommand = requestAndProcessCommand();
    printStructure(currCommand);
    setStatus(executeCommand(currCommand));
    free(currCommand);
  } while (TRUE);
  return 0;
}