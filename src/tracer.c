#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <common.h>

int main(){
    int pipe = open(REQUEST_PIPE_PATH,O_WRONLY);
    write(pipe,"ola\n\0",sizeof(char) * 5);
    close(pipe);
    return 0;
}