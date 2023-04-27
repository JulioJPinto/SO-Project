#include <string.h>

#include "errors.h"
#include "requests.h"

error verify_arg_number(int argc) {
    if (argc != 4) {
        return WRONG_NUM_ARGUMENTS;
    }
    return NONE;
}

request_type identify_command_arg(char *command) {
    if (strcmp(command, "execute") != 0) {
        return UNKNOWN;
    }
    return SINGLE_EXEC;
}

request_type identify_option(char *option) {
    if (strcmp(option, "-u") != 0) {
        return UNKNOWN;
    }
    return SINGLE_EXEC;
}

error verify_input(int argc, char **argv) {
    error argument_check;
    argument_check = verify_arg_number(argc);
    if (argument_check != NONE) {
        return argument_check;
    }
    request_type command_check;
    command_check = identify_command_arg(argv[1]);
    if (command_check == UNKNOWN) {
        return UNKNOWN_COMMAND;
    }
    if (identify_option(argv[2]) == UNKNOWN) {
        return UNKNOWN_OPTION;
    }
    return NONE;
}

request_type identify_command(char **argv) {
    request_type command;
    command = identify_command_arg(argv[1]);
    if (command == SINGLE_EXEC) {
        return identify_option(argv[2]);
    }
    return UNKNOWN;
}