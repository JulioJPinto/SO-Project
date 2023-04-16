#include "common.h"
#include "errors.h"
#include "execute.h"
#include "input.h"
#include "parser.h"
#include "requests.h"

int main(int argc, char **argv) {
  int sucess;
  int input_check = verify_input(argc, argv);
  if (input_check < 0) {
    int print_status = print_error(input_check);
    return print_status;
  }
  if (input_check == SINGLE_EXECUTE) {
    sucess = single_execute(argv[3]);
  }
  return sucess;
}