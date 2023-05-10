#include <string.h>

#include "errors.h"
#include "requests.h"

error verify_arg_number(int argc) {
    if (argc >= 2) {
        return NONE;
    }
    return WRONG_NUM_ARGUMENTS;
}

request_type identify_command_arg(char *command) {
    if (strcmp(command, "execute") == 0) {
        return SINGLE_EXEC;
    }
    if (strcmp(command, "status") == 0) {
        return STATUS;
    }
    if (strcmp(command, "stats-time") == 0) {
        return STATS_TIME;
    }
    if (strcmp(command, "stats-command") == 0) {
        return STATS_COMMAND;
    }
    return UNKNOWN;
}

request_type identify_option(char *option) {
    if (option[1] == 'u') {
        return SINGLE_EXEC;
    }
    if (option[1] == 'p') {
        return PIPELINE_EXEC;
    }
    return UNKNOWN;
}

error verify_input(int argc, char **argv) {
    error argument_check = verify_arg_number(argc);

    if (argument_check != NONE) {
        return argument_check;
    }

    request_type command_check = identify_command_arg(argv[1]);

    if (command_check == UNKNOWN) {
        return UNKNOWN_COMMAND;
    }

    if (command_check == SINGLE_EXEC && (identify_option(argv[2]) == UNKNOWN)) {
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
    return command;
}