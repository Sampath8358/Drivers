#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

#define IOCTL_READ  _IOR('d', 1, int)
#define IOCTL_WRITE _IOW('d', 2, int)

 
int main()
{
        int fd;
        int  value = 0;
        int number = 17;
        printf("\nOpening Driver\n");
        fd = open("/dev/myDevFile", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
       
        printf("Writing Value to Driver\n");
        ioctl(fd, IOCTL_WRITE, number); 
 
        printf("Reading Value from Driver\n");
        ioctl(fd, IOCTL_READ, value);
        printf("Value is %d\n", value);
 
        printf("Closing Driver\n");
        close(fd);
}