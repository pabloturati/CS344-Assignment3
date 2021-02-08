#ifndef ACTIVEPROCESSHANDLER_H
#define ACTIVEPROCESSHANDLER_H

int initializeActiveProcessArr();
void checkForBackgroundProcessCompletions();
void freeProcessArray();
int getProcessIdListIndex(pid_t processId);
void registerBackgroundProcess(pid_t processId);

#endif