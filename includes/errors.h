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
    NO_FOLDER_GIVEN,
    NO_PIDS_GIVEN,
    FILE_NOT_FOUND,
    TOO_MANY_PIDS
} error;

typedef char* ErrorInfo ;

static const ErrorInfo errors[] = {
    // There is no error info for 'NONE' being its the default value for veryfing
    "Program usage: ./tracer <command> (options) (arguments)\n",
    "Unknown command\n",
    "Unknown option\n",
    "Error creating FIFO\n",
    "Couldn't find the original request upon the program finishing\n",
    "Requested program exited with errors\n",
    "Program usage: ./monitor <Process folder path>\n",
    "No PIDS given\n",
    "File not found\n",
    "Too many PIDs passed as argument, increase NAME_MAX constant\n",
};

int print_error(error error);

#endif