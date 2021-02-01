#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/* 
Launches a child process using the passed arguments  
Input: args (list of strings)
Output: 0 in case of success, otherwise error code > 0
Reference: parts of code adapted from example 4_2_execv_fork_ls
*/

int launchSubProcess(char **args)
{
  int childProcessStatus;
  pid_t spawnPid = fork();

  switch (spawnPid)
  {
  case -1: // In case of Fork error
    perror("fork()\n");
    return 1;
  case 0: // Child process
    printf("Child(%d) is running command\n", getpid());
    execv(args[0], args);
    // exec only returns if there is an error
    perror("exec_error");
    return 2;
  default:
    // Parent execution. Waits for child to complete.
    spawnPid = waitpid(spawnPid, &childProcessStatus, 0);
    printf("Parent(%d): Child(%d) terminated", getpid(), spawnPid);
  }
  return 0;
}