#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "errors.h"
#include "requests.h"

int handle_request(Request request, Request *running_programs) {
    switch (request.type) {
    case SINGLE_EXEC: {
        // The pipe is open
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);

        // The permission for the program to run is sent
        running_programs[request.child_pid] = request;
        int run_permission = 1;
        write(output_pipe, &run_permission, sizeof(int));

        // The pipe is closed
        close(output_pipe);
        free(output_pipe_string);
    } break;
    case FINISHED_EXEC: {
        // The pipe is open
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);

        // The time taken to execute the program is calculated
        struct timeval running_time;
        request = running_programs[request.child_pid];
        struct timeval init_time = request.time;
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        running_time.tv_sec = current_time.tv_sec - init_time.tv_sec;
        running_time.tv_usec = current_time.tv_usec - init_time.tv_usec;
        write(output_pipe, &running_time, sizeof(struct timeval));

        // The pipe is closed
        close(output_pipe);
        free(output_pipe_string);
    } break;
    default:
        break;
    }
    return 0;
}