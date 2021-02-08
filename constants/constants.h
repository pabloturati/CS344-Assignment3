#ifndef CONSTANTS_H
#define CONSTANTS_H
#define TRUE 1
#define FALSE 0
#define COMMAND_MEMORY_SIZE 1024
#define TOKEN_ARR_SIZE 64
#define TOKEN_DELIMETERS " \t\r\n\a"
#define PROMPT_STR ": "
#define COMMENT_SIMBOL "#"
#define VAR_EXPANSION_BUFF_SIZE 1024
#define PROCESS_ID_VARIABLE "$$"
#define REDIRECT_OUTPUT_SYMBOL ">"
#define REDIRECT_INPUT_SYMBOL "<"
#define BACKGROUND_PROCESS_SYMBOL "&"
#define OUT_FILE_PERMISSION 0644
#define KILL_PROCESS_RETURN_VAL 2
#define HOME_ENV_VAR "HOME"
#define INPUT_OPERATION 0
#define OUTPUT_OPERATION 1
#define NULL_REDIRECT_PATH "/dev/null"

const char *TOO_FEW_ARGUMENT_MSG;
const char *EXEC_ERROR_MSG_LABEL;
const char *MISSING_PARAM_ERROR_MSG_LABEL;
const char *OPEN_READ_FILE_ERROR_MSG_LABEL;
const char *OPEN_WRITE_FILE_ERROR_MSG_LABEL;
const char *REDIRECT_ERROR_MSG_LABEL;
const char *COMMAND_PARSE_ERROR_MSG;
const char *FORK_ERROR_MSG;
const char *BACKGROUND_PROCESS_ID_MSG;
const char *PROCESS_TERMINATION_BY_SIGNAL_MSG;
const char *SIGSTOP_FOREGROUND_ONLY_MSG;

struct ShCommand
{
  char *path;
  char **args;
  char *outRedirFile;
  char *inRedirFile;
  int isBackgroundProcess;
};

void resetCommandInstanceAndArray(struct ShCommand *, char **);
void setStatus(int);
int getStatus();
int killChildProcess();
int getShellProcessId();
void reportErrorAndFlushStdOut(const char *);
int openFileForReading(char *);
int hasNoMoreArgumentsAfterCurrent(char *);
int openFileForWriting(char *);
int handleRedirectFlow(char *, int, int (*)(char *));

int stringEquals(char *, char *);
int isRedirectInputSymbol(char *);
int isRedirectOutputSymbol(char *);
int isRunProcessOnBackgroundSymbol(char *);

// Debugger functions
void printStructure(struct ShCommand *);
void printStringArr(char **);

#endif