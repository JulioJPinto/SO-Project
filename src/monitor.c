#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <common.h>

int main(){
    write(STDOUT_FILENO,"Server starting",sizeof(char) * 16);
    if( ( mkfifo(REQUEST_PIPE_PATH,S_IRWXU)) != 0) {
        write(STDOUT_FILENO,"Error creating FIFO",sizeof(char) * 20);
        return -1;
    }
    int pipe = open(REQUEST_PIPE_PATH,O_RDONLY);
    char* a = malloc(sizeof(char) * 4);
    read(pipe,a,sizeof(char) * 4);
    write(STDOUT_FILENO,a,sizeof(char) * 4);
    unlink(REQUEST_PIPE_PATH);
    return 0;
}