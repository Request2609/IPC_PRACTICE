#include <iostream>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
int main()
{
    int fd = open("hello.c", O_RDWR) ;
    if(fd < 0) {
        printf("打开文件失败！\n") ;
        return 1;
    }
    int fd1 = open("hello", O_CREAT|O_WRONLY, 0666) ;
    if(fd1 < 0) {
         printf("创建文件失败！\n") ;
        return 1 ;
    }
    char buf[1024] ;
    int cur = 0;
    int ret = 0 ;
    int right = 0 ;
    int left = 0 ;
    while((ret = read(fd, buf, sizeof(buf)))) {
        cur = left ;
        left +=  ret;
        right = left ;

        int res = pwrite(fd1, buf, right-cur, cur) ;
        if(res < 0) {
            printf("写文件失败！\n") ;
            return 1;
        }
    }   

    close(fd) ;
    close(fd1) ;
   return 0;
}

