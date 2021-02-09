#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include "../constants/constants.h"

/*********** Shared hander methods ***********/

/*********** SIGINT Handlers and methods ***********/

/*
SIGINT callback
Input: signal number (integer)
Output: void
*/
void handleSIGINT(int signo)
{
  writeMessageWithSignalNumber(PROCESS_TERMINATION_BY_SIGNAL_MSG, signo);
}

/*
Configures SIGINT to be ignored
Input: void
Output: void
Adapted from class example 5_3_siguser.c
*/
void setIgnoreSIGINT()
{
  struct sigaction ignoreAction = {0};
  ignoreAction.sa_handler = SIG_IGN;
  sigaction(SIGINT, &ignoreAction, NULL);
}
/*
Configures a callback for SIGINT
Input: void
Output: void
Adapted from class example 5_3_siguser.c
*/
void setHandleSIGINT()
{
  struct sigaction SIGINT_action = {0};
  SIGINT_action.sa_handler = handleSIGINT;
  sigfillset(&SIGINT_action.sa_mask);
  SIGINT_action.sa_flags = 0;
  sigaction(SIGINT, &SIGINT_action, NULL);
}

/*********** SIGTSTP Handlers and methods ***********/

/*
SIGSTP callback
Input: signal number (integer)
Output: void
*/
void handleSIGTSTP(int signo)
{
  if (!getForegroundModeVal())
    write(STDOUT_FILENO, SIGSTOP_ENTER_FOREGROUND_ONLY_MSG, 49);
  else
    write(STDOUT_FILENO, SIGSTOP_EXIT_FOREGROUND_ONLY_MSG, 29);
  toggleForegroundMode();
}

/*
Configures a callback for SIGTSTP
Input: void
Output: void
Adapted from class example 5_3_siguser.c
*/
void setIgnoreSIGTSTP()
{
  struct sigaction ignoreAction = {0};
  ignoreAction.sa_handler = SIG_IGN;
  sigaction(SIGTSTP, &ignoreAction, NULL);
}

/*
Configures a callback for SIGTSTP
Input: void
Output: void
Adapted from class example 5_3_siguser.c
*/
void setHandleSIGTSTP()
{
  struct sigaction SIGTSTP_action = {0}, ignore_action = {0};
  SIGTSTP_action.sa_handler = handleSIGTSTP;
  sigemptyset(&SIGTSTP_action.sa_mask);
  SIGTSTP_action.sa_flags = SA_RESTART;
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}