#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants/constants.h"
#include "ioHandlerMethods/ioHandlerMethods.h"
#include "builtinFunctions/builtinFunctions.h"
#include "subProcessHandlers/subProcessHandlers.h"
#include "subProcessHandlers/subProcessHandlers.h"
#include "signalHandlers/signalHandlers.h"

int main()
{
  initilizeSignalIgnoreHandlers();
  do
  {
    struct ShCommand *currCommand = requestAndProcessCommand();
    fflush(stdout);
    setStatus(executeCommand(currCommand));
    free(currCommand);
  } while (TRUE);
  return 0;
}