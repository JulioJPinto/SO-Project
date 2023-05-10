#include <unistd.h>
#include <string.h>

#include "errors.h"

int print_error(error error) {
    return write(STDERR_FILENO, errors[error - 1], sizeof(char) * strlen(errors[error - 1]));
}