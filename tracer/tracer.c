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
    int result;
    
    error input_check = verify_input(argc, argv);
    if (input_check != NONE) {
        print_error(input_check);
        return EXIT_FAILURE;
    }

    request_type request_type = identify_command(argv);

    pid_t pid = getpid();
    char *output_pipe_string = get_pipe_name(pid);

    if (mkfifo(output_pipe_string, S_IRWXU) != 0) {
        print_error(FIFO_CREATING_ERROR);
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    switch (request_type) {
        case SINGLE_EXEC:
            result = single_execute(argv[3], output_pipe_string);
            break;
        case PIPELINE_EXEC:
            result = pipeline_execute(argv[3], output_pipe_string);
            break;
        case STATUS:
            result = execute_status(output_pipe_string);
            break;
        default:
            result = EXIT_FAILURE;
            break;
    }

    unlink(output_pipe_string);
    free(output_pipe_string);
    return result;
}