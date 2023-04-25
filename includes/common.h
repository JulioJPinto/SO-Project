#ifndef COMMON_H
#define COMMON_H
#include <sys/time.h>
#include <sys/types.h>

#define MAX_PID_LENGTH 5
#define NAME_MAX 30
#define REQUEST_PIPE_PATH "request_pipe"

char *pid_running_msg(pid_t PID);

char *time_taken_msg(struct timeval time);

char *output_pipe_by_pid(pid_t pid);

#endif