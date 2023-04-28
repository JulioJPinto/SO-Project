#ifndef REQUESTS_H
#define REQUESTS_H

typedef enum {
    UNKNOWN,
    FINISHED_EXEC,
    SINGLE_EXEC,
    PIPELINE_EXEC,
    STATUS
} request_type;

#include "common.h"

typedef struct request {
    request_type type;
    pid_t requesting_pid;
    pid_t child_pid;
    char program_name[NAME_MAX];
    struct timeval time;
} Request;

Request new_execute_request(pid_t requesting_PID, pid_t child_PID,
                            char *program_name);

Request new_pipeline_request(pid_t requesting_pid, pid_t child_pid,
                             char *program_name);

Request finished_execution_request(pid_t requesting_PID, pid_t child_PID);

Request new_status_request(pid_t requesting_pid);

#endif