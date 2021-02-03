#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

const char *TOO_FEW_ARGUMENT_MSG = "smallsh: Unable to execute, too few arguments\n";

static int status = 0;

int getStatus()
{
  return status;
}

void setStatus(int newStatus)
{
  status = newStatus;
}