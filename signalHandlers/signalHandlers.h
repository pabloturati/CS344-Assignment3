#ifndef SIGNALHANDLERS_H
#define SIGNALHANDLERS_H

void handleSIGINT(int);
void setIgnoreSIGINT();
void setHandleSIGINT();

void handleSIGTSTP(int);
void setIgnoreSIGTSTP();
void setHandleSIGTSTP();

void handleSIGCHLD(int);
void setIgnoreSIGCHLD();
void setHandleSIGCHLD();

#endif
