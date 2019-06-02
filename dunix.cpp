#include <stdio.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/un.h>
#include<sys/socket.h>
#define BUFSIZE  1024
#define SV_SOCK_PATH "/tmp/uu"

int main()
{
    struct sockaddr_un addr ;
    int sfd ,cfd ;
    char buf[BUFSIZE] ;
    bzero(buf, 1024) ;
    sfd = socket(AF_UNIX, SOCK_STREAM, 0) ;
    
    if(sfd == -1) {
        std::cout <<"socket err" <<std::endl ;
        exit(1) ;
    }
    if(remove(SV_SOCK_PATH) == -1&& errno != ENOENT) {  
        std::cout<< "remove" <<std::endl ;
        exit(1) ;
    }

    memset(&addr, 0, sizeof(addr)) ;
    addr.sun_family = AF_UNIX ;
   strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1) ;

    int size = offsetof(struct sockaddr_un, sun_path)+strlen(addr.sun_path) ;
    if(bind(sfd, (struct sockaddr*)&addr, size) == -1) {
        std::cout <<"bind error!"<<std::endl ;
        exit(1) ;
    }
    if(listen(sfd, 10) == -1) {
        std::cout <<"listen error!"<<std::endl ;
        exit(1);
    }
    ssize_t  num ;
    for(;;){
        cfd = accept(sfd, NULL, NULL) ;
        if(cfd == -1){
            std::cout << "accept error!"<<std::endl ;
            exit(1) ;
        } 
        while((num = read(cfd, buf, BUFSIZE)) > 0)
            write(STDOUT_FILENO, buf, BUFSIZE); 
    }
    close(cfd) ;
    return 0;
}

