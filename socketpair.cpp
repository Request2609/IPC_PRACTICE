#include <iostream>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
using namespace std ;

int main()
{
    
    int s[2] ;

    //创建双向管道
    if(socketpair(PF_UNIX, SOCK_STREAM, 0, s) < 0)
    {
        cout << "创建失败" << endl ;
        exit(1) ;
    }

    pid_t pid ;
    pid = fork() ;

    if(pid < 0)
    {
        cout << "创建进程失败！" <<endl ;
        exit(1) ;
    }

    if(pid == 0)
    {
        //关闭读
        close(s[1]) ;
        char buf[1024] = "你是子进程"; 
        while(1)
        {
            write(s[0], buf, sizeof(buf)) ;
            sleep(1) ;
            size_t ss = read(s[0], buf, sizeof(buf)) ;
            if(ss > 0)
            {
                cout << "孩子进程：" <<buf<< endl ;
            }
        }
        
    }
    else
    {
        close(s[0]) ;
        char buf[1024] = "你是父进程"  ;
        while(1)
        {
            int ss = read(s[1], buf, sizeof(buf)) ;
            if(ss > 0)
            {
                cout << "父进程：" << buf <<endl ;
            }
            write(s[1], buf, sizeof(buf)) ;
        }
    }
    return 0;
}

