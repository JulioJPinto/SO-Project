#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "errors.h"
#include "execute.h"
#include "input.h"
#include "requests.h"

int main(int argc, char **argv) {
    int sucess;
    error input_check = verify_input(argc, argv);
    if (input_check != NONE) {
        print_error(input_check);
        return 0;
    }
    request_type request_type = identify_command(argv);

    pid_t pid = getpid();
    char *output_pipe_string = output_pipe_by_pid(pid);

    if (mkfifo(output_pipe_string, S_IRWXU) != 0) {
        print_error(FIFO_CREATING_ERROR);
        return 1;
    }

    switch (request_type) {
    case SINGLE_EXEC:
        sucess = single_execute(argv[3], output_pipe_string);
        break;
    case STATUS: {
        sucess = execute_status(output_pipe_string);
    } break;
    default:
        sucess = 1;
        break;
    }

    unlink(output_pipe_string);
    free(output_pipe_string);
    return sucess;
}