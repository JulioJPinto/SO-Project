#include <stdlib.h>

#include "array.h"
#include "requests.h"

// Creates new array for running programs to be stored
Running_Programs new_list() {
    Request *array = malloc(sizeof(Request) * 25);
    Running_Programs new_list;
    new_list.array = array;
    new_list.current_size = 0;
    new_list.max_size = 25;
    return new_list;
}

// Increases the size of the array
void extend_array(Running_Programs *list) {
    list->max_size *= 2;
    list->array = realloc(list->array, list->max_size);
}

// Adds a program to the list of currently running programs
void add_running_program(Running_Programs *list, Request new_program) {
    if (list->current_size == list->max_size) {
        extend_array(list);
    }
    list->array[list->current_size] = new_program;
    list->current_size++;
}

// Finds the position of a given program by its pid
int find_running_program(Running_Programs *list, pid_t pid) {
    for (int i = 0; i < list->current_size; i++) {
        if (list->array[i].child_pid == pid) {
            return i;
        }
    }
    return -1;
}

// Deletes a program from the list of currently running programs
void delete_running_program(Running_Programs *list, int position) {
    for (int i = position; i < list->current_size - 1; i++) {
        list->array[i] = list->array[i + 1];
    }
    list->current_size--;
}