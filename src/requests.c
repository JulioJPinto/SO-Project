#include <string.h>
#include <sys/types.h>

#include "common.h"
#include "requests.h"

Request new_execute_request(pid_t requesting_pid, pid_t child_pid,
                            char *program_name) {
    Request new_request;
    new_request.type = SINGLE_EXEC;
    new_request.requesting_pid = requesting_pid;
    new_request.child_pid = child_pid;
    strncpy(new_request.program_name, program_name, NAME_MAX - 1);
    new_request.program_name[NAME_MAX - 1] = '\0';
    struct timeval current_time;
    if (gettimeofday(&current_time, NULL) == 0) {
        new_request.time = current_time;
    }
    return new_request;
}
Request new_pipeline_request(pid_t requesting_pid, pid_t child_pid,
                             char *program_name) {
    Request new_request;
    new_request.type = PIPELINE_EXEC;
    new_request.requesting_pid = requesting_pid;
    new_request.child_pid = child_pid;
    strncpy(new_request.program_name, program_name, NAME_MAX - 1);
    new_request.program_name[NAME_MAX - 1] = '\0';
    struct timeval current_time;
    if (gettimeofday(&current_time, NULL) == 0) {
        new_request.time = current_time;
    }
    return new_request;
}

Request finished_execution_request(pid_t requesting_pid, pid_t child_pid) {
    Request new_request;
    new_request.type = FINISHED_EXEC;
    new_request.requesting_pid = requesting_pid;
    new_request.child_pid = child_pid;
    struct timeval current_time;
    if (gettimeofday(&current_time, NULL) == 0) {
        new_request.time = current_time;
    }
    return new_request;
}

Request new_status_request(pid_t requesting_pid) {
    Request new_request;
    new_request.type = STATUS;
    new_request.requesting_pid = requesting_pid;
    return new_request;
}

Request new_stats_time_request(pid_t requesting_pid, char **pids,
                               int pids_number) {
    Request new_request;
    new_request.type = STATS_TIME;
    new_request.requesting_pid = requesting_pid;

    // The program name field will be used to carry the relevant pids
    strncpy(new_request.program_name, pids[0], NAME_MAX - 1);
    for (int i = 1; i < pids_number; i++) {
        strncat(new_request.program_name, " ",
                NAME_MAX - strlen(new_request.program_name) - 1);
        strncat(new_request.program_name, pids[i],
                NAME_MAX - strlen(new_request.program_name) - 1);
    }

    return new_request;
}

Request new_stats_command_request(pid_t requesting_pid, char *command,
                                  char **pids, int pids_number) {
    Request new_request;
    new_request.type = STATS_COMMAND;
    new_request.requesting_pid = requesting_pid;

    // The program name field will be used to carry the relevant pids
    strncpy(new_request.program_name, command, NAME_MAX - 1);
    for (int i = 0; i < pids_number; i++) {
        strncat(new_request.program_name, " ",
                NAME_MAX - strlen(new_request.program_name) - 1);
        strncat(new_request.program_name, pids[i],
                NAME_MAX - strlen(new_request.program_name) - 1);
    }

    return new_request;
}

Request new_stats_uniq_request(pid_t requesting_pid, char **pids,
                                int pids_number) {
    
    Request new_request = { STATS_UNIQ, requesting_pid};

    strncpy(new_request.program_name, pids[0], NAME_MAX - 1);
    for (int i = 1; i < pids_number; i++) {
        strncat(new_request.program_name, " ",
                NAME_MAX - strlen(new_request.program_name) - 1);
        strncat(new_request.program_name, pids[i],
                NAME_MAX - strlen(new_request.program_name) - 1);
    }

    return new_request;
}