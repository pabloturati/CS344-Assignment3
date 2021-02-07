#ifndef SUBPROCESSHANDLERS_H
#define SUBPROCESSHANDLERS_H

int launchSubProcess(struct ShCommand *);
int executeCommand(struct ShCommand *);
int adjustProcessStreams(struct ShCommand *);

#endif