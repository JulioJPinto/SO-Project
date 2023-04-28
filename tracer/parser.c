#include <stdlib.h>
#include <string.h>

#include <stdio.h>

char **parse_command(char *command_string) {
    char **parsed_command = malloc(sizeof(char *) * 11);
    for (int i = 0; i < 11; i++) {
        parsed_command[i] = NULL;
    }
    // The cycle iterates until the pointer is at the last word
    char *token = strtok(command_string, " ");
    for (int i = 0; i < 10 && token != NULL; i++) {
        parsed_command[i] = token;
        token = strtok(NULL, " ");
    }
    return parsed_command;
}

char ***parse_pipeline(char *commands_string) {
    char ***parsed_commands = malloc(sizeof(char *) * 11);
    for (int i = 0; i < 11; i++) {
        parsed_commands[i] = NULL;
    }
    for (int i = 0; i < 10 && commands_string != NULL; i++) {
        char *command = commands_string;
        commands_string = strstr(commands_string, "|");
        if (commands_string != NULL) {
            *commands_string = '\0';
            commands_string++;
        }
        parsed_commands[i] = parse_command(command);
    }
    return parsed_commands;
}