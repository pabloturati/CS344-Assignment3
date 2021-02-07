#ifndef IOHANDLERMETHODS_H
#define IOHANDLERMETHODS_H
#include "../constants/constants.h"

char *readCommandline();
struct ShCommand *handleSplitInputCommand(char *);
int parseStringLineToCommand(char **, struct ShCommand *);
int parseLineToTokenArr(char *, char **);
struct ShCommand *requestAndProcessCommand();
char *expandProcessVar(char *, int);

#endif