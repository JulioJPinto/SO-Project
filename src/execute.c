#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"
#include "parser.h"
#include "requests.h"

int single_execute(char *command) {

  char **parsed_command = parse_command(command);

  pid_t pid = fork();
  if (pid == 0) {
    execvp(parsed_command[0], parsed_command);
  }
  int pipe = open(REQUEST_PIPE_PATH, O_WRONLY);

  Request new_request = new_execute_request(pid, parsed_command[0]);
  write(pipe, &new_request, sizeof(Request));

  char *running_msg = pid_running_msg(pid);
  write(STDOUT_FILENO, running_msg, sizeof(char) * (14 + MAX_PID_LENGTH));

  int status;
  pid_t finished;
  finished = wait(&status);
  Request finish_request = finished_execution_request(pid);
  write(pipe, &finish_request, sizeof(Request));
  close(pipe);
  free(parsed_command);
  return 0;
}