#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#include "common.h"
#include "parser.h"
#include "requests.h"

/*Very basic input verification, later on it would ideally would identify the command and 
  passed onto main codified into an int, which could be decoded in main by a dispatch table with a 
  function pointer to handle that specific command*/
int verify_arguments(int argc, char** argv){
    if(argc != 4){
        write(STDOUT_FILENO,"Not enough arguments\n",sizeof(char) * 22);
        return -1;
    }
    if(strcmp(argv[1],"execute") != 0){
        write(STDOUT_FILENO,"Unknown command\n",sizeof(char) * 17);
        return -1;
    }
    if(strcmp(argv[2],"-u") != 0){
        write(STDOUT_FILENO,"Unknown command\n",sizeof(char) * 17);
        return -1;
    }
    return 0;
}

int main(int argc, char** argv){
    if(verify_arguments(argc,argv) == -1){
        return -1;
    }

    char ** parsed_command = parse_command(argv[3]);
    //write(STDOUT_FILENO,parsed_command[0],100);

    pid_t pid = fork();
    if(pid == 0){
        execvp(parsed_command[0], parsed_command);
    }

    int pipe = open(REQUEST_PIPE_PATH,O_WRONLY);
    Request new_request = new_execute_request(pid,parsed_command[0]);
    write(pipe,&new_request,sizeof(Request));

    int status;
    pid_t finished;
    finished = wait(&status);
    Request finish_request = finished_execution_request(pid);
    write(pipe,&finish_request,sizeof(Request));

    close(pipe);
    return 0;
}