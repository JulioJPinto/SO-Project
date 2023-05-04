#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "array.h"
#include "common.h"
#include "errors.h"
#include "interface.h"
#include "requests.h"

static int running = 1;

void monitor_handler(int x) {
    (void)x;
    running = 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        print_error(NO_FOLDER_GIVEN);
        return 0;
    }
    char *pids_folder_path = malloc(sizeof(char) * (2 + strlen(argv[1])));
    strcpy(pids_folder_path, argv[1]);
    strcat(pids_folder_path, "/");

    // The request pipe is made
    write(STDOUT_FILENO, "Server starting\n", sizeof(char) * 17);
    if ((mkfifo(REQUEST_PIPE_PATH, S_IRWXU)) != 0) {
        print_error(FIFO_CREATING_ERROR);
        return 0;
    }

    // The request pipe is open
    int request_pipe = open(REQUEST_PIPE_PATH, O_RDONLY);
    Request request;
    Running_Programs running_programs = new_list();

    signal(SIGINT, monitor_handler);

    // The server keeps listening for requests
    while (running) {
        int read_bytes = read(request_pipe, &request, sizeof(Request));
        if (read_bytes) {
            // The requests are then handled
            write(STDOUT_FILENO, "Handling Request\n", sizeof(char) * 18);
            handle_request(request, &running_programs, pids_folder_path);
        }
    }
    write(STDOUT_FILENO, "Server shutting down\n", sizeof(char) * 22);
    unlink(REQUEST_PIPE_PATH);
    free(running_programs.array);
    free(pids_folder_path);
    return 0;
}