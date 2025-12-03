#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include<string.h>
int main()
{
    char str[50];
    char buff[]="hello from user\n";
    int fd;
    fd=open("/dev/sravan",O_RDWR,0666);
    if(fd==-1)
    {
        printf("error\n");
        return 1;
    }
    write(fd,buff,strlen(buff)+1);
    read(fd,str,sizeof(str));
    printf("%s",str);
    close(fd);
}
