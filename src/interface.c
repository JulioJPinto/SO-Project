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

void save_request_to_file(Request request, char *pids_folder_path) {
    // The path to the new file is created and the file is opened
    char *file_path =
        malloc(sizeof(char) * (strlen(pids_folder_path) + MAX_PID_LENGTH));
    sprintf(file_path, "%s%d", pids_folder_path, request.child_pid);
    int file = open(file_path, O_CREAT | O_WRONLY, 0666);

    // The program's name is written to the file
    write(file, request.program_name,
          sizeof(char) * strlen(request.program_name));
    write(file, "\n", sizeof(char));

    // The time taken(ms) is written to the file
    long time_taken = time_struct_to_long(request.time);
    char time_string[100];
    int chars_written = sprintf(time_string, "%ld\n", time_taken);
    write(file, time_string, sizeof(char) * (chars_written + 1));
}

int handle_request(Request request, Running_Programs *running_programs,
                   char *pids_folder_path) {
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

        // The program's log is saved to a file
        init_request.time = running_time;
        save_request_to_file(init_request, pids_folder_path);

        // The pipe is closed
        close(output_pipe);
        free(output_pipe_string);
    } break;
    case STATUS: {
        // The output pipe is open
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);
        free(output_pipe_string);

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
    case STATS_TIME: {
        // The output pipe is open
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);
        free(output_pipe_string);

        char *file_path_string =
            malloc(sizeof(char) * (strlen(pids_folder_path) + MAX_PID_LENGTH));
        int file_path, bytes_read;
        char buffer[100], *time_position;
        char *pids_string = request.program_name;
        char *pid = strtok(pids_string, " ");
        long total_time = 0;
        while (pid != NULL) {
            sprintf(file_path_string, "%s%s", pids_folder_path, pid);
            file_path = open(file_path_string, O_RDONLY);
            if (file_path == -1) {
                print_error(FILE_NOT_FOUND);
                break;
            }
            bytes_read = read(file_path, buffer, sizeof(char) * 100);
            time_position = strstr(buffer, "\n");
            time_position++;
            long time = 0;
            time = (strtol(time_position, NULL, 0));
            char *temp;
            while (bytes_read == 100) {
                bytes_read = read(file_path, buffer, sizeof(char) * 100);
                time += strtol(buffer, &temp, 0) * (temp - buffer);
            }
            total_time += time;
            close(file_path);
            pid = strtok(NULL, " ");
        }
        write(output_pipe, &total_time, sizeof(long));
        close(output_pipe);
        free(file_path_string);
    } break;
    case STATS_COMMAND: {
        // The output pipe is open
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);
        free(output_pipe_string);

        char *file_path_string =
            malloc(sizeof(char) * (strlen(pids_folder_path) + MAX_PID_LENGTH));
        char *temp, *program = strtok(request.program_name, " ");
        char *token = strtok(NULL, " ");
        char buffer[50];
        int file, result = 0;
        while (token != NULL) {
            sprintf(file_path_string, "%s%s", pids_folder_path, token);
            file = open(file_path_string, O_RDONLY);
            read(file, buffer, sizeof(char) * 50);
            temp = strstr(buffer, "\n");
            *temp = '\0';
            if (!strcmp(buffer, program)) {
                result++;
            }
            token = strtok(NULL, " ");
        }
        write(output_pipe, &result, sizeof(int));
        free(file_path_string);
        close(output_pipe);
    } break;
    case STATS_UNIQ: {
        char *output_pipe_string = output_pipe_by_pid(request.requesting_pid);
        int output_pipe = open(output_pipe_string, O_WRONLY);
        free(output_pipe_string);


        char *file_path_string =
            malloc(sizeof(char) * (strlen(pids_folder_path) + MAX_PID_LENGTH));
        char *temp, *program = strtok(request.program_name, " ");
        char *token = strtok(NULL, " ");
        char buffer[50];
        int file;

        char *uniqs_array[64];
        for(int i = 0; i < 64; i++) {
            uniqs_array[i] = NULL;
        }

        while (token != NULL) {
            sprintf(file_path_string, "%s%s", pids_folder_path, token);
            file = open(file_path_string, O_RDONLY);
            read(file, buffer, sizeof(char) * 50);

            char pid_prog_name[50];
            for(int i = 0; i < 50 ; i++) {
                pid_prog_name[i] = buffer[i];
                if(pid_prog_name[i] == '\n') {
                    pid_prog_name[i++] == '\0';
                    break;
                }
            }
            
            int controller = 0;
            int counter = 0;
            for( ; counter < 64 && uniqs_array[counter] != NULL; counter++) {
                if(!strcmp(uniqs_array[counter], pid_prog_name)) {
                    controller = 1;
                    break;
                }
            }
            if(!controller) {
                strcmp(uniqs_array[counter + 1], pid_prog_name);
            }

            token = strtok(NULL, " ");
        }

        char* result = "";
        for(int i = 0; i < 64 && uniqs_array[i] != NULL; i++) {
            strcat(result,uniqs_array[i]);
            strcat(result,"\n");
        }

        write(output_pipe, result, sizeof(char) * strlen(result));
        free(file_path_string);
        close(output_pipe);
    } break;
    default:
        break;
    }
    return 0;
}