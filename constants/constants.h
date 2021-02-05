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

const char *TOO_FEW_ARGUMENT_MSG;
void setStatus(int);
int getStatus();

#endif