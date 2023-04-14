#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <common.h>

int main(){
    write(STDOUT_FILENO,"Server starting\n",sizeof(char) * 17);
    if( ( mkfifo(REQUEST_PIPE_PATH,S_IRWXU)) != 0) {
        write(STDOUT_FILENO,"Error creating FIFO\n",sizeof(char) * 21);
        return -1;
    }
    int pipe = open(REQUEST_PIPE_PATH,O_RDONLY);
    char* a = malloc(sizeof(char) * 5);
    read(pipe,a,sizeof(char) * 5);
    write(STDOUT_FILENO,a,sizeof(char) * 5);
    unlink(REQUEST_PIPE_PATH);
    return 0;
}