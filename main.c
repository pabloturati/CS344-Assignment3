#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants/constants.h"
#include "ioHandlerMethods/ioHandlerMethods.h"
#include "builtinFunctions/builtinFunctions.h"
#include "subProcessHandlers/subProcessHandlers.h"
#include "subProcessHandlers/subProcessHandlers.h"
#include "signalHandlers/signalHandlers.h"
#include "activeProcessHandlers/activeProcessHandlers.h"

void initializeApp()
{
  // Sets SIGINT to be ignored.
  setIgnoreSIGINT();
  // Initializes global background process array to empty
  initializeActiveProcessArr();
}

int main()
{
  initializeApp();
  do
  {
    checkForBackgroundProcessCompletions();
    struct ShCommand *currCommand = requestAndProcessCommand();
    fflush(stdout);
    setStatus(executeCommand(currCommand));
    free(currCommand);
  } while (TRUE);
  return 0;
}