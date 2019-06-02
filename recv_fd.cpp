#include<sys/socket.h>
#include<stdio.h>
#include<sys/un.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>
//recv_fd 读取套接字中所有字节直至遇到NULL字符
//null字符之前所有的字符都传送给调用者的userfunc

//该程序总是准备接收一个描述符
//在每次调用之前设置msg_control和msg_controllen
//在msg_controllen返回的是非０时才确实接收到描述符
#define CONTROLLEN CMSG_LEN(sizeof(int)) 

static struct cmsghdr *cmptr = NULL ;

int recv_fd(int fd, ssize_t(*userfunc)(int, const void *t, size_t)) {
    
    int newFd , nr ,status ;
    char* ptr ;
    char buf[1024] ;
    struct iovec iov[1] ;
    struct msghdr msg ;
    status = 1 ;

    for(;;) {
        iov[0].iov_base = buf ;
        iov[0].iov_len =sizeof(buf) ;   
        msg.msg_iov = iov ;
        msg.msg_iovlen = 1 ;
        msg.msg_name = NULL ;
        msg.msg_namelen = 0 ;

        if(cmptr == NULL && (cmptr = (struct cmsghdr*)malloc(CONTROLLEN)) == NULL) {
            return -1 ;
        }
        msg.msg_control = cmptr ;
        msg.msg_controllen = CONTROLLEN ;
        if((nr = recvmsg(fd, &msg, 0)) < 0) {
            return -1 ;
        }
        else if(nr == 0){
            return -1 ;
        }
        for(ptr = buf; ptr < &buf[nr]; ) {
            if(*ptr++ == 0) {
                if(ptr != &buf[nr-1]) {
                    printf("出现错误\n") ;
                    return -1 ;
                }
                status = * ptr&0xFF ;
                if(status == 0) {
                    if(msg.msg_controllen < CONTROLLEN) {
                        printf("出现错误！\n") ;
                    }
                    newFd = *(int*)CMSG_DATA(cmptr) ;
                }
                else {
                    newFd = -status ;
                }
                nr = -2 ;
            }
        }
        if(nr > 0 && (*userfunc)(2, buf, nr) != nr) {
            return -1 ;
        }

        if(status >= 0) {
            return newFd ;
        }
    }
}
