#include"sys.h"
#include<string>
#include<iostream>
using namespace std ;

//管道是半双工,一种进程间通信的方式
//在操作中需要返回两个文件描述符,用长度的2*sizeof(int)的数组存起来
//其中第一个描述符的是为读操作而创建和打开的
//第二个描述符是为写操作创建和打开的
int main()
{

    int result ;
    int fds[2] ;
    result = pipe( fds ) ;

    if( result == -1 )
    {
        cout << "创建管道失败" << endl ;
        exit( 1 ) ;
    }
    string datas = "管道通信." ;

    int* write_fd = &fds[1] ;
    int* read_fd = &fds[0] ;
    char buffer[100] ;

    pid_t pid ;
    //创建进程
    pid = fork() ;
    
    if( pid == -1 )
    {
        cout << "创建进程失败" << endl ;
        exit( 1 ) ;
    }
    
    if( pid == 0 )
    {
        //关闭读端,给写端填数据
        close( *read_fd ) ;
        result = write( *write_fd , datas.data(), strlen( datas.data()) ) ;
        return  0 ;
    }
    
    else
    {
        close( *write_fd ) ;
        int byte = read( *read_fd, buffer, sizeof(buffer)) ;  
        cout << "read data length = " << byte << endl ;
        cout << buffer <<endl ;
        return 0 ;
    }

    return 0;
}

