#ifndef COMMON_H
#define COMMON_H
#include <sys/time.h>
#include <sys/types.h>

#define NAME_MAX 30
#define REQUEST_PIPE_PATH "request_pipe"

typedef struct request{
    int type;
    pid_t PID;
    char program_name[NAME_MAX];
    struct timeval time;
} Request;
#endif