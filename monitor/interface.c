#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "array.h"
#include "common.h"
#include "errors.h"
#include "requests.h"

int handle_request(Request request, Running_Programs *running_programs) {
    switch (request.type) {
    case SINGLE_EXEC:
    case PIPELINE_EXEC: {
        // The pipe is open
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);

        // The permission for the program to run is sent
        add_running_program(running_programs, request);
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
        int request_position =
            find_running_program(running_programs, request.child_pid);
        if (request_position == -1) {
            print_error(REQUEST_NOT_FOUND);
        }
        Request init_request = running_programs->array[request_position];
        struct timeval init_time = init_request.time;
        struct timeval current_time;
        gettimeofday(&current_time, NULL);
        struct timeval running_time =
            calculate_time_taken(init_time, current_time);
        write(output_pipe, &running_time, sizeof(struct timeval));
        delete_running_program(running_programs, request_position);

        // The pipe is closed
        close(output_pipe);
        free(output_pipe_string);
    } break;
    case STATUS: {
        // The output pipe is open
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);
        // The running message for each program is created
        Request program;
        char message[100];
        int bytes_needed;
        int current_program_number = running_programs->current_size;
        struct timeval current_time, execution_time;
        long time_ms;
        gettimeofday(&current_time, NULL);
        while (current_program_number) {
            program = running_programs->array[current_program_number - 1];
            // The time taken is calculated
            execution_time = calculate_time_taken(program.time, current_time);
            time_ms = time_struct_to_long(execution_time);

            // The message is created
            bytes_needed =
                snprintf(message, 99, "%d %s %ld ms", program.child_pid,
                         program.program_name, time_ms);
            if (bytes_needed <= 99) {
                message[bytes_needed] = '\n';
            } else {
                message[98] = '\n';
            }
            // The message is sent to the client
            write(output_pipe, message, sizeof(char) * 100);

            current_program_number--;
        }
        // An empty string is sent to signal all the programs were sent
        write(output_pipe, "", sizeof(char));
        close(output_pipe);
    } break;
    default:
        break;
    }
    return 0;
}