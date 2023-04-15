#include <stdlib.h>
#include <string.h>

char** parse_command(char* command_string){
    char** parsed_command = malloc(sizeof(char*) * 11);
    for(int i = 0; i < 11; i++){
        parsed_command[i] = NULL;    
    }
    //The cycle iterates until the pointer is at the last word
    char* token = strtok(command_string, " ");
    for(int i = 0; i < 10 && token != NULL; i++){
        parsed_command[i] = token;
        token = strtok(NULL," ");
    }
    return parsed_command;
}