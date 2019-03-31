#include <iostream>
#include<sys/types.h>
#include<unistd.h>
#include<syslog.h>
using namespace std ;

void deamon()
{
    openlog("/dev/www", LOG_CONS|LOG_PID, LOG_USER) ;
    pid_t pid ;
    pid = fork() ;

    if(pid > 0)
    {
        syslog(LOG_INFO, "父进程关闭") ;
        exit(0) ;
    }
    else if(pid < 0)
    {
        syslog(LOG_ERR, "创建进程出错") ;
        exit(1) ;
    }
    
    //创建进程回话
    int s = setsid() ;
    if(s < 0)
    {
        syslog(LOG_INFO, "创建会话组失败!") ; 
        exit(1) ;
    }
    if(chdir("serverHttp") < 0)
    {
        syslog(LOG_ERR, "切换目录出错") ;
        exit(1) ;        
    }

    close(STDOUT_FILENO) ;
    close(STDIN_FILENO) ;
    close(STDERR_FILENO) ;
    closelog() ;
}
