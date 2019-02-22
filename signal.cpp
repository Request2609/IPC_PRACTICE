#include <iostream>
#include<signal.h>
using namespace std ;

typedef void(*sighandler_t)(int);

static void sig_handle( int signo )
{   

    if(signo == SIGSTOP )
    {
        cout<<"接收到SIGSTOP"<< endl ;
    }

    if(signo == SIGKILL )
    {
        cout<<"接收到SIGKILL"<<endl ;
    }
    else
    {
       cout<< "接收到信号"<<signo<<endl ;
    }
    return ;
}
int main()
{

    sighandler_t  ret ;
    
    ret =  signal( SIGSTOP,sig_handle ) ;
    if( ret == SIG_ERR ){
        return -1 ;
    }
    ret =  signal(SIGKILL , sig_handle ) ;
    if( ret == SIG_ERR ){
        return -1 ;
    }

    for( ; ; )
    return 0;
}

