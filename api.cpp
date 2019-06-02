#include<sys/un.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>

#define CONTROLLEN CMSG_LEN(sizeof(int)) 

static struct cmsghdr* cmptr = NULL ;

//send_fd 先发送２字节０，然后发送实际描述符
//发送文件描述符
//通过Unix域套接字传递文件描述符，snedmsg调用被
//用来传送协议数据以及描述符
int send_fd(int fd, int fd_to_send) {
    
    struct iovec iov[1] ;
    struct msghdr msg ;
    char buf[2] ;
        
    //向往缓冲区填上两字节数据，描述符随其后发送
    iov[0].iov_base = buf ;
    iov[0].iov_len = 2 ;
    msg.msg_iov = iov ;
    msg.msg_iovlen = 1 ;
    msg.msg_name = NULL ;
    msg.msg_namelen = 0 ;
    
    if(fd_to_send < 0) {
        msg.msg_control = NULL ;
        msg.msg_controllen = 0 ;
        buf[1] = -fd_to_send ;
        if(buf[1] == 0) {
            buf[1] = 1 ;
        }
    }
    else {
        if(cmptr == NULL && (cmptr = (struct cmsghdr*)malloc(CONTROLLEN)) == NULL) {
            return -1 ;
        }
        cmptr->cmsg_level = SOL_SOCKET ;
        cmptr->cmsg_type = SCM_RIGHTS ;
        cmptr->cmsg_len = CONTROLLEN ;
        msg.msg_control = cmptr ;
        msg.msg_controllen = CONTROLLEN ;
        *(int*)CMSG_DATA(cmptr) = fd_to_send ;
        buf[1] = 0 ;
    }
    buf[0] =  0; 
    if(sendmsg(fd, &msg, 0) != 2) {
        return -1 ;
    }
    return 0 ;
}
