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

typedef struct ErrorInfo {
    char *message;
    size_t size;
} ErrorInfo;

static const ErrorInfo errors[] = {
    // There is no error info for 'NONE' being its the default value for
    // veryfing
    {"Program usage: ./tracer <command> (options) (arguments)\n",
     sizeof(char) * 57},
    {"Unknown command\n", sizeof(char) * 17},
    {"Unknown option\n", sizeof(char) * 16},
    {"Error creating FIFO\n", sizeof(char) * 21},
    {"Couldn't find the original request upon the program finishing\n",
     sizeof(char) * 63},
    {"Requested program exited with errors\n", sizeof(char) * 38},
    {"Program usage: ./monitor <Process folder path>\n", sizeof(char) * 48},
    {"No PIDS given\n", sizeof(char) * 15},
    {"File not found\n", sizeof(char) * 16},
    {"Too many PIDs passed as argument, increase NAME_MAX constant\n", sizeof(char) * 62},

};

int print_error(error error);

#endif