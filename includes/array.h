#ifndef ARRAY_H
#define ARRAY_H
#include "requests.h"
typedef struct {
    Request *array;
    int current_size;
    int max_size;
} Running_Programs;

// Creates new array for running programs to be stored
Running_Programs new_list();

// Adds a program to the list of currently running programs
void add_running_program(Running_Programs *list, Request new_program);

// Finds the position of a given program by its pid
int find_running_program(Running_Programs *list, pid_t pid);

// Deletes a program from the list of currently running programs
void delete_running_program(Running_Programs *list, int position);
#endif