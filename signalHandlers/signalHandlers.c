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
  write(STDOUT_FILENO, SIGSTOP_FOREGROUND_ONLY_MSG, 49);
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
  struct sigaction SIGTSTP_action = {0};
  SIGTSTP_action.sa_handler = handleSIGTSTP;
  sigfillset(&SIGTSTP_action.sa_mask);
  SIGTSTP_action.sa_flags = 0;
  sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}

/*********** SIGCHLD Handlers and methods ***********/

// /*
// SIGCHLD callback
// Input: void
// Output: void
// Reference: Adapted in parts from http://www.microhowto.info/howto/reap_zombie_processes_using_a_sigchld_handler.html
// */
// void handleSIGCHLD(int signo)
// {

//   int saved_errno = errno;
//   write(STDOUT_FILENO, SIGCHLD_TERMINATION_MSG_PART_1, 17);
//   // PID here
//   write(STDOUT_FILENO, SIGCHLD_TERMINATION_MSG_PART_2, 22);
//   while (waitpid((pid_t)(-1), 0, WNOHANG) > 0)
//   {
//   }
//   errno = saved_errno;
// }

// /*
// Configures a callback for SIGCHLD
// Input: void
// Output: void
// Adapted from class example 5_3_siguser.c
// */
// void setIgnoreSIGCHLD()
// {
//   struct sigaction ignoreAction = {0};
//   ignoreAction.sa_handler = SIG_IGN;
//   sigaction(SIGCHLD, &ignoreAction, NULL);
// }

// /*
// Configures a callback for SIGCHLD. To be used to reap background child processes
// Input: void
// Output: void
// Reference: Adapted in parts from https://www.andrew.cmu.edu/course/15-310/applications/homework/homework4/signal_example2.html
// */
// void setHandleSIGCHLD()
// {
//   struct sigaction SIGCHILD_action;
//   SIGCHILD_action.sa_handler = handleSIGCHLD;
//   sigemptyset(&SIGCHILD_action.sa_mask);
//   SIGCHILD_action.sa_flags = 1;
//   if (sigaction(SIGCHLD, &SIGCHILD_action, 0) == -1)
//   {
//     perror(0);
//     exit(1);
//   }
// }
