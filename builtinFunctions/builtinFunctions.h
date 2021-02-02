#ifndef BUILTINFUNCTIONS_H
#define BUILTINFUNCTIONS_H

int cdCommand(char **args);
int exitCommand(char **args);
int helpCommand(char **args);
int commandFuncArrLength();
int executeCommand(char **args);
int isAComment(char *args);

// extern char *commandIdentifiers[];
extern char *SHELL_COMMANDS[];
extern int (*shellCommandFunctions[])(char **);

#endif