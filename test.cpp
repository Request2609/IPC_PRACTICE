#include <iostream>
#include<sys/uio.h>
#include<string.h>
int main()
{
    struct iovec iov[3] ;
    const char* p1 = "I'" ;
    const char* p2 = "m " ;
    const char* p3 = "programing!\n";
    iov[0].iov_base =(void*) p1 ;
    iov[0].iov_len = strlen(p1) ;
    iov[1].iov_base =(void*) p2 ;
    iov[1].iov_len = strlen(p2) ;
    iov[2].iov_base =(void*) p3 ;
    iov[2].iov_len = strlen(p3) ;
    writev(1, iov, 3) ;
    return 0;
}

