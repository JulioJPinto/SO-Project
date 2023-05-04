#ifndef ERRORS_H
#define ERRORS_H

// Definition of errors
typedef enum {
    NONE,
    WRONG_NUM_ARGUMENTS,
    UNKNOWN_COMMAND,
    UNKNOWN_OPTION,
    FIFO_CREATING_ERROR,
    REQUEST_NOT_FOUND,
    PROGRAM_EXEC_ERROR,
    NO_FOLDER_GIVEN
} error;

int print_error(error error);

#endif