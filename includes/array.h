#ifndef ARRAY_H
#define ARRAY_H
#include "requests.h"
typedef struct {
    Request *array;
    int current_size;
    int max_size;
} Running_Programs;

Running_Programs new_list();

void add_running_program(Running_Programs *list, Request new_program);

int find_running_program(Running_Programs *list, pid_t pid);

void delete_running_program(Running_Programs *list, int position);
#endif