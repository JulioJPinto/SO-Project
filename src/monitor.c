#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "common.h"

int main(){
    write(STDOUT_FILENO,"Server starting\n",sizeof(char) * 17);
    if( ( mkfifo(REQUEST_PIPE_PATH,S_IRWXU)) != 0) {
        write(STDOUT_FILENO,"Error creating FIFO\n",sizeof(char) * 21);
        return -1;
    }
    int pipe = open(REQUEST_PIPE_PATH,O_RDONLY);
    Request request;
    read(pipe,&request,sizeof(Request));
    write(STDOUT_FILENO,"program started\n",sizeof(char) * 17);
    read(pipe,&request,sizeof(Request));
    write(STDOUT_FILENO,"program finished\n",sizeof(char) * 18);
    unlink(REQUEST_PIPE_PATH);
    return 0;
}