#ifndef ERRORS_H
#define ERRORS_H

// Definition of errors
#define WRONG_NUM_ARGUMENTS -1
#define UNKNOWN_COMMAND -2
#define UNKNOWN_OPTION -3
#define FIFO_CREATING_ERROR -4

int print_error(int error);

#endif