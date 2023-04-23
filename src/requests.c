#include <string.h>
#include <sys/types.h>

#include "common.h"
#include "requests.h"

Request new_execute_request(pid_t requesting_pid, pid_t child_pid,
                            char *program_name) {
  Request new_request;
  new_request.type = SINGLE_EXEC;
  new_request.requesting_pid = requesting_pid;
  new_request.child_pid = child_pid;
  strncpy(new_request.program_name, program_name, NAME_MAX);
  struct timeval current_time;
  if (gettimeofday(&current_time, NULL) == 0) {
    new_request.time = current_time;
  }
  return new_request;
}

Request finished_execution_request(pid_t requesting_pid, pid_t child_pid) {
  Request new_request;
  new_request.type = FINISHED_EXEC;
  new_request.requesting_pid = requesting_pid;
  new_request.child_pid = child_pid;
  struct timeval current_time;
  if (gettimeofday(&current_time, NULL) == 0) {
    new_request.time = current_time;
  }
  return new_request;
}
