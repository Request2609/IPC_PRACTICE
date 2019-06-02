#include<sys/un.h>
#include<stdio.h>
#include<sys/socket.h>
const char* name = "/tmp/h" ;

int main() {
    sockaddr_un un ;
    char buf[20] ;
    int sockfd, len, nr ;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0) ;
    un.sun_family = AF_UNIX ;
    strncpy(un.sun_path, name, sizeof(un.sun_path)-1) ;
    if(bind(sockfd, (struct sockaddr*)&un, sizeof(un))< 0) {
        return 0;
    }
    if(listen(sockfd, 10) < 0){
        return 0;
    }
    
    for(;;) {
        socklen_t len =sizeof(un) ;
        int connfd = accept(sockfd, (sockaddr*)&un, &len) ;
        if(connfd < 0) {
            printf("接收连接错误！\n") ;
            return 0;
        }
        char data[1024] ;
        memset(data, 0, sizeof(data)) ;
        if(recv(connfd, data, sizeof(data), 0) < 0){
            break ;
        }
        printf("接收到普通数据:%s\n", data) ;
        int newfd = recvfd(sockfd) ;
    }
}

