#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "errors.h"
#include "parser.h"
#include "requests.h"

int single_execute(char *command, char *output_pipe_string) {

    // The command to be executed is parsed
    char **parsed_command = parse_command(command);

    /* Creating an internal pipe to give the program provided
       as argument (child process) permission to run*/
    int internal_pipe[2];
    pipe(internal_pipe);
    pid_t child_pid = fork();
    if (child_pid == 0) {
        close(internal_pipe[1]);
        int run_permission;
        read(internal_pipe[0], &run_permission, sizeof(int));
        if (run_permission) {
            execvp(parsed_command[0], parsed_command);
        } else {
            _exit(1);
        }
    }
    close(internal_pipe[0]);

    // Opens the input and output pipes
    int request_pipe = open(REQUEST_PIPE_PATH, O_WRONLY);

    // Sends to the server the information that the program is running
    pid_t requesting_pid = getpid();
    Request new_request =
        new_execute_request(requesting_pid, child_pid, parsed_command[0]);
    write(request_pipe, &new_request, sizeof(Request));

    // Waiting for permission from the server for the program to run
    int output_pipe = open(output_pipe_string, O_RDONLY);
    int run_permission;
    read(output_pipe, &run_permission, sizeof(int));
    write(internal_pipe[1], &run_permission, sizeof(int));

    // Writing "Running PID" message
    char *running_msg = pid_running_msg(child_pid);
    write(STDOUT_FILENO, running_msg, sizeof(char) * (14 + MAX_PID_LENGTH));
    free(running_msg);

    // The server is informed when the program has finished running
    int status;
    wait(&status);
    Request finish_request =
        finished_execution_request(requesting_pid, child_pid);
    write(request_pipe, &finish_request, sizeof(Request));

    // The program execution time is gathered from the server
    struct timeval execution_time;
    int bytes_read = 0;
    while (!bytes_read) {
        bytes_read = read(output_pipe, &execution_time, sizeof(struct timeval));
    }

    // The program execution time is printed to the user
    char *time_msg = time_taken_msg(execution_time);
    write(STDOUT_FILENO, time_msg, sizeof(char) * strlen(time_msg));
    free(time_msg);

    close(request_pipe);
    close(output_pipe);
    free(parsed_command);
    return 0;
}