#ifndef SUBPROCESSHANDLERS_H
#define SUBPROCESSHANDLERS_H

int launchSubProcess(char **);
int executeCommand(char **);
char *expandProcessVar(char *, int);
int adjustProcessStreams(char **);

#endif