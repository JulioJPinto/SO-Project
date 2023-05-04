#include <unistd.h>

#include "errors.h"

int print_error(error error) {
    int print_status;
    switch (error) {
    case WRONG_NUM_ARGUMENTS:
        print_status =
            write(STDERR_FILENO,
                  "Program usage: ./tracer <command> (options) (arguments)\n",
                  sizeof(char) * 57);
        return print_status;
    case UNKNOWN_COMMAND:
        print_status =
            write(STDERR_FILENO, "Unknown command\n", sizeof(char) * 17);
        return print_status;
    case UNKNOWN_OPTION:
        print_status =
            write(STDERR_FILENO, "Unknown option\n", sizeof(char) * 16);
        return print_status;
    case FIFO_CREATING_ERROR:
        print_status =
            write(STDERR_FILENO, "Error creating FIFO\n", sizeof(char) * 21);
        return print_status;
    case REQUEST_NOT_FOUND:
        print_status = write(
            STDERR_FILENO,
            "Couldn't find the original request upon the program finishing\n",
            sizeof(char) * 63);
        return print_status;
    case PROGRAM_EXEC_ERROR:
        print_status =
            write(STDERR_FILENO, "Requested program exited with errors\n",
                  sizeof(char) * 38);
        return print_status;
    default:
        return -1;
    }
}