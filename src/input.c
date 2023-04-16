#include <string.h>

#include "common.h"

int verify_arg_number(int argc) {
  if (argc != 4) {
    return WRONG_NUM_ARGUMENTS;
  }
  return 0;
}

int identify_command(char *command) {
  if (strcmp(command, "execute") != 0) {
    return UNKNOWN_COMMAND;
  }
  return SINGLE_EXECUTE;
}

int identify_option(char *option) {
  if (strcmp(option, "-u") != 0) {
    return UNKNOWN_OPTION;
  }
  return SINGLE_EXECUTE;
}

int verify_input(int argc, char **argv) {
  int argument_check, command_check;
  argument_check = verify_arg_number(argc);
  if (argument_check != 0) {
    return argument_check;
  }
  command_check = identify_command(argv[1]);
  if (command_check != 0) {
    return command_check;
  }
  return identify_option(argv[2]);
}