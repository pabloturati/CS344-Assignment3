#ifndef IOHANDLERMETHODS_H
#define IOHANDLERMETHODS_H

char *readCommandline();
char **splitInputCommand(char *);
char **requestAndTokenizeInput();
char *expandProcessVar(char *, int);

#endif