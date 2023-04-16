#ifndef REQUESTS_H
#define REQUESTS_H

#define START_EXEC 1
#define FINISH_EXEC 2

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