#include <iostream>
#include<sys/un.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#define SIZE 1024
#define SV_SOCK_PATH "/tmp/uu"

int main()
{
    
    struct sockaddr_un addr ;
    int sfd ;
    ssize_t num ;
    char buf[SIZE] ;

    bzero(buf, SIZE) ;
    sfd = socket(AF_UNIX, SOCK_STREAM, 0) ;
    memset(buf, 0, sizeof(buf)) ;
    memset(&addr, 0, sizeof(addr)) ;
    addr.sun_family = AF_UNIX ;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1) ;

    int size = offsetof(struct sockaddr_un, sun_path)+strlen(addr.sun_path) ;
    if(connect(sfd, (struct sockaddr*)&addr, size)) {
        std::cout <<"connnect error!" << std::endl ;
        exit(1) ;
    }
    while((num = read(STDIN_FILENO, buf, SIZE)) >0) {
        write(sfd, buf, num) ;
    }
    if(num == -1) {
        std::cout <<"write error!" << std ::endl ;
        exit(1) ;
    }

    return 0;
}

