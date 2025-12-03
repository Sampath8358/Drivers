#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    int fd;
    fd = open("/dev/wqdevicefile1", O_RDWR);
    char buf[100];
    char buffer[100];
    write(fd, buffer, sizeof(buffer));
    printf("ipoindi\n");
    close(fd);
}