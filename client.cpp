#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys/uio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<sys/un.h>
 
 
#define SOCK_FILE "/tmp/sl"	//通信的socket文件
int create_local_sock(char *sockfile)//创建socket
{
	int local_fd = 0;
	struct sockaddr_un serveraddr;
	if(-1 == (local_fd=socket(AF_LOCAL,SOCK_STREAM,0)))//TCP方式
	{
		perror("socket");
		exit(EXIT_FAILURE);	
	}	
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sun_family = AF_LOCAL;
	strncpy(serveraddr.sun_path,sockfile,strlen(sockfile));
	
	if(-1 == connect(local_fd,(struct sockaddr *)&serveraddr,sizeof(serveraddr)))
	{
		perror("connect");
		exit(EXIT_FAILURE);	
	}
	return local_fd;
}
 
static int recv_fd(int sock_fd,int *fd_to_recv,char *buf,int len)//接收消息
{
	struct cmsghdr *cmsg;
	cmsg = (struct cmsghdr*)alloca(sizeof(struct cmsghdr) + sizeof(fd_to_recv));//分配控制消息存储空间
	cmsg->cmsg_len = sizeof(struct cmsghdr) + sizeof(fd_to_recv);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	
	struct msghdr msg;
	msg.msg_control = cmsg;
	msg.msg_controllen = cmsg->cmsg_len;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	
	struct iovec iov[3];	//普通消息接收空间
	iov[0].iov_base = buf;
	iov[0].iov_len = len;
	msg.msg_iov = iov;
	msg.msg_iovlen = 3;
   /// printf("接收到的信息数据：");
   // writev(1, iov, 3);
	if(recvmsg(sock_fd,&msg,0) < 0)	//接收消息，从服务器端获得的msg结构体的信息保存在recvmsg()函数的第二个参数msg中。
	{   
		printf("recvmsg error,errno is %d\n",errno);
		fprintf(stderr,"recvmsg failed.errno:%s\n",strerror(errno));
		return errno;
	}	
    memcpy(fd_to_recv,(int*)CMSG_DATA(cmsg),sizeof(int));	//获取文件描述符

	if(msg.msg_controllen != cmsg->cmsg_len)
	{
		*fd_to_recv = -1;
			
	}
	return 0;
}
 
int main(int argc,char *argv[])
{
	int sock_fd = 0;
	int file_fd;
	const char *ptr = "now write data to file \n"	;	//写入到文件的内容
	char buf[129];
	memset(buf,'\0',128);
	sock_fd = create_local_sock((char*)SOCK_FILE);		//创建socket
	recv_fd(sock_fd,&file_fd,buf,128);	//接收消息
    char data[1024] ;
    memset(data, 0, sizeof(data)) ;
    printf("file_fd:%d\n", file_fd) ;
    while(read(file_fd, data, sizeof(data)) > 0) {
        printf("读取文件数据：%s", data) ;
    }
	write(file_fd,ptr,strlen(ptr));		//写内容到对方发送过来的fd对应文件
	printf("\n接受的服务端来的消息：%s\n",buf);
	unlink(SOCK_FILE);
}
 
