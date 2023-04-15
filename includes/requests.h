#ifndef REQUESTS_H
#define REQUESTS_H
#include "common.h"

Request new_execute_request(unsigned PID,char* program_name);

Request finished_execution_request(pid_t PID);

#endif