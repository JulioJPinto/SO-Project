#ifndef INTERFACE_H
#define INTERFACE_H
#include <sys/types.h>

#include "array.h"
#include "requests.h"

int handle_request(Request request, Running_Programs *running_programs);

#endif