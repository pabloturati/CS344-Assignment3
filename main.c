#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants/constants.h"
#include "ioHandlerMethods/ioHandlerMethods.h"

int main()
{
  do
  {
    char **tokens = requestAndTokenizeInput();

    free(tokens);
  } while (TRUE);
  return 0;
}