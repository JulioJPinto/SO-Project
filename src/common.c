#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "common.h"

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

char *total_exec_time_msg(long ms) {
    char ms_string[100];
    int digits_written = sprintf(ms_string, "%ld ", ms);

    char *time_msg = malloc(sizeof(char) * (28 + digits_written));
    strcpy(time_msg, "Total execution time is ");
    strcat(time_msg, ms_string);
    strcat(time_msg, "ms\n");
    return time_msg;
}

char *output_pipe_by_pid(pid_t pid) {
    char PID_string[MAX_PID_LENGTH + 1];
    int bytes_written = sprintf(PID_string, "%d", pid);

    char *output_pipe_string = malloc(sizeof(char) * (bytes_written + 8));
    strcpy(output_pipe_string, "output_");
    strcat(output_pipe_string, PID_string);
    return output_pipe_string;
}

// Calculates the time difference between two timestamps
struct timeval calculate_time_taken(struct timeval init_time,
                                    struct timeval finish_time) {
    struct timeval time_taken;
    time_taken.tv_sec = finish_time.tv_sec - init_time.tv_sec;
    time_taken.tv_usec = finish_time.tv_usec - init_time.tv_usec;
    return time_taken;
}