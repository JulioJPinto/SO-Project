#ifndef EXECUTE_H
#define EXECUTE_H

int single_execute(char *command, char *output_pipe_string);

int pipeline_execute(char *commands, char *output_pipe_string);

int execute_status(char *output_pipe_string);

int execute_stats_time(char **pids, int pids_number, char *output_pipe_string);

int execute_stats_command(char *command, char **pids, int pids_number,
                          char *output_pipe_string);

#endif