#ifndef SIGNALHANDLERS_H
#define SIGNALHANDLERS_H

void initilizeSignalIgnoreHandlers();
void initializeChildSignalHandlers();
void handle_SIGINT(int);

#endif
