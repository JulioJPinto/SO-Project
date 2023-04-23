#ifndef INTERFACE_H
#define INTERFACE_H
#include <sys/types.h>

#include "requests.h"

int handle_request(Request request, Request *running_programs);

#endif