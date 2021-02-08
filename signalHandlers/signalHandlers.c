#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "../constants/constants.h"

void initilizeSignalIgnoreHandlers()
{
  struct sigaction ignoreAction = {0};
  ignoreAction.sa_handler = SIG_IGN;
  sigaction(SIGINT, &ignoreAction, NULL);
}

void ignoreSIGTSTP()
{
  struct sigaction ignoreAction = {0};
  ignoreAction.sa_handler = SIG_IGN;
  sigaction(SIGTSTP, &ignoreAction, NULL);
}

void handle_SIGINT(int signo)
{
  write(STDOUT_FILENO, PROCESS_TERMINATION_BY_SIGNAL_MSG, 25);
}

void initializeChildSignalHandlers()
{
  struct sigaction SIGINT_action = {0};
  SIGINT_action.sa_handler = handle_SIGINT;
  sigfillset(&SIGINT_action.sa_mask);
  SIGINT_action.sa_flags = 0;
  sigaction(SIGINT, &SIGINT_action, NULL);
}