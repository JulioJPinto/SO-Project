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

// Handles instances of excution of a single program
int single_execute(char *command, char *output_pipe_string) {

    // The command to be executed is parsed
    char **parsed_command = parse_command(command);

    /* Creating an internal pipe to give the program provided
       as argument (child process) permission to run*/
    int internal_pipe[2];
    pipe(internal_pipe);

    // Forking the child process that will execute the program
    pid_t child_pid = fork();
    if (child_pid == 0) {
        close(internal_pipe[1]);
        // Waiting for permission to start running
        int run_permission;
        read(internal_pipe[0], &run_permission, sizeof(int));
        if (run_permission) {
            // Running the program
            execvp(parsed_command[0], parsed_command);
        } else {
            _exit(1);
        }
    }
    close(internal_pipe[0]);

    // Opens the input pipe
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

    // Closing resources
    close(request_pipe);
    close(output_pipe);
    free(parsed_command);
    return 0;
}

// Handles instances of excution of a pipeline of programs
int pipeline_execute(char *commands, char *output_pipe_string) {

    int input_pipe = open(REQUEST_PIPE_PATH, O_WRONLY);

    // The command to be executed is parsed
    char ***parsed_commands = parse_pipeline(commands);

    // The program names are added
    char program_name[NAME_MAX];
    int n;
    strncpy(program_name, parsed_commands[0][0],
            NAME_MAX - 1 - strlen(program_name));
    for (n = 1; n < 10 && parsed_commands[n] != NULL; n++) {
        strncat(program_name, " | ", NAME_MAX - 1 - strlen(program_name));
        strncat(program_name, parsed_commands[n][0],
                NAME_MAX - 1 - strlen(program_name));
    }
    program_name[NAME_MAX - 1] = '\0';

    // Creating pipes to transfer information from each program to the next
    int **pipes = malloc(sizeof(int *) * (n - 1));
    for (int i = 0; i < n - 1; i++) {
        pipes[i] = malloc(sizeof(int) * 2);
        pipe(pipes[i]);
    }
    int *read_pipe, *write_pipe;

    /* Creating an internal pipe to give the program provided
       as argument (child process) permission to run */
    int internal_pipe[2];
    pipe(internal_pipe);

    // A cycle is created where each command will run in order
    pid_t pid;
    for (int i = 0; i < n; i++) {

        pid_t child_pid = fork();
        // Child process
        if (child_pid == 0) {
            /* Before starting the pipeline, the program needs to wait for
               permission from the server */
            if (i == 0) {
                close(internal_pipe[1]);
                int run_permission = 0, bytes_read = 0;
                while (!bytes_read) {
                    bytes_read =
                        read(internal_pipe[0], &run_permission, sizeof(int));
                }
                if (!run_permission)
                    break;
            }
            /* In the first iteration the program isn't fed any info from
               previous programs, so it doesn't use a read pipe*/
            if (i != 0) {
                read_pipe = pipes[i - 1];
                dup2(read_pipe[0], STDIN_FILENO);
                close(read_pipe[0]);
                close(read_pipe[1]);
            }
            /* In the last iteration the program doesn't feed any info and
               prints to STDOUT, so it doesn't redirect its output to a pipe*/
            if (i != n - 1) {
                write_pipe = pipes[i];
                dup2(write_pipe[1], STDOUT_FILENO);
                close(write_pipe[0]);
                close(write_pipe[1]);
            }
            execvp(parsed_commands[i][0], parsed_commands[i]);
        }
        // Parent process
        if (child_pid) {
            // In the first iteration a run request is sent to the server
            if (i == 0) {
                close(internal_pipe[0]);
                pid = child_pid;

                // Writing "Running PID" message
                char *running_msg = pid_running_msg(child_pid);
                write(STDOUT_FILENO, running_msg,
                      sizeof(char) * (14 + MAX_PID_LENGTH));
                free(running_msg);

                Request request =
                    new_pipeline_request(getpid(), child_pid, program_name);
                write(input_pipe, &request, sizeof(Request));

                /*The parent process waits for permission to run from the
                  server, then sends it on to the child process via an unnamed
                  pipe*/
                int run_permission = 0, bytes_read = 0;
                int output_pipe = open(output_pipe_string, O_RDONLY);
                while (!bytes_read) {
                    bytes_read =
                        read(output_pipe, &run_permission, sizeof(int));
                }
                close(output_pipe);
                write(internal_pipe[1], &run_permission, sizeof(int));
            }
            /*The input pipe for the current program is closed on the parent
            process' end to cause EOF and terminate the program*/
            if (i != 0) {
                close(pipes[i - 1][0]);
                close(pipes[i - 1][1]);
            }
        }
    }

    // The exit statuses of all programs are then gathered
    int *status = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) {
        wait(&(status[i]));
    }

    // The exit statuses are then checked for any error
    for (int i = 0; i < n; i++) {
        if (!WIFEXITED(status[i])) {
            print_error(PROGRAM_EXEC_ERROR);
        }
    }
    free(status);

    // The server is informed the program has stopped
    Request request = finished_execution_request(getpid(), pid);
    write(input_pipe, &request, sizeof(Request));
    int output_pipe = open(output_pipe_string, O_RDONLY);
    struct timeval execution_time;
    int read_bytes = 0;
    while (!read_bytes) {
        read_bytes = read(output_pipe, &execution_time, sizeof(struct timeval));
    }

    // The program execution time is printed to the user
    char *time_msg = time_taken_msg(execution_time);
    write(STDOUT_FILENO, time_msg, sizeof(char) * strlen(time_msg));
    free(time_msg);

    return 0;
}

// Requests the server the current programs and prints them to the user
int execute_status(char *output_pipe_string) {
    // Opens the input pipe
    int input_pipe = open(REQUEST_PIPE_PATH, O_WRONLY);

    // Creates and sends the request to the server
    pid_t pid = getpid();
    Request status_request = new_status_request(pid);
    write(input_pipe, &status_request, sizeof(Request));

    int output_pipe = open(output_pipe_string, O_RDONLY);
    char message[100];
    int bytes_read = 0;
    while (!bytes_read) {
        bytes_read = read(output_pipe, message, sizeof(char) * 100);
    }
    while (strcmp(message, "")) {
        if (bytes_read != 0) {
            write(STDOUT_FILENO, message, sizeof(char) * strlen(message));
        }
        bytes_read = read(output_pipe, message, sizeof(char) * 100);
    }
    close(input_pipe);
    close(output_pipe);
    return 0;
}
