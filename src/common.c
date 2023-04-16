#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

//Not sure this is the best place for this function, maybe a messages.c?
char* pid_running_msg(pid_t PID){
    char* running_msg = malloc(sizeof(char) * (14 + MAX_PID_LENGTH));
    strcpy(running_msg,"Running PID ");
    char PID_string[MAX_PID_LENGTH + 1];
    sprintf(PID_string,"%d",PID);
    strcat(running_msg,PID_string);
    strcat(running_msg,"\n");
    return running_msg;
}
