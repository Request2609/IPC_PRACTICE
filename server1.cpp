#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
 
void socketSendfd(int, int);
 
void namedSocket() {
    int sockfd, size;
    sockaddr_un un;
    char buf[20] = {"123"};
 
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, "/tmp/s");
 
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        printf("socket error");
    size = sizeof(un.sun_family) + strlen(un.sun_path);
//    print(size);
 
    if (bind(sockfd, (sockaddr *) &un, size) < 0) {//绑定
        printf("bind error");
        return;
    }
//    print("bound OK");
    if (listen(sockfd, 5) == -1) {//监听
        printf("listen error");
        return;
    }
 
    sockaddr_un client_addr;//OUT，接收客户端un信息
    socklen_t len;
    int clifd;
    if ((clifd = accept(sockfd, (sockaddr *) &client_addr, &len)) == -1) {
        printf("accept error");
        return;
    }
    printf("发送普通数据:%s\n", buf) ;
    send(clifd, buf, 10, 0);//传送普通数据
 
    int fd = open("hello", O_CREAT | O_RDWR, 0666);
    char data[12] ;
    memset(data, 0, sizeof(data)) ;
    read(fd, data, sizeof(data)) ;
    printf("读取文件的数据：%s\n",data) ;

    socketSendfd(clifd, fd);//传送文件描述符
 
    close(fd);
    close(sockfd);
    unlink(un.sun_path);//解除链接操作
}
 
void socketSendfd(int clifd, int fdToSend){
    struct iovec iov[1];
    struct msghdr msg;//②msghdr结构体
    struct cmsghdr *cmptr = nullptr;//③cmsghdr结构体
    int cmsghdrlen = CMSG_LEN(sizeof(int));//CMSG_LEN()返回cmsghdr结构的cmsg_len成员的值，考虑到任何必要的对齐。它取出参数的长度。这是一个常量表达式。
    char buf[2] = {0};
 
    iov[0].iov_base = buf;
    iov[0].iov_len = 2;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    msg.msg_name = NULL;
    msg.msg_namelen = 0;
 
 
    cmptr = (cmsghdr *)malloc(cmsghdrlen);
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;//SCM_RIGHTS表明在传送访问权，访问权仅能通过UNIX域套接字传送。
    cmptr->cmsg_len = cmsghdrlen;
 
 
    msg.msg_control = cmptr;
    msg.msg_controllen = cmsghdrlen;
    printf("要发送文件描述符:%d\n", fdToSend) ;
    *(int *)CMSG_DATA(cmptr) = fdToSend;//CMSG_DATA()返回cmsghdr的数据部分指针。
 
    if(sendmsg(clifd, &msg, 0) == -1){
        perror("send error");
    }
    close(fdToSend) ;
}
 
int main() {
 
    namedSocket();
 
    return 0;
}
