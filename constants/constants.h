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
#define RED_OUT_SYM ">"
#define RED_IN_SYM "<"
#define OUT_FILE_PERMISSION 0644
#define KILL_PROCESS_RETURN_VAL 2
#define HOME_ENV_VAR "HOME"
#define INPUT_OPERATION 0
#define OUTPUT_OPERATION 1

const char *TOO_FEW_ARGUMENT_MSG;
const char *EXEC_ERROR_MSG_LABEL;
const char *MISSING_PARAM_ERROR_MSG_LABEL;
const char *OPEN_READ_FILE_ERROR_MSG_LABEL;
const char *OPEN_WRITE_FILE_ERROR_MSG_LABEL;
const char *REDIRECT_ERROR_MSG_LABEL;

void setStatus(int);
int getStatus();
int killChildProcess();
int getShellProcessId();
void reportErrorAndFlushStdOut(const char *);
int openFileForReading(char *);
int hasNoMoreArgumentsAfterCurrent(char *);
int openFileForWriting(char *);
int handleRedirectFlow(char **args, int pos, int operationType, int (*)(char *));

#endif