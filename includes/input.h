#ifndef INPUT_H
#define INPUT_H

#include "requests.h"

request_type identify_command(char **argv);

error verify_input(int argc, char **argv);

#endif
