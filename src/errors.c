#include <unistd.h>

#include "errors.h"

int print_error(error error) {
    return write(STDERR_FILENO, errors[error - 1].message, errors[error - 1].size);
}