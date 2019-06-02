#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>

int recvfd(int);

void client_socketun(){
    int sockfd, newfd, len, nr;
    sockaddr_un un;
    char buf[20]={0};

    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, "/tmp/s");
    len = sizeof(un.sun_family) + strlen(un.sun_path);

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(connect(sockfd, (sockaddr *)&un, len) == -1){//连接socket
        printf("connect error");
        return ;
    }

    // read(sockfd, buf, 20);
    recv(sockfd, buf, 20, 0);//接收普通数据
    printf("接收到普通数据：%s\n", buf) ; 
    if((newfd = recvfd(sockfd)) == -1){//接收msg数据
        printf("rec error");
    }
    else{
        printf("通过新的文件描述符读取文件数据：\n") ;
        while(1){
            if((nr = read(newfd, buf, 10)) == -1){
                perror("read error");
            }
            if(nr == 0) {
                break ;
            }
            else{
                printf("%s", buf);
                fflush(stdout);
            }
        }
    }
    close(newfd) ;
    close(sockfd);
}

int recvfd(int sockfd){
    int newfd, nr;
    struct cmsghdr *cmptr = nullptr;//OUT
    int cmsghdrlen = CMSG_LEN(sizeof(int));
    char buf[2];
    struct iovec iov[1];
    struct msghdr msg;

    iov[0].iov_base = buf;
    iov[0].iov_len = 2;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    cmptr = (cmsghdr *)malloc(cmsghdrlen);
    msg.msg_control = cmptr;
    msg.msg_controllen = cmsghdrlen;

    if((nr = recvmsg(sockfd, &msg, 0)) < 0){
        perror("recvmsg error\n");
    }else{
        newfd = *(int *)CMSG_DATA(cmptr);
        fflush(stdout);
        return newfd;
    }

    return -1;
}

int main(){

    client_socketun();

    return 0;
}
