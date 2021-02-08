#ifndef BUILTINFUNCTIONS_H
#define BUILTINFUNCTIONS_H

int cdCommand(char **args);
int exitCommand(char **args);
int statusCommand(char **args);
int commandFuncArrLength();
int isAComment(char **args);
int isAnEmptyLineOrComment(char **args);

extern char *SHELL_COMMANDS[];
extern int (*shellCommandFunctions[])(char **);

#endif