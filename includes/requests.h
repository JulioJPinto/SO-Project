#ifndef REQUESTS_H
#define REQUESTS_H

#define CREATE_CONNECTION 1
#define CLOSE_CONNECTION 0
#define START_EXEC 2
#define FINISH_EXEC 3

#include "common.h"

typedef struct request {
  int type;
  pid_t PID;
  char program_name[NAME_MAX];
  struct timeval time;
} Request;

Request new_execute_request(pid_t PID, char *program_name);

Request finished_execution_request(pid_t PID);

#endif