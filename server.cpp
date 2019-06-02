#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<alloca.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/un.h>
 
#define SOCK_FILE "/tmp/sl"	//本地通信时Socket文件的文件名

int create_local_sock(char *sockfile)//次函数用于创建本地Socket
{
	int local_fd;
	struct sockaddr_un myaddr;
	if(-1 == (local_fd = socket(AF_LOCAL,SOCK_STREAM,0)))//创建Socket
	{
		perror("socket");
		exit(EXIT_FAILURE);	
	}	
	
	bzero(&myaddr,sizeof(myaddr));
	myaddr.sun_family = AF_LOCAL;//本地Socket
	strncpy(myaddr.sun_path,sockfile,strlen(sockfile));
	if(-1 == bind(local_fd,(struct sockaddr *)&myaddr,sizeof(myaddr)))//绑定
	{
		perror("bind");
		exit(EXIT_FAILURE);	
	}
	
	if(-1 == listen(local_fd,5))//监听
	{
		perror("listen");
		exit(EXIT_FAILURE);	
	}
	
	int new_fd;
	struct sockaddr_un peeraddr;
    socklen_t len = sizeof(peeraddr);
	new_fd = accept(local_fd,(struct sockaddr *)&peeraddr,&len);//阻塞等待
	
	if (-1 == new_fd)
	{
		perror("accept");
		exit(EXIT_FAILURE);	
	}
	return new_fd;
}
 
int send_fd(int sock_fd,char *file)	//发送文件描述符，期望对方写此文件描述符
{
	int fd_to_send;
	if(-1 == (fd_to_send = open(file,O_RDWR|O_APPEND)))//打开文件
	{
		perror("open");
		exit(EXIT_FAILURE);	
	}
    
    char buf[10] ;
    //读取文件部分内容,为了验证两个进程共享文件表项,移动一下文件指针
    read(fd_to_send, buf, sizeof(buf)) ;
    printf("读取到的部分文件内容为：%s",  buf) ;

	struct cmsghdr *cmsg;//描述符控制信息
	
	cmsg =(struct cmsghdr*) alloca(sizeof(struct cmsghdr) + sizeof(fd_to_send));//申请控制空间
	cmsg->cmsg_len = sizeof(struct cmsghdr) + sizeof(fd_to_send);//控制消息长度
	cmsg->cmsg_level = SOL_SOCKET;//控制消息级别
	cmsg->cmsg_type = SCM_RIGHTS;//控制消息类型
	memcpy(CMSG_DATA(cmsg),&fd_to_send,sizeof(fd_to_send));//控制消息内容
	
	struct msghdr msg;	//整个消息
	msg.msg_control = cmsg;//初始化控制消息
	msg.msg_controllen = cmsg->cmsg_len;
	
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	struct iovec iov[3];	//普通消息空间
	
    const char*p1 = "hello,";
	iov[0].iov_base = (void*)p1 ;	//普通消息1
	iov[0].iov_len = strlen("hello,");
     const char*p2 = "world," ;
	iov[1].iov_base = (void*)p2;//普通消息2
	iov[1].iov_len = strlen("world,");
     const char*p3 = "perfect!" ;
	iov[2].iov_base =(void*)p3;//普通消息3
	iov[2].iov_len = strlen("perfect!");
	
	msg.msg_iov = iov;
	msg.msg_iovlen = 3;
	
	if(sendmsg(sock_fd,&msg,0) < 0)	//将消息一起发送给对方
	{
		printf("sendmsg error,errno is %d\n",errno);
		fprintf(stderr,"sendmsg failed.errno:%s\n",strerror(errno));
		return errno;	
	}
	return 1;
	
}
 
int main(int argc,char *argv[])
{
	int sock_fd = 0;
	unlink(SOCK_FILE);
	if(argc != 2)	
	{
		printf("pls usage %s file_send\n",argv[0]);
		exit(EXIT_FAILURE);	
	}	
	sock_fd = create_local_sock((char*)SOCK_FILE);	//创建socket
	if(send_fd(sock_fd,argv[1]) != 1)
	{
		printf("send error");
		exit(EXIT_FAILURE);	
	}
}
