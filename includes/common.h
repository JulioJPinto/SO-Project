#ifndef COMMON_H
#define COMMON_H
#include <sys/time.h>
#include <sys/types.h>

#define MAX_PID_LENGTH 5
#define NAME_MAX 30
#define REQUEST_PIPE_PATH "request_pipe"

// Definition of errors
#define WRONG_NUM_ARGUMENTS -1
#define UNKNOWN_COMMAND -2
#define UNKNOWN_OPTION -3

// Commands definition
#define SINGLE_EXECUTE 1
#define CHAINED_EXECUTE 2

char *pid_running_msg(pid_t PID);
#endif