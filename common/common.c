#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// Not sure this is the best place for this function, maybe a messages.c?
char *pid_running_msg(pid_t PID) {
    char *running_msg = malloc(sizeof(char) * (14 + MAX_PID_LENGTH));
    sprintf(running_msg, "Running PID %d\n", PID);
    return running_msg;
}

long time_struct_to_long(struct timeval time) {
    long ms = time.tv_sec * 1000;
    ms += (time.tv_usec / 1000);
    return ms;
}

char *time_taken_msg(struct timeval time) {
    long ms = time_struct_to_long(time);
    char ms_string[100];
    int digits_written = sprintf(ms_string, "%ld ", ms);

    char *time_msg = malloc(sizeof(char) * (14 + digits_written));
    strcpy(time_msg, "\nEnded in ");
    strcat(time_msg, ms_string);
    strcat(time_msg, "ms\n");
    return time_msg;
}

#define PIPE_PREFIX "output_"

char *get_pipe_name(pid_t pid) {
    char *pipe_name = malloc(sizeof(char) * (strlen(PIPE_PREFIX) + MAX_PID_LENGTH));
    sprintf(pipe_name, "%s%d", PIPE_PREFIX, pid);
    return pipe_name;
}

// Calculates the time difference between two timestamps
struct timeval calculate_time_taken(struct timeval init_time,
                                    struct timeval finish_time) {
    struct timeval time_taken;
    time_taken.tv_sec = finish_time.tv_sec - init_time.tv_sec;
    time_taken.tv_usec = finish_time.tv_usec - init_time.tv_usec;
    return time_taken;
}