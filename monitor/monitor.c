#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
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

int main() {
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

    // The server keeps hearing for requests
    while (running) {
        int read_bytes = read(request_pipe, &request, sizeof(Request));
        if (read_bytes) {
            // The requests as then handled
            write(STDOUT_FILENO, "Handling Request\n", sizeof(char) * 18);
            handle_request(request, &running_programs);
        }
    }
    unlink(REQUEST_PIPE_PATH);
    write(STDOUT_FILENO, "Server shutting down\n", sizeof(char) * 22);
    free(running_programs.array);
    return 0;
}