#ifndef COMMON_H
#define COMMON_H
#include <sys/time.h>
#include <sys/types.h>

#define MAX_PID_LENGTH 5
#define NAME_MAX 50
#define REQUEST_PIPE_PATH "request_pipe"

char *pid_running_msg(pid_t PID);

/*Converts the two fields in struct timeval into one integer in ms
  (ONLY USE FOR TIME INTERVALS, NOT TO DISPLAY TIME SINCE EPOCH)*/
long time_struct_to_long(struct timeval time);

char *time_taken_msg(struct timeval time);

char *total_exec_time_msg(long time);

char *output_pipe_by_pid(pid_t pid);

// Calculates the time difference between two timestamps
struct timeval calculate_time_taken(struct timeval init_time,
                                    struct timeval finish_time);

char *total_times_ran_msg(char *program, int result);

#endif
