#include <string.h>
#include <sys/types.h>

#include "requests.h"
#include "common.h"

Request new_execute_request(pid_t PID,char* program_name){
    Request new_request;
    new_request.type = START_EXEC;
    new_request.PID = PID;
    strncpy(new_request.program_name,program_name,NAME_MAX);
    struct timeval current_time;
    if(gettimeofday(&current_time,NULL) == 0){
        new_request.time = current_time;
    }
    return new_request;
}

Request finished_execution_request(pid_t PID){
    Request new_request;
    new_request.type = FINISH_EXEC;
    new_request.PID = PID;
    struct timeval current_time;
    if(gettimeofday(&current_time,NULL) == 0){
        new_request.time = current_time;
    }
    return new_request;
}
